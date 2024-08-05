#pragma once

#include "result.h"

namespace Core {
    namespace Meta {
        template<typename ValueType>
        struct IsPOD {
            enum {
                Result = std::is_pod<ValueType>::value
            };
        };
    }
}
