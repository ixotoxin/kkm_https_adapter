// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#ifdef EXTERNAL_CONFIG_VARIABLES
#   define CONFIG_DEF(TYPE, NAME, VALUE)
#   define CONFIG_CONST(TYPE, NAME, VALUE) extern const TYPE NAME
#   define CONFIG_VAR(TYPE, NAME, VALUE) extern TYPE NAME
#else
#   define CONFIG_DEF(TYPE, NAME, VALUE) constexpr const TYPE NAME { VALUE }
#   define CONFIG_CONST(TYPE, NAME, VALUE) constexpr const TYPE NAME { VALUE }
#   define CONFIG_VAR(TYPE, NAME, VALUE) inline TYPE NAME { VALUE }
#endif

#ifdef EXTERNAL_CONFIG_STRINGS
#   define CONFIG_STR(NAME, VALUE) extern const std::string_view NAME
#   define CONFIG_WSTR(NAME, VALUE) extern const std::wstring_view NAME
#else
#   define CONFIG_STR(NAME, VALUE) constexpr const std::string_view NAME { VALUE }
#   define CONFIG_WSTR(NAME, VALUE) constexpr const std::wstring_view NAME { VALUE }
#endif
