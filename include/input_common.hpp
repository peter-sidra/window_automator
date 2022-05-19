#pragma once

namespace window_automator {

struct InputConfig {
    bool random_delay = false;
    long long delay_min = 1;
    long long delay_max = 5;
    bool ignore_when_sys_key_pressed = false;
};

} // namespace window_automator
