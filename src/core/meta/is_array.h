#pragma once

namespace Core {
    namespace Meta {
        template<typename ValueType>
        struct IsArray {
            enum {
                Result = std::is_array<ValueType>::value
            };
        };

        template<typename ValueType, int Size>
        constexpr inline int ArraySize(const ValueType (&)[Size]) {
            return Size;
        };
    }
}
