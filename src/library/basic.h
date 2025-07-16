// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#undef NOMINMAX
#define NOMINMAX 1

#ifdef WITH_MEMORY_PROFILING
#   ifndef DEBUG
#       define DEBUG
#   endif
#   ifndef _DEBUG
#       define _DEBUG
#   endif
#   define _CRTDBG_MAP_ALLOC // NOLINT(*-reserved-identifier)
#endif

#include <windows.h>

#ifdef WITH_MEMORY_PROFILING
#   include <cstdlib>
#   include <crtdbg.h>
#endif

#include <cstdint>
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
        constexpr const wchar_t * c_somethingWrong { L"Что-то пошло не так!" };
        constexpr const wchar_t * c_fallbackErrorMessage { L"[error]" };
        constexpr std::wstring_view c_invalidValue { L"Недопустимое значение" };
        constexpr std::wstring_view c_rangeError { L"Значение вне диапазона" };
        constexpr std::wstring_view c_invalidVariable { L"Ошибка в '{}': {}" };
    }

    namespace Mbs {
        constexpr const char * c_somethingWrong { "Что-то пошло не так!" };
        constexpr const char * c_fallbackErrorMessage { "[error]" };
    }
}

#ifdef ENABLE_MEMORY_PROFILING
#undef ENABLE_MEMORY_PROFILING
#endif

#ifdef MEMORY_LEAK
#undef MEMORY_LEAK
#endif

#ifdef WITH_MEMORY_PROFILING
#   define ENABLE_MEMORY_PROFILING \
        do { \
            constexpr auto _reportMode_ = /*_CRTDBG_MODE_DEBUG |*/ _CRTDBG_MODE_FILE /*| _CRTDBG_MODE_WNDW*/; \
            ::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_LEAK_CHECK_DF); \
            ::_CrtSetReportMode(_CRT_ASSERT, _reportMode_); \
            ::_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR); \
            ::_CrtSetReportMode(_CRT_WARN, _reportMode_); \
            ::_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR); \
            ::_CrtSetReportMode(_CRT_ERROR, _reportMode_); \
            ::_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR); \
            Log::Console::write(Log::Level::Warning, L"Memory profiling enabled"); \
        } while (false)
#   ifdef WITH_MEMORY_LEAKS
#       define MEMORY_LEAK \
            do { \
                auto _memoryLeak_ = new unsigned short[32] { 0xadde, 0xefbe }; \
                std::wstring _memoryLeakString_ { \
                    std::format( \
                        L"I smell {:x}{:x} {:x}{:x} (test message indicating that a leak has taken place)", \
                        _memoryLeak_[0] & 0xff, _memoryLeak_[0] >> 8, _memoryLeak_[1] & 0xff, _memoryLeak_[1] >> 8 \
                    ) \
                }; \
                Log::Console::write(Log::Level::Warning, _memoryLeakString_); \
            } while (false)
#   endif
#endif

#ifndef ENABLE_MEMORY_PROFILING
#define ENABLE_MEMORY_PROFILING do {} while (false)
#endif

#ifndef MEMORY_LEAK
#define MEMORY_LEAK do {} while (false)
#endif
