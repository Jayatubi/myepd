#pragma once

#include <limits>

namespace Core {
    typedef unsigned char U8;
    typedef unsigned short U16;
    typedef unsigned int U32;
    typedef unsigned long long U64;

    typedef unsigned long Size;

    typedef signed char S8;
    typedef signed short S16;
    typedef signed int S32;
    typedef signed long long S64;

    typedef float F32;
    typedef double F64;

    static const U8 U8_Max = std::numeric_limits<U8>::max();
    static const U8 U8_Min = std::numeric_limits<U8>::min();
    static const U16 U16_Max = std::numeric_limits<U16>::max();
    static const U16 U16_Min = std::numeric_limits<U16>::min();
    static const U32 U32_Max = std::numeric_limits<U32>::max();
    static const U32 U32_Min = std::numeric_limits<U32>::min();
    static const U64 U64_Max = std::numeric_limits<U64>::max();
    static const U64 U64_Min = std::numeric_limits<U64>::min();

    static const S8 S8_Max = std::numeric_limits<S8>::max();
    static const S8 S8_Min = std::numeric_limits<S8>::min();
    static const S16 S16_Max = std::numeric_limits<S16>::max();
    static const S16 S16_Min = std::numeric_limits<S16>::min();
    static const S32 S32_Max = std::numeric_limits<S32>::max();
    static const S32 S32_Min = std::numeric_limits<S32>::min();
    static const S64 S64_Max = std::numeric_limits<S64>::max();
    static const S64 S64_Min = std::numeric_limits<S64>::min();

    static const F32 F32_Max = std::numeric_limits<F32>::max();
    static const F32 F32_Min = std::numeric_limits<F32>::min();
    static const F64 F64_Max = std::numeric_limits<F64>::max();
    static const F64 F64_Min = std::numeric_limits<F64>::min();

    typedef char Char;
    typedef wchar_t WChar;

    typedef int Socket;
}
