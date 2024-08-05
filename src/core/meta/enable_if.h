#pragma once

namespace Core {
    namespace Meta {
        template<bool test, typename T>
        struct EnableIf {
        };

        template<typename T>
        struct EnableIf<true, T> {
            typedef T Result;
        };
    }
}
