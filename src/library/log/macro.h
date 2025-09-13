// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#ifdef EXTERNAL_KKM_VARIABLES
#   define LOG_DEF(TYPE, NAME, VALUE)
#   define LOG_CONST(TYPE, NAME, VALUE) extern const TYPE NAME
#   define LOG_VAR(TYPE, NAME, VALUE) extern TYPE NAME
#else
#   define LOG_DEF(TYPE, NAME, VALUE) constexpr const TYPE NAME { VALUE }
#   define LOG_CONST(TYPE, NAME, VALUE) constexpr const TYPE NAME { VALUE }
#   define LOG_VAR(TYPE, NAME, VALUE) inline TYPE NAME { VALUE }
#endif

#ifdef EXTERNAL_LOG_STRINGS
#   define LOG_STR(NAME, VALUE) extern const std::string_view NAME
#   define LOG_WSTR(NAME, VALUE) extern const std::wstring_view NAME
#else
#   define LOG_STR(NAME, VALUE) constexpr const std::string_view NAME { VALUE }
#   define LOG_WSTR(NAME, VALUE) constexpr const std::wstring_view NAME { VALUE }
#endif
