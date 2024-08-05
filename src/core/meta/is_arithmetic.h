#pragma once

namespace Core {
    namespace Meta {
        template<typename ValueType>
        struct IsArithmetic {
            enum {
                Result = std::is_arithmetic<ValueType>::value
            };
        };
    }
}
