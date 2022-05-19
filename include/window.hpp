#pragma once

#include <optional>
#include <string>
#include <windows.h>

#include "keyboard.hpp"
#include "mouse.hpp"

namespace window_automator {

namespace detail {

template <auto config> class WindowKeyboardBase {
  public:
    auto get_keyboard() -> keyboard::Keyboard<config> & {
        return keyboard;
    }

  protected:
    void set_keyboard(keyboard::Keyboard<config> keyboard) {
        this->keyboard = keyboard;
    }

  private:
    keyboard::Keyboard<config> keyboard;
};

template <auto config> class WindowMouseBase {
  public:
    auto get_mouse() -> mouse::Mouse<config> & {
        return mouse;
    }

  protected:
    void set_mouse(mouse::Mouse<config> mouse) {
        this->mouse = mouse;
    }

  private:
    mouse::Mouse<config> mouse;
};

template <auto T = [] {}> struct Empty {
    constexpr Empty(auto &&.../*unused*/) {}
};

} // namespace detail

template <auto keyboard_config = [] {}, auto mouse_config = [] {}>
class Window :
    // clang-format off
    public
    std::conditional_t<std::is_same_v<decltype(keyboard_config), InputConfig>,
                       detail::WindowKeyboardBase<keyboard_config>,
                       detail::Empty<>>,
    public
    std::conditional_t<std::is_same_v<decltype(mouse_config), InputConfig>,
                       detail::WindowMouseBase<mouse_config>,
                       detail::Empty<>> {
    // clang-format on
  public:
    auto is_in_focus() -> bool {
        return handle == GetForegroundWindow();
    };

    auto restore_foreground() -> void {
        if (handle == nullptr) {
            return;
        }
        // SetCapture(handle);
        ShowWindowAsync(handle, SW_RESTORE);
        // BringWindowToTop(handle);
        // SwitchToThisWindow(handle, 0);
    };

    auto restore_background() -> void {
        if (handle == nullptr) {
            return;
        }
        ShowWindowAsync(handle, SW_SHOWNOACTIVATE);
    };

    auto is_minimized() -> bool {
        return IsIconic(handle) != 0;
    };

    auto is_valid() -> bool {
        return IsWindow(handle) != 0;
    };

    auto get_handle() -> HWND {
        return handle;
    };

    static auto create(std::string title)
        -> std::optional<Window<keyboard_config, mouse_config>> {
        auto *handle = find_window_by_title(title.c_str());

        if (handle == nullptr) {
            return std::nullopt;
        }

        return Window{title, handle};
    };

  private:
    Window(std::string title, HWND handle)
        : title{std::move(title)}, handle{handle} {
        if constexpr (std::is_same_v<decltype(keyboard_config), InputConfig>) {
            this->set_keyboard(keyboard::Keyboard<keyboard_config>{handle});
        }
        if constexpr (std::is_same_v<decltype(mouse_config), InputConfig>) {
            this->set_mouse(mouse::Mouse<mouse_config>{handle});
        }
    };

    std::string title;
    HWND handle;

    static auto find_window_by_title(std::string_view window_title) -> HWND {
        // NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
        struct EnumData {
            HWND hWnd = nullptr;
            std::string_view window_title;
        };

        WNDENUMPROC enum_windows_proc = [](HWND hWnd, LPARAM lParam) -> BOOL {
            std::string_view window_title =
                reinterpret_cast<EnumData *>(lParam)->window_title;

            static std::vector<char> buff(window_title.length() + 1, 0);

            GetWindowTextA(hWnd, buff.data(), static_cast<int>(buff.size()));

            if (std::string_view{buff.data()} == window_title) {
                reinterpret_cast<EnumData *>(lParam)->hWnd = hWnd;
                return 0;
            }
            return 1;
        };

        EnumData data{};
        data.window_title = window_title;
        if (EnumWindows(enum_windows_proc, reinterpret_cast<LPARAM>(&data)) ==
            0) {
            return data.hWnd;
        }

        return nullptr;
        // NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)
    };
};

} // namespace window_automator
