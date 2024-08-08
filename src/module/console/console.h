#pragma once

#include "module/singleton.h"

#include <Arduino.h>

class Console : public Singleton<Console> {
    typedef decltype(Serial) LowlevelType;
public:
    Console();

    ~Console();

    template<typename T>
    void print(T&& data)
    {
        serial().print(data);
    }

    template <typename... T>
    void printf(const char* format, T ... args) {
        serial().printf(format, args ...);
    }

public:

    LowlevelType& serial();
};
