#pragma once

#include <Windows.h>
#include <chrono>
#include <random>
#include <thread>

#include "input_common.hpp"
#include "mouse_utils.hpp"

namespace window_automator::mouse {

template <InputConfig config = InputConfig{}> class Mouse {
  private:
    enum class Action {
        click_left,
        click_right,
        move,
    };

  public:
    Mouse() : window_handle(nullptr){};
    Mouse(HWND window_handle) : window_handle{window_handle} {};

    auto click_left(utils::Coordinates coordinates) -> void {
        dispatch_action<Action::click_left>(coordinates);
    }
    auto click_right(utils::Coordinates coordinates) -> void {
        dispatch_action<Action::click_right>(coordinates);
    }
    auto move(utils::Coordinates coordinates) -> void {
        dispatch_action<Action::move>(coordinates);
    }

  private:
    HWND window_handle;

    template <Action action>
    auto dispatch_action(utils::Coordinates coordinates) {
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

        if constexpr (action == Action::click_left) {
            mouse::utils::click_left(window_handle, coordinates);
        } else if constexpr (action == Action::click_right) {
            mouse::utils::click_right(window_handle, coordinates);
        } else if constexpr (action == Action::move) {
            mouse::utils::move(window_handle, coordinates);
        }

        if constexpr (config.random_delay) {
            std::this_thread::sleep_for(random_ms());
        }
    }
};

} // namespace window_automator::mouse