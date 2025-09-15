// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "macro.h"
#include <string_view>

namespace Basic {
    namespace Wcs {
        LIB_WCSTR(c_ok, L"OK");
        LIB_WCSTR(c_somethingWrong, L"Что-то пошло не так!");
        LIB_WCSTR(c_fallbackErrorMessage, L"[ error ]");
        LIB_WSTR(c_inFile, L"в файле");
    }

    namespace Mbs {
        LIB_CSTR(c_ok, "OK");
        LIB_CSTR(c_somethingWrong, "Что-то пошло не так!");
        LIB_CSTR(c_fallbackErrorMessage, "[ error ]");
        LIB_STR(c_inFile, "в файле");
    }
}
