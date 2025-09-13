// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "macro.h"
#include "winapi.h"
#include <clocale>

namespace Basic {
    EXECUTE_BEFORE_MAIN(setup_console_output) {
        ::SetConsoleOutputCP(CP_UTF8);
        ::SetConsoleCP(CP_UTF8);
        ::setlocale(LC_ALL, ".UTF8");
    }
}
