#pragma once

#include "result.h"

namespace Core {
    namespace Meta {
        template<typename ValueType>
        struct IsVoid {
            enum {
                Result = std::is_void<ValueType>::value
            };
        };
    }
}
