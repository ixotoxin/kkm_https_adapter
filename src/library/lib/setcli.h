// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "macro.h"
#include "winapi.h"
#include <fcntl.h>
#include <io.h>
#include <type_traits>
#include <utility>
#include <cstdio>
#include <clocale>

namespace Config {
    using CodePage = std::pair<::UINT, const char *>;
    constexpr CodePage c_acpOut [[maybe_unused]] { CP_ACP, ".ACP" };
    constexpr CodePage c_utf8Out [[maybe_unused]] { CP_UTF8, ".UTF8" };

    [[maybe_unused]]
    inline void initConsole(CodePage codePage) {
        ::SetConsoleOutputCP(codePage.first);
        ::SetConsoleCP(codePage.first);
        ::setlocale(LC_ALL, codePage.second);
    }

    using OutputMode = int;
    constexpr OutputMode c_u16Text [[maybe_unused]] { _O_U16TEXT };

    [[maybe_unused]]
    inline void initConsole(OutputMode outputMode) {
        ::_setmode(::_fileno(stdout), outputMode);
        ::_setmode(::_fileno(stderr), outputMode);
    }
}
