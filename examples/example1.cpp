#include <chrono>
#include <iostream>
#include <thread>

#include "keyboard.hpp"
#include "window.hpp"

auto main() -> int {
    using namespace window_automator;

    // Create a window with keyboard and mouse input
    auto window = Window<InputConfig{}, InputConfig{}>::create("example1.cpp");

    if (!window) {
        std::cout << "Could not find window" << std::endl;
        return 1;
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::cout << "Window has focus: " << window->is_in_focus() << std::endl;
    std::cout << "Window is minimized: " << window->is_minimized() << std::endl;

    window->get_keyboard().post_key_up('P');
    window->get_keyboard().post_key_up('E');
    window->get_keyboard().post_key_up('T');
    window->get_keyboard().post_key_up('E');
    window->get_keyboard().post_key_up('R');

    window->get_mouse().click_right({.x = 500, .y = 500});

    // This should fail to compile
    // auto window_only = Window<>::create("example1.cpp");
    // window_only->get_keyboard().post_key_up('P');
    // window_only->get_mouse().click_right({.x = 500, .y = 500});
}