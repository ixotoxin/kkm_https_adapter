// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "varop.h"
#include "variables.h"
#include <iostream>

namespace Config {
    std::wostream & vars(std::wostream & stream) {
        stream
            << L"RTM: main.configDirectory = \"" << Config::s_directory.native() << L"\"\n"
            L"RTM: main.configFile = \"" << Config::s_file.native() << L"\"\n";

        return stream;
    }
}
