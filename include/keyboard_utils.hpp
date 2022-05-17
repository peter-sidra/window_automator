#pragma once

#include <Windows.h>

namespace window_automator::keyboard::utils {

auto is_shift_down() -> bool;
auto is_ctrl_down() -> bool;
auto is_alt_down() -> bool;
auto is_system_key_down() -> bool;

void post_key_down(HWND window_handle, int key_id);
void post_key_up(HWND window_handle, int key_id);

void key_up(int key_id);
void key_down(int key_id);

} // namespace window_automator::keyboard::utils
