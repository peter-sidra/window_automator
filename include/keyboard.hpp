#pragma once

#include <chrono>
#include <random>
#include <thread>

#include "keyboard_utils.hpp"

namespace WindowAutomator::Keyboard {

struct Config {
    bool random_delay = false;
    long long delay_min = 2;
    long long delay_max = 10;
    bool ignore_when_sys_key_pressed = false;
};

template <Keyboard::Config config = Config{}> class Keyboard {
  private:
    enum class Action {
        key_down,
        key_up,
        post_key_down,
        post_key_up,
    };

  public:
    Keyboard() : window_handle(nullptr){};
    Keyboard(HWND window_handle) : window_handle{window_handle} {};

    auto set_window_handle(HWND window_handle) -> void {
        this->window_handle = window_handle;
    };

    auto post_key_down(int key_id) const -> void {
        dispatch_action<Action::post_key_down>(key_id);
    };
    auto post_key_up(int key_id) const -> void {
        dispatch_action<Action::post_key_up>(key_id);
    };
    auto key_up(int key_id) const -> void {
        dispatch_action<Action::key_up>(key_id);
    };
    auto key_down(int key_id) const -> void {
        dispatch_action<Action::key_down>(key_id);
    };

  private:
    HWND window_handle;

    template <Action action> void dispatch_action(int key_id) const {
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
            if (KeyboardUtils::is_system_key_down()) {
                return;
            }
        }

        if constexpr (action == Action::key_down) {
            KeyboardUtils::key_down(key_id);
        } else if constexpr (action == Action::key_up) {
            KeyboardUtils::key_up(key_id);
        } else if constexpr (action == Action::post_key_down) {
            KeyboardUtils::post_key_down(window_handle, key_id);
        } else if constexpr (action == Action::post_key_up) {
            KeyboardUtils::post_key_up(window_handle, key_id);
        }

        // Add random delay if enabled
        if constexpr (config.random_delay) {
            std::this_thread::sleep_for(random_ms());
        }
    }
};

} // namespace WindowAutomator::Keyboard
