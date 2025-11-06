// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#ifndef EXECUTE_BEFORE_MAIN
#   ifndef _MSC_VER
#       define EXECUTE_BEFORE_MAIN(FUNC) static void __attribute__((constructor)) FUNC(void)
#   else
#       define EXECUTE_BEFORE_MAIN(FUNC) \
            inline void FUNC##___f(void); \
            struct FUNC##___t { FUNC##___t(void) { FUNC##___f(); } }; \
            inline FUNC##___t FUNC##___s; \
            inline void FUNC##___f(void)
#   endif
#endif

#ifdef EXTERNAL_LIB_VARIABLES
#   define LIB_DEF(TYPE, NAME, VALUE)
#   define LIB_CONST(TYPE, NAME, VALUE) extern const TYPE NAME
#   define LIB_VAR(TYPE, NAME, VALUE) extern TYPE NAME
#else
#   define LIB_DEF(TYPE, NAME, VALUE) constexpr const TYPE NAME { VALUE }
#   define LIB_CONST(TYPE, NAME, VALUE) constexpr const TYPE NAME { VALUE }
#   define LIB_VAR(TYPE, NAME, VALUE) inline TYPE NAME { VALUE }
#endif

#ifdef EXTERNAL_LIB_STRINGS
#   define LIB_CSTR(NAME, VALUE) extern const char * NAME
#   define LIB_WCSTR(NAME, VALUE) extern const wchar_t * NAME
#   define LIB_STR(NAME, VALUE) extern const std::string_view NAME
#   define LIB_FMT(F, A, ...) std::vformat(F, std::make_format_args(A __VA_OPT__(,) __VA_ARGS__))
#   define LIB_WSTR(NAME, VALUE) extern const std::wstring_view NAME
#   define LIB_WFMT(F, A, ...) std::vformat(F, std::make_wformat_args(A __VA_OPT__(,) __VA_ARGS__))
#else
#   define LIB_CSTR(NAME, VALUE) constexpr const char * NAME { VALUE }
#   define LIB_WCSTR(NAME, VALUE) constexpr const wchar_t * NAME { VALUE }
#   define LIB_STR(NAME, VALUE) constexpr const std::string_view NAME { VALUE }
#   define LIB_FMT(F, A, ...) std::format(F, A __VA_OPT__(,) __VA_ARGS__)
#   define LIB_WSTR(NAME, VALUE) constexpr const std::wstring_view NAME { VALUE }
#   define LIB_WFMT(F, A, ...) std::format(F, A __VA_OPT__(,) __VA_ARGS__)
#endif
