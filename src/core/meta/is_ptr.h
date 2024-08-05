#pragma once
namespace Core {
    namespace Meta {
        template<typename ValueType>
        struct IsPtr {
            enum {
                Result = false
            };
        };

        template<typename ValueType>
        struct IsPtr<ValueType*> {
            enum {
                Result = true
            };
        };

        template<typename ValueType>
        struct AddPtr {
            typedef ValueType* Result;
        };

        template<typename ValueType>
        struct AddPtr<ValueType*> {
            typedef ValueType* Result;
        };

        template<typename ValueType>
        struct RemovePtr {
            typedef ValueType Result;
        };

        template<typename ValueType>
        struct RemovePtr<ValueType*> {
            typedef ValueType Result;
        };
    }
}
