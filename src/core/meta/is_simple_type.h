#pragma once

#include "result.h"
#include "is_ptr.h"
#include "is_pod.h"

namespace Core {
    namespace Meta {
        template<typename ValueType>
        struct IsSimpleType : public IfThenElse<
            (IsPOD<ValueType>::Result || IsClass<ValueType>::Result) && !IsPtr<ValueType>::Result,
            TrueType,
            FalseType>::Result {
        };
    }
}
