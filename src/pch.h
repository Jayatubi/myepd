#pragma once

#include <Arduino.h>

inline unsigned long long operator ""_s(unsigned long long s) {
    return s * 1000;
}

inline unsigned long long operator ""_m(unsigned long long m) {
    return m * 1000 * 60;
}
