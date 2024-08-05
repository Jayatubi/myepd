#pragma once

#include "result.h"

namespace Core {
    namespace Meta {
        template<typename BaseType, typename DerivedType>
        struct IsDerived {
            enum {
                Result = __is_base_of(BaseType, DerivedType)
            };
        };
    }
}
