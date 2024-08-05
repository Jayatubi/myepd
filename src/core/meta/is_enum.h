#pragma once

namespace Core {
    namespace Meta {
        template<typename ValueType>
        struct IsEnum {
            enum {
                Result = __is_enum(ValueType)
            };
        };
    }
}
