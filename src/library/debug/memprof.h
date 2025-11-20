// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#ifdef WITH_ASAN
#   if WITH_ASAN && defined(WITH_CRTDBG)
#       undef WITH_CRTDBG
#       define WITH_CRTDBG 0
#   endif
#else
#   define WITH_ASAN 0
#endif

#ifndef WITH_UBSAN
#   define WITH_UBSAN 0
#endif

#ifndef WITH_CRTDBG
#   define WITH_CRTDBG 0
#endif

#if WITH_ASAN || WITH_CRTDBG
#   if defined(DEBUG) || defined(_DEBUG)
#       include <log/core.h>
#   else
#       error Memory profiling allowed only in debug version
#   endif
#else
#   ifdef WITH_LEAKS
#       undef WITH_LEAKS
#   endif
#endif

#ifndef WITH_LEAKS
#   define WITH_LEAKS 0
#endif

#undef FORCE_MEMORY_LEAK

#if WITH_LEAKS
#   define FORCE_MEMORY_LEAK \
        do { \
            auto memoryLeak_d7xWJOjy6zu1v7Re = new unsigned short[32] { 0xadde, 0xefbe }; \
            if (Log::Console::ready(Log::Level::Warning)) { \
                Log::Console::write( \
                    Log::Level::Warning, \
                    std::format( \
                        L"I'll put {:x}{:x} {:x}{:x} here (test message indicating that a leak has taken place)", \
                        memoryLeak_d7xWJOjy6zu1v7Re[0] & 0xff, memoryLeak_d7xWJOjy6zu1v7Re[0] >> 8, \
                        memoryLeak_d7xWJOjy6zu1v7Re[1] & 0xff, memoryLeak_d7xWJOjy6zu1v7Re[1] >> 8 \
                    ) \
                ); \
                Log::Console::write( \
                    Log::Level::Warning, \
                    std::format( \
                        L"Address of leaked memory block: 0x{:016X}", \
                        reinterpret_cast<uintptr_t>(memoryLeak_d7xWJOjy6zu1v7Re) \
                    ) \
                ); \
            } \
        } while (false)
#else
#   define FORCE_MEMORY_LEAK do {} while (false)
#endif

#define MEMORY_PROFILING_FLAG_KEY "!debug"

#if WITH_ASAN
#   define MEMORY_PROFILING_FLAG_VALUE "Memory profiling enabled (AddressSanitizer)"
#elif WITH_CRTDBG
#   define _CRTDBG_MAP_ALLOC // NOLINT(*-reserved-identifier)
#   include <lib/winapi.h>
#   include <crtdbg.h>
#   include <cstdlib>
#   include <format>
#   define MEMORY_PROFILING_FLAG_VALUE "Memory profiling enabled (CRT Debug)"
#else
#   define MEMORY_PROFILING_FLAG_VALUE ""
#endif

namespace Config {
    [[maybe_unused]]
    inline void initProfiler() {
#if WITH_ASAN
        Log::Console::write(Log::Level::Warning, L"Memory profiling enabled (AddressSanitizer)");
        Log::Console::write(Log::Level::Error, L"!! Unimplemented !!");
        __debugbreak();
#elif WITH_CRTDBG
        constexpr auto reportMode = /*_CRTDBG_MODE_DEBUG |*/ _CRTDBG_MODE_FILE /*| _CRTDBG_MODE_WNDW*/;
        ::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF /*| _CRTDBG_CHECK_CRT_DF*/ | _CRTDBG_LEAK_CHECK_DF);
        ::_CrtSetReportMode(_CRT_ASSERT, reportMode);
        ::_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
        ::_CrtSetReportMode(_CRT_WARN, reportMode);
        ::_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
        ::_CrtSetReportMode(_CRT_ERROR, reportMode);
        ::_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
        Log::Console::write(Log::Level::Warning, L"Memory profiling enabled (CRT Debug)");
#endif
    }
}
