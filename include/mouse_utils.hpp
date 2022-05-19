#pragma once

#include <Windows.h>

namespace window_automator::mouse::utils {
// NOLINTBEGIN(readability-identifier-length)

struct Coordinates {
    int x;
    int y;
};

auto click_left(HWND window_handle, Coordinates coordinates) -> void;
auto click_right(HWND window_handle, Coordinates coordinates) -> void;
auto move(HWND window_handle, Coordinates coordinates) -> void;

// NOLINTEND(readability-identifier-length)

} // namespace window_automator::mouse::utils