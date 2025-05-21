// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <filesystem>

namespace Config {
    inline std::filesystem::path s_binaryFile {};
    inline std::filesystem::path s_workDirectory {};
    inline std::filesystem::path s_configDirectory {};
    inline std::filesystem::path s_configFile {};

    void read(wchar_t **);
}
