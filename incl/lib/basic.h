// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#undef NOMINMAX
#define NOMINMAX 1

#ifndef _MSC_VER
#   define EXECUTE_BEFORE_MAIN(FUNC) static void __attribute__((constructor)) FUNC(void)
#else
#   define EXECUTE_BEFORE_MAIN(FUNC) \
        static void FUNC##___f(void); \
        struct FUNC##___t { FUNC##___t(void) { FUNC##___f(); } }; \
        static FUNC##___t FUNC##___s; \
        static void FUNC##___f(void)
#endif

#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <cstdint>
#include <clocale>
#include <memory>
#include <string>
#include <format>

consteval size_t operator""_size_t(const unsigned long long value) noexcept { return value; }

namespace Log {
#ifdef DEBUG
    inline bool s_appendLocation { true };
#else
    inline bool s_appendLocation { false };
#endif
}

namespace Basic {
    namespace Wcs {
        constexpr const wchar_t * c_ok { L"OK" };
        constexpr const wchar_t * c_somethingWrong { L"Что-то пошло не так!" };
        constexpr const wchar_t * c_fallbackErrorMessage { L"[error]" };
        constexpr std::wstring_view c_invalidValue { L"Недопустимое значение" };
        constexpr std::wstring_view c_rangeError { L"Значение вне диапазона" };
        constexpr std::wstring_view c_invalidVariable { L"Ошибка в '{}': {}" };
    }

    namespace Mbs {
        constexpr const char * c_ok { "OK" };
        constexpr const char * c_somethingWrong { "Что-то пошло не так!" };
        constexpr const char * c_fallbackErrorMessage { "[error]" };
    }
}
