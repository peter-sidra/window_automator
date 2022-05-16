#include "keyboard_utils.hpp"

namespace WindowAutomator::KeyboardUtils {

auto is_shift_down() -> bool {
    SHORT shiftKeyState = GetAsyncKeyState(VK_SHIFT);
    return ((1 << (sizeof(SHORT) * 8 - 1)) & shiftKeyState) != 0;
}

auto is_ctrl_down() -> bool {
    SHORT ctrlKeyState = GetAsyncKeyState(VK_CONTROL);
    return ((1 << (sizeof(SHORT) * 8 - 1)) & ctrlKeyState) != 0;
}

auto is_alt_down() -> bool {
    SHORT altKeyState = GetAsyncKeyState(VK_MENU);
    return ((1 << (sizeof(SHORT) * 8 - 1)) & altKeyState) != 0;
}

auto is_system_key_down() -> bool {
    // Check if shift or ctrl or alt is pressed
    return is_shift_down() || is_ctrl_down() || is_alt_down();
}

void post_key_down(HWND window_handle, int key_id) {
    PostMessage(window_handle, WM_KEYDOWN, key_id, 1);
}

void post_key_up(HWND window_handle, int key_id) {
    PostMessage(window_handle, WM_KEYUP, key_id, 1);
}

void key_up(int key_id) {
    // NOLINTBEGIN(cppcoreguidelines-pro-type-union-access)
    INPUT input;
    input.type = INPUT_KEYBOARD;
    input.ki.wScan = 0;
    input.ki.time = 0;
    input.ki.dwExtraInfo = 0;
    input.ki.wVk = key_id;
    input.ki.dwFlags = 2;
    SendInput(1, &input, sizeof(INPUT));
    // NOLINTEND(cppcoreguidelines-pro-type-union-access)
}

void key_down(int key_id) {
    // NOLINTBEGIN(cppcoreguidelines-pro-type-union-access)
    INPUT input;
    input.type = INPUT_KEYBOARD;
    input.ki.wScan = 0;
    input.ki.time = 0;
    input.ki.dwExtraInfo = 0;
    input.ki.wVk = key_id;
    input.ki.dwFlags = 0;
    SendInput(1, &input, sizeof(INPUT));
    // NOLINTEND(cppcoreguidelines-pro-type-union-access)
}

} // namespace WindowAutomator::KeyboardUtils