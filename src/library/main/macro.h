// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <cmake/options.h>

#ifdef EXTERNAL_MAIN_VARIABLES
#   define MAIN_DEF(TYPE, NAME, VALUE)
#   define MAIN_CONST(TYPE, NAME, VALUE) extern const TYPE NAME
#   define MAIN_VAR(TYPE, NAME, VALUE) extern TYPE NAME
#else
#   define MAIN_DEF(TYPE, NAME, VALUE) constexpr const TYPE NAME { VALUE }
#   define MAIN_CONST(TYPE, NAME, VALUE) constexpr const TYPE NAME { VALUE }
#   define MAIN_VAR(TYPE, NAME, VALUE) inline TYPE NAME { VALUE }
#endif

#ifdef EXTERNAL_MAIN_STRINGS
#   define MAIN_STR(NAME, VALUE) extern const std::string_view NAME
#   define MAIN_FMT(F, A, ...) std::vformat(F, std::make_format_args(A __VA_OPT__(,) __VA_ARGS__))
#   define MAIN_WSTR(NAME, VALUE) extern const std::wstring_view NAME
#   define MAIN_WFMT(F, A, ...) std::vformat(F, std::make_wformat_args(A __VA_OPT__(,) __VA_ARGS__))
#else
#   define MAIN_STR(NAME, VALUE) constexpr const std::string_view NAME { VALUE }
#   define MAIN_FMT(F, A, ...) std::format(F, A __VA_OPT__(,) __VA_ARGS__)
#   define MAIN_WSTR(NAME, VALUE) constexpr const std::wstring_view NAME { VALUE }
#   define MAIN_WFMT(F, A, ...) std::format(F, A __VA_OPT__(,) __VA_ARGS__)
#endif

#if BUILD_SEPARATED
#   define MON_SEP(MON, SEP) SEP
#else
#   define MON_SEP(MON, SEP) MON
#endif
