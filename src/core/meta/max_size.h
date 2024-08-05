#pragma once
namespace Core {
    namespace Meta {
        template<typename FisrtType, typename... RestTypes>
        struct MaxSize {
            enum {
                Result = Max<sizeof(FisrtType), MaxSize<RestTypes...>::Result>::Result
            };
        };

        template<typename FirstType>
        struct MaxSize<FirstType> {
            enum {
                Result = sizeof(FirstType)
            };
        };
    }
}
