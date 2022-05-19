#include "mouse_utils.hpp"

namespace window_automator::mouse::utils {

auto click_left(HWND window_handle, Coordinates coordinates) -> void {
    PostMessage(window_handle, WM_LBUTTONDOWN, 0,
                MAKELPARAM((int)coordinates.x, (int)coordinates.y));
    PostMessage(window_handle, WM_LBUTTONUP, 0,
                MAKELPARAM((int)coordinates.x, (int)coordinates.y));
}

auto click_right(HWND window_handle, Coordinates coordinates) -> void {
    PostMessage(window_handle, WM_RBUTTONDOWN, 0,
                MAKELPARAM((int)coordinates.x, (int)coordinates.y));
    PostMessage(window_handle, WM_RBUTTONUP, 0,
                MAKELPARAM((int)coordinates.x, (int)coordinates.y));
}

auto move(HWND window_handle, Coordinates coordinates) -> void {
    SetCapture(window_handle);
    PostMessage(window_handle, WM_MOUSEMOVE, 0,
                MAKELPARAM((int)coordinates.x, (int)coordinates.y));
}

} // namespace window_automator::mouse::utils
