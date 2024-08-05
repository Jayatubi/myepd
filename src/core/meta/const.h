#pragma once

/*
 These meta classes are used to add/remove the 'const' quantifier of a specified type.

 Example:

    typedef int MyType;
    typedef const int MyConstType;

    Meta::AddConst<MyType>::Result              // Result is const int
    Meta::RemoveConst<MyConstType>::Result      // Result is: int
 */
namespace Core {
    namespace Meta {
        template<typename ValueType>
        struct RemoveConst {
            typedef ValueType Result;
        };

        template<typename ValueType>
        struct RemoveConst<const ValueType> {
            typedef ValueType Result;
        };

        template<typename ValueType>
        struct AddConst {
            typedef const ValueType Result;
        };

        template<typename ValueType>
        struct AddConst<const ValueType> {
            typedef const ValueType Result;
        };
    }
}
