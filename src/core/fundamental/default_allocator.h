#pragma once

#include "../basic_type/basic_type.h"
#include <malloc.h>
#include <cstring>

namespace Core {

    struct DefaultAllocator {
        template<typename ValueType, typename ... ArgTypes>
        static ValueType* alloc(ArgTypes ... args) {
            void* ptr = malloc(sizeof(ValueType));
            return new(ptr) ValueType(args ...);
        }

        template<typename ValueType, typename ... ArgTypes>
        static ValueType* zero_alloc(ArgTypes ... args) {
            void* ptr = malloc(sizeof(ValueType));
            memset(ptr, 0, sizeof(ValueType));
            return new(ptr) ValueType(args ...);
        }

        template<typename ValueType>
        static void free(ValueType* pointee) {
            if (pointee != nullptr) {
                pointee->~ValueType();
                ::free(pointee);
                pointee = nullptr;
            }
        }
    };
}
