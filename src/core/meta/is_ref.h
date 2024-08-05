#pragma once
namespace Core {
    namespace Meta {
        template<typename ValueType>
        struct IsRef {
            enum {
                Result = std::is_reference<ValueType>::value
            };
        };

        template<typename ValueType>
        struct AddRef {
            typedef ValueType& Result;
        };

        template<typename ValueType>
        struct AddRef<ValueType&> {
            typedef ValueType& Result;
        };

        template<typename ValueType>
        struct RemoveRef {
            typedef ValueType Result;
        };

        template<typename ValueType>
        struct RemoveRef<ValueType&> {
            typedef ValueType Result;
        };
    }
}
