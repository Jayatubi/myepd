#pragma once

namespace Core {
    namespace Meta {
        namespace Details {
            struct EmptyType {
            };
            typedef EmptyType _Yes[1];
            typedef EmptyType _No[2];
        }

        typedef Details::_Yes& Yes;
        typedef Details::_No& No;

        struct TrueType {
            enum {
                Result = 1
            };
        };

        struct FalseType {
            enum {
                Result = 0
            };
        };
    }
}
