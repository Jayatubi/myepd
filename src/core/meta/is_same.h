#pragma once

#include <type_traits>

namespace Core {
    namespace Meta {
        template<typename ValueTypeA, typename ValueTypeB>
        struct IsSame {
            enum {
                Result = std::is_same<ValueTypeA, ValueTypeB>::value
            };
        };
    }
}
