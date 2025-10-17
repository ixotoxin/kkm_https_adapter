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

#ifdef WITH_LEAKS
#   if !WITH_ASAN && !WITH_CRTDBG
#       undef WITH_LEAKS
#       define WITH_LEAKS 0
#   endif
#else
#   define WITH_LEAKS 0
#endif

#if (WITH_ASAN || WITH_CRTDBG) && !defined(DEBUG) && !defined(_DEBUG)
#   error Memory profiling allowed only in debug version
#endif

#undef FORCE_MEMORY_LEAK

#if WITH_LEAKS
#   include <log/core.h>
#   define FORCE_MEMORY_LEAK \
        do { \
            auto _memoryLeak_ = new unsigned short[32] { 0xadde, 0xefbe }; \
            if (Log::Console::ready(Log::Level::Warning)) { \
                Log::Console::write( \
                    Log::Level::Warning, \
                    std::format( \
                        L"I'll put {:x}{:x} {:x}{:x} here (test message indicating that a leak has taken place)", \
                        _memoryLeak_[0] & 0xff, _memoryLeak_[0] >> 8, _memoryLeak_[1] & 0xff, _memoryLeak_[1] >> 8 \
                    ) \
                ); \
                Log::Console::write( \
                    Log::Level::Warning, \
                    std::format( \
                        L"Address of leaked memory block: 0x{:016X}", \
                        reinterpret_cast<uintptr_t>(_memoryLeak_) \
                    ) \
                ); \
            } \
        } while (false)
#else
#   define FORCE_MEMORY_LEAK do {} while (false)
#endif

#undef START_MEMORY_PROFILING

#if WITH_ASAN
#   define START_MEMORY_PROFILING Log::Console::write(Log::Level::Warning, L"Memory profiling enabled (AddressSanitizer)");
#elif WITH_CRTDBG
#   define _CRTDBG_MAP_ALLOC // NOLINT(*-reserved-identifier)
#   include <lib/winapi.h>
#   include <crtdbg.h>
#   include <cstdlib>
#   include <format>
#   define START_MEMORY_PROFILING Log::Console::write(Log::Level::Warning, L"Memory profiling enabled (CRT Debug)");
namespace Init {
    EXECUTE_BEFORE_MAIN(startMemoryProfiling) {
        constexpr auto _reportMode_ = /*_CRTDBG_MODE_DEBUG |*/ _CRTDBG_MODE_FILE /*| _CRTDBG_MODE_WNDW*/;
        ::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF /*| _CRTDBG_CHECK_CRT_DF*/ | _CRTDBG_LEAK_CHECK_DF);
        ::_CrtSetReportMode(_CRT_ASSERT, _reportMode_);
        ::_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
        ::_CrtSetReportMode(_CRT_WARN, _reportMode_);
        ::_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
        ::_CrtSetReportMode(_CRT_ERROR, _reportMode_);
        ::_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
    }
}
#else
#   define START_MEMORY_PROFILING do {} while (false)
#endif
