// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "macro.h"
#include <string_view>

namespace Config {
    CONFIG_CONST(std::wstring_view, c_directoryEnv, L"kkmha_conf_dir");
    CONFIG_CONST(std::wstring_view, c_directory, L"conf");
    CONFIG_CONST(std::wstring_view, c_file, L"kkmha.json");
}
