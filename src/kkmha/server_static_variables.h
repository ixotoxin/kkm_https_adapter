// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "server_static_defaults.h"
#include <lib/json.h>
#include <filesystem>

namespace Server::Static {
    inline bool s_enable { c_enableStatic };
    inline std::filesystem::path s_directory { c_staticDirectory };
    inline std::string s_indexFile { c_indexFile };
    inline std::filesystem::path s_mimeMapFile { c_mimeMapFile };
    inline Nln::Json s_mimeMap(Nln::EmptyJsonObject);
    inline bool s_enableUnknownType { c_enableUnknownType };
}
