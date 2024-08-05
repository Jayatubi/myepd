#pragma once


namespace Core {
    struct StringHash {
        template<int N, int I>
        struct Internal {
            constexpr static int Calculate(const char (& str)[N]) {
                return (Internal<N, I - 1>::Calculate(str) ^ (str[I - 1] & 0xFF)) * PRIME;
            }
        };

        template<int N>
        struct Internal<N, 1> {
            constexpr static int Calculate(const char (& str)[N]) {
                return (OFFSET ^ (str[0] & 0xFF)) * PRIME;
            }
        };

        template<int N>
        constexpr static int StaticHash(const char (& str)[N]) {
            return Internal<N, N>::Calculate(str);
        }

        static const unsigned OFFSET = 0x01234567;
        static const unsigned PRIME = 0x89ABCDEF;

        static unsigned Hash(const char* bytes, unsigned length = 0) {
            length = length ? length : strlen(bytes);

            unsigned result = 0;
            if (bytes != nullptr) {
                result = OFFSET;

                for (unsigned i = 0; i < length; i++) {
                    result = (result ^ (bytes[i] & 0xFF)) * PRIME;
                }
            }
            return result;
        }

    };
}
