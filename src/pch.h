#pragma once

#include <Arduino.h>

inline unsigned long long operator ""_s(unsigned long long ms) {
    return ms * 1000;
}

inline unsigned long long operator ""_m(unsigned long long ms) {
    return ms * 1000 * 60;
}
