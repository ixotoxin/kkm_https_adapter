// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "text.h"
#include <cwctype>

[[nodiscard, maybe_unused]]
bool Meta::Wcs::trimPredicate(wchar_t c) {
    // return std::iswspace(c);
    return c == L' ' || c == L'\n' || c == L'\r' || c == L'\t' || c == L'\v' || c == L'\f';
}

[[nodiscard, maybe_unused]]
bool Meta::Wcs::noTrimPredicate(wchar_t c) {
    // return !std::iswspace(c);
    return c != L' ' && c != L'\n' && c != L'\r' && c != L'\t' && c != L'\v' && c != L'\f';
}

[[nodiscard, maybe_unused]]
wchar_t Meta::Wcs::toLower(wchar_t c) {
    // ISSUE: Не совсем корректный способ, но для данного проекта достаточен.
    return static_cast<wchar_t>(std::towlower(c));
}

[[nodiscard, maybe_unused]]
bool Meta::Mbs::trimPredicate(unsigned char c) {
    // return std::isspace(c);
    // return c <= 0x20u || c == 0x7fu;
    return c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == '\v' || c == '\f';
}

[[nodiscard, maybe_unused]]
bool Meta::Mbs::noTrimPredicate(unsigned char c) {
    // return !std::isspace(c);
    // return c > 0x20u && c != 0x7fu;
    return c != ' ' && c != '\n' && c != '\r' && c != '\t' && c != '\v' && c != '\f';
}

[[nodiscard, maybe_unused]]
char Meta::Mbs::toLower(unsigned char c) {
    // ISSUE: Не совсем корректный способ, но для данного проекта достаточен.
    return static_cast<Char>(std::tolower(c));
}
