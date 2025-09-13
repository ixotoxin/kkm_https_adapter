// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include <log/variables.h>
#include <string_view>

namespace Log {
    const std::wstring_view c_logFileFormat { L"kkmjl-{:04d}-{:02d}.log" };
    const wchar_t * c_eventSource { L"KKMJL" };
}
