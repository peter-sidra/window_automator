#pragma once

#include <Windows.h>

namespace WindowAutomator::KeyboardUtils {

auto is_shift_down() -> bool;
auto is_ctrl_down() -> bool;
auto is_alt_down() -> bool;
auto is_system_key_down() -> bool;

void post_key_down(HWND window_handle, int key_id);
void post_key_up(HWND window_handle, int key_id);

void key_up(int key_id);
void key_down(int key_id);

} // namespace WindowAutomator::KeyboardUtils
