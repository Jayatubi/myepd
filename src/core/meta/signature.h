#pragma once

#include "../basic_type/tuple.h"

namespace Core {
    namespace Meta {
        template<typename>
        struct Signature;

        template<typename _ResultType, typename... ArgTypes>
        struct Signature<_ResultType (*)(ArgTypes...)> {
            typedef _ResultType ResultType;
            typedef Tuple<ArgTypes...> ParamType;

            template<U32 Index>
            struct Arg {
                typedef typename TupleAt<Index, ParamType>::ValueType ValueType;
            };
        };
    }
}
