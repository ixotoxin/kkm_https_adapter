// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "service_varop.h"
#include "service_defaults.h"

namespace Service {
    std::wostream & vars(std::wostream & stream) {
        stream
            << L"DEF: service.systemName = \"" << c_systemName << L"\"\n"
            L"DEF: service.displayName = \"" << c_displayName << L"\"\n";

        return stream;
    }
}
