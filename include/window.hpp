#pragma once

#include <optional>
#include <string>
#include <windows.h>

#include "keyboard.hpp"

namespace window_automator {

namespace detail {

template <auto keyboard_config> class WindowKeyboardBase {
  public:
    auto get_keyboard() -> keyboard::Keyboard<keyboard_config> & {
        return keyboard;
    }

  protected:
    auto set_keyboard(keyboard::Keyboard<keyboard_config> keyboard) -> void {
        this->keyboard = keyboard;
    }

  private:
    keyboard::Keyboard<keyboard_config> keyboard;
};

template <auto T = [] {}> struct Empty {
    constexpr Empty(auto &&.../*unused*/) {}
};

} // namespace detail

template <auto keyboard_config = [] {}>
class Window
    : public std::conditional_t<
          std::is_same_v<decltype(keyboard_config), keyboard::Config>,
          detail::WindowKeyboardBase<keyboard_config>, detail::Empty<>> {

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
        -> std::optional<Window<keyboard_config>> {
        auto *handle = find_window_by_title(title.c_str());

        if (handle == nullptr) {
            return std::nullopt;
        }

        return Window{title, handle};
    };

  private:
    Window(std::string title, HWND handle)
        : title{std::move(title)}, handle{handle} {
        if constexpr (std::is_same_v<decltype(keyboard_config),
                                     keyboard::Config>) {
            this->set_keyboard(keyboard::Keyboard<keyboard_config>{handle});
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
