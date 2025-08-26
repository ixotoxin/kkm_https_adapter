// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "basic.h"

namespace Basic {
    EXECUTE_BEFORE_MAIN(setup_console_output) {
        ::SetConsoleOutputCP(CP_ACP);
        ::SetConsoleCP(CP_ACP);
        ::setlocale(LC_ALL, ".ACP");
    }
}
