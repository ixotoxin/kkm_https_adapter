// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "varop.h"
#include "variables.h"
#include <cmake/variables.h>
#include <iostream>

namespace Main {
    std::wostream & vars(std::wostream & stream) {
        stream
            << L"BLD: main.version = \"" << BUILD_VERSION << L"\"\n"
            L"BLD: main.buildTimestamp = \"" << BUILD_TIMESTAMP << L"\"\n"
            L"BLD: main.buildType = \"" << BUILD_TYPE << L"\"\n"
            L"RTM: main.binaryFile = \"" << Main::s_file.c_str() << L"\"\n"
            L"RTM: main.workDirectory = \"" << Main::s_directory.c_str() << L"\"\n";

        return stream;
    }
}
