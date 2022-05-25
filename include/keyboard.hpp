#pragma once

#include <chrono>
#include <random>
#include <thread>

#include "input_common.hpp"
#include "keyboard_utils.hpp"

namespace window_automator::keyboard {

template <InputConfig config = InputConfig{}> class Keyboard {
  public:
    Keyboard() : window_handle(nullptr){};
    Keyboard(HWND window_handle) : window_handle{window_handle} {};

    auto set_window_handle(HWND window_handle) -> void {
        this->window_handle = window_handle;
    };

    auto post_key_down(int key_id) const -> void {
        dispatch_action(
            [this, key_id] { utils::post_key_down(window_handle, key_id); });
    };
    auto post_key_up(int key_id) const -> void {
        dispatch_action(
            [this, key_id] { utils::post_key_up(window_handle, key_id); });
    };
    auto key_up(int key_id) const -> void {
        dispatch_action([key_id] { utils::key_up(key_id); });
    };
    auto key_down(int key_id) const -> void {
        dispatch_action([key_id] { utils::key_down(key_id); });
    };

  private:
    HWND window_handle;

    template <typename Action>
        requires requires(Action action) { std::invoke(action); }
    void dispatch_action(Action action) const {
        static auto random_ms = [] {
            // Initialize the random number generator if needed
            if constexpr (config.random_delay) {
                static std::random_device rdv;
                static std::mt19937 gen(rdv());
                static std::uniform_int_distribution<decltype(config.delay_min)>
                    dist(config.delay_min, config.delay_max);
                return std::chrono::milliseconds(dist(gen));
            }
        };

        if constexpr (config.ignore_when_sys_key_pressed) {
            if (utils::is_system_key_down()) {
                return;
            }
        }

        std::invoke(action);

        // Add random delay if enabled
        if constexpr (config.random_delay) {
            std::this_thread::sleep_for(random_ms());
        }
    }
};

} // namespace window_automator::keyboard
