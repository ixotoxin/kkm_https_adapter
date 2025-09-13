// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "macro.h"
#include "winapi.h"
#include <fcntl.h>
#include <io.h>
#include <cstdio>

namespace Basic {
    EXECUTE_BEFORE_MAIN(setup_console_output) {
        ::_setmode(::_fileno(stdout), _O_U16TEXT);
        ::_setmode(::_fileno(stderr), _O_U16TEXT);
    }
}
