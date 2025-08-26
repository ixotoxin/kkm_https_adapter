// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "basic.h"

#ifndef ENABLE_MEMORY_PROFILING
#   define ENABLE_MEMORY_PROFILING 0
#endif

#ifndef ENABLE_MEMORY_LEAKS
#   define ENABLE_MEMORY_LEAKS 0
#endif

#if ENABLE_MEMORY_PROFILING
#   ifndef DEBUG
#       define DEBUG
#   endif
#   ifndef _DEBUG
#       define _DEBUG
#   endif
#   define _CRTDBG_MAP_ALLOC // NOLINT(*-reserved-identifier)

#   include <crtdbg.h>
#   include <cstdlib>

#   if ENABLE_MEMORY_LEAKS
#       define FORCE_MEMORY_LEAK \
            do { \
                auto _memoryLeak_ = new unsigned short[32] { 0xadde, 0xefbe }; \
                Log::Console::write( \
                    Log::Level::Warning, \
                    std::format( \
                        L"I'll put {:x}{:x} {:x}{:x} here (test message indicating that a leak has taken place)", \
                        _memoryLeak_[0] & 0xff, _memoryLeak_[0] >> 8, _memoryLeak_[1] & 0xff, _memoryLeak_[1] >> 8 \
                    ) \
                ); \
                Log::Console::write( \
                    Log::Level::Warning, \
                    std::format(L"Address of leaked memory block: 0x{:016X}", reinterpret_cast<uintptr_t>(_memoryLeak_)) \
                ); \
            } while (false)
#   else
#       define FORCE_MEMORY_LEAK
#   endif

#   define START_MEMORY_PROFILING Log::Console::write(Log::Level::Warning, L"Memory profiling enabled");

namespace Basic {
    EXECUTE_BEFORE_MAIN(start_memory_profiling) {
        constexpr auto _reportMode_ = /*_CRTDBG_MODE_DEBUG |*/ _CRTDBG_MODE_FILE /*| _CRTDBG_MODE_WNDW*/;
        ::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_LEAK_CHECK_DF);
        ::_CrtSetReportMode(_CRT_ASSERT, _reportMode_);
        ::_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
        ::_CrtSetReportMode(_CRT_WARN, _reportMode_);
        ::_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
        ::_CrtSetReportMode(_CRT_ERROR, _reportMode_);
        ::_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
    }
}
#endif

#ifndef START_MEMORY_PROFILING
#   define START_MEMORY_PROFILING do {} while (false)
#endif

#ifndef FORCE_MEMORY_LEAK
#   define FORCE_MEMORY_LEAK do {} while (false)
#endif
