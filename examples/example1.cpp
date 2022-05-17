#include <chrono>
#include <iostream>
#include <thread>

#include "keyboard.hpp"
#include "window.hpp"

auto main() -> int {
    using namespace window_automator;

    auto window = Window<keyboard::Config{}>::create("example1.cpp");
    // auto window = Window<>::create("example1.cpp");

    if (!window) {
        std::cout << "Could not find window" << std::endl;
        return 1;
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::cout << "Window has focus: " << window->is_in_focus() << std::endl;
    std::cout << "Window is minimized: " << window->is_minimized() << std::endl;

    window->get_keyboard().post_key_up('P');

    std::this_thread::sleep_for(std::chrono::seconds(2));
}