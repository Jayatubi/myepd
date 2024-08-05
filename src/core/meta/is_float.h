#pragma once

#include "result.h"

namespace Core {
    namespace Meta {
        template<typename ValueType>
        struct IsFloat {
            enum {
                Result = std::is_floating_point<ValueType>::value
            };
        };
    }
}
