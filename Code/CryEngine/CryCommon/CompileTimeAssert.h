// Compile-time assert.
// Syntax: COMPILE_TIME_ASSERT(BooleanExpression);
// 

#pragma once

#if defined(__cplusplus) && !defined(__RECODE__)

#define COMPILE_TIME_ASSERT(expr) static_assert(expr,#expr)

#else

#define COMPILE_TIME_ASSERT(expr)

#endif
