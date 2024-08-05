#pragma once

#include "result.h"

namespace Core {
    namespace Meta {
        template<typename ValueType>
        struct IsScalar {
            enum {
                Result = std::is_scalar<ValueType>::value
            };
        };
    }
}
