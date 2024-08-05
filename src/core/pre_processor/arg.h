#pragma once

#include "expand.h"

#define PP_ARG_COUNT(...) PP_EXPAND(PP_ARG_POPER(_X, ##__VA_ARGS__, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))
#define PP_ARG_POPER(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, N, ...) N

#define PP_ARG_AT_VARIADIC(Index, ...) PP_CAT(PP_ARG_AT_, Index)(PP_EXPAND(__VA_ARGS__) , )

#define PP_ARG_AT_VARIADIC_0(_0, ...) _0
#define PP_ARG_AT_VARIADIC_1(_0, _1, ...) _1
#define PP_ARG_AT_VARIADIC_2(_0, _1, _2, ...) _2
#define PP_ARG_AT_VARIADIC_3(_0, _1, _2, _3, ...) _3
#define PP_ARG_AT_VARIADIC_4(_0, _1, _2, _3, _4, ...) _4
#define PP_ARG_AT_VARIADIC_5(_0, _1, _2, _3, _4, _5, ...) _5
#define PP_ARG_AT_VARIADIC_6(_0, _1, _2, _3, _4, _5, _6, ...) _6
#define PP_ARG_AT_VARIADIC_7(_0, _1, _2, _3, _4, _5, _6, _7, ...) _7
#define PP_ARG_AT_VARIADIC_8(_0, _1, _2, _3, _4, _5, _6, _7, _8, ...) _8
#define PP_ARG_AT_VARIADIC_9(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, ...) _9
#define PP_ARG_AT_VARIADIC_10(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, ...) _10
#define PP_ARG_AT_VARIADIC_11(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, ...) _11
#define PP_ARG_AT_VARIADIC_12(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, ...) _12
#define PP_ARG_AT_VARIADIC_13(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, ...) _13
#define PP_ARG_AT_VARIADIC_14(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, ...) _14
#define PP_ARG_AT_VARIADIC_15(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, ...) _15
#define PP_ARG_AT_VARIADIC_16(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, ...) _16

#define PP_ARG_AT(Index, Data) PP_EXPAND( PP_CAT(PP_ARG_AT_VARIADIC_, Index) Data)


















