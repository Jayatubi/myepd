#pragma once

#include "if_then_else.h"
#include "is_same.h"
#include "is_ptr.h"
#include "is_ref.h"
#include "const.h"
#include "result.h"
#include "../basic_type/basic_type.h"

namespace Core {
    namespace Meta {
        template<int N, int... Rest>
        struct Min {
            enum {
                Result = IfThenElseValue<(N < Min<Rest...>::Result), N, Min<Rest...>::Result>::Result
            };
        };

        template<int N>
        struct Min<N> {
            enum {
                Result = N
            };
        };

        template<int N, int... Rest>
        struct Max {
            enum {
                Result = IfThenElseValue<(N > Max<Rest...>::Result), N, Max<Rest...>::Result>::Result
            };
        };

        template<int N>
        struct Max<N> {
            enum {
                Result = N
            };
        };

        template<unsigned N>
        struct NextPOT {
            enum {
                Step0 = N - 1,
                Step1 = Step0 | (Step0 >> 1),
                Step2 = Step1 | (Step1 >> 2),
                Step4 = Step2 | (Step2 >> 4),
                Step8 = Step4 | (Step4 >> 8),
                Step16 = Step8 | (Step8 >> 16),
                Step32 = Step16, // | (Step16 >> 32),
                Result = Step32 + 1
            };
        };

        template<typename ValueType>
        struct RemoveAll {
            typedef typename RemoveConst<typename RemovePtr<typename RemoveRef<ValueType>::Result>::Result>::Result Result;
        };

        template<typename ValueType, typename... ArgTypes>
        struct IsCallable {
            template<typename TestType>
            struct Matcher;

            template<typename TestType>
            static Meta::Yes Tester(Matcher<decltype(std::declval<ValueType>()(std::declval<ArgTypes>()...))>*);

            template<typename TestType>
            static Meta::No Tester(...);

            enum {
                Result = IsSame<decltype(Tester<ValueType>(nullptr)), Meta::Yes>::Result
            };
        };

        template<typename DstType, typename SrcType>
        struct IsConvertable {
            static Meta::Yes Tester(const DstType&);

            static Meta::No Tester(...);

            enum {
                Result = IsSame<decltype(Tester(std::declval<SrcType>())), Meta::Yes>::Result
            };
        };

        template<typename ValueType>
        struct IsComparable {
            template<typename TestType>
            struct Matcher;

            template<typename TestType>
            static Meta::Yes Tester(Matcher<decltype(std::declval<TestType>() == std::declval<TestType>())>*);

            template<typename TestType>
            static Meta::No Tester(...);

            enum {
                Result = IsSame<decltype(Tester<ValueType>(nullptr)), Meta::Yes>::Result
            };
        };

        template<U32 Value>
        struct ConstEvaluate {
            enum {
                Result = Value
            };
        };
    }
}
