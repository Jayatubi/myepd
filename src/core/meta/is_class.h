#pragma once

#include "result.h"

namespace Core {
    namespace Meta {
        template<typename ValueType>
        struct IsClass {
            enum {
                Result = std::is_class<ValueType>::value
            };
        };
    }
}
