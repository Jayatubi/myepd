#pragma once
namespace Core {
    namespace Meta {
        template<bool Condition, typename Then, typename Else>
        struct IfThenElse {
            typedef Then Result;
        };

        template<typename Then, typename Else>
        struct IfThenElse<false, Then, Else> {
            typedef Else Result;
        };

        template<bool cond, int Then, int Else>
        struct IfThenElseValue {
            enum {
                Result = Then
            };
        };

        template<int Then, int Else>
        struct IfThenElseValue<false, Then, Else> {
            enum {
                Result = Else
            };
        };

        template<bool Value, typename Type>
        struct Case {
            enum {
                Condition = Value
            };

            typedef Type Result;
        };

        template<typename Default, typename First, typename... Rests>
        struct Switch {
            typedef typename IfThenElse<First::Condition,
                                        typename First::Result,
                                        typename Switch<Default, Rests...>::Result>::Result Result;
        };

        template<typename Default, typename First>
        struct Switch<Default, First> {
            typedef typename IfThenElse<First::Condition, typename First::Result, Default>::Result Result;
        };
    }
}
