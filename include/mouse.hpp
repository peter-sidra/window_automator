#pragma once

#include <Windows.h>
#include <chrono>
#include <random>
#include <thread>

#include "input_common.hpp"
#include "mouse_utils.hpp"

namespace window_automator::mouse {

template <InputConfig config = InputConfig{}> class Mouse {
  public:
    Mouse() : window_handle(nullptr){};
    Mouse(HWND window_handle) : window_handle{window_handle} {};

    auto click_left(utils::Coordinates coordinates) -> void {
        dispatch_action([this, coordinates] {
            utils::click_left(window_handle, coordinates);
        });
    }
    auto click_right(utils::Coordinates coordinates) -> void {
        dispatch_action([this, coordinates] {
            utils::click_right(window_handle, coordinates);
        });
    }
    auto move(utils::Coordinates coordinates) -> void {
        dispatch_action(
            [this, coordinates] { utils::move(window_handle, coordinates); });
    }

  private:
    HWND window_handle;

    template <typename Action>
        requires requires(Action action) { std::invoke(action); }
    auto dispatch_action(Action action) {
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

        std::invoke(action);

        if constexpr (config.random_delay) {
            std::this_thread::sleep_for(random_ms());
        }
    }
};

} // namespace window_automator::mouse