// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "macro.h"
#include <lib/winapi.h>
#include <string_view>

namespace Log {
    LOG_DEF(bool, c_outputTimestamp, false);
    LOG_DEF(bool, c_outputLevel, true);
    LOG_CONST(std::wstring_view, c_directoryEnv, L"kkmha_logs_dir");
    LOG_CONST(std::wstring_view, c_directory, L"logs");
    LOG_CONST(std::wstring_view, c_logFileFormat, L"kkmha-{:04d}-{:02d}.log");
    LOG_CONST(wchar_t *, c_eventSource, L"KKM HTTPS Adapter");
    LOG_CONST(::DWORD, c_eventId, 0);
    LOG_CONST(::WORD, c_eventCategory, 0);
}
