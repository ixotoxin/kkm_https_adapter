// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "macro.h"
#include <filesystem>

namespace Main {
    CONFIG_VAR(std::filesystem::path, s_file, );
    CONFIG_VAR(std::filesystem::path, s_directory, );
}

namespace Config {
    CONFIG_VAR(std::filesystem::path, s_directory, );
    CONFIG_VAR(std::filesystem::path, s_file, );
}
