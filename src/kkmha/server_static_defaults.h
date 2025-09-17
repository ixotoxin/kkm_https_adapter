// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <lib/datetime.h>
#include <string_view>

namespace Server::Static {
    using namespace std::chrono_literals;

    constexpr const DateTime::Offset c_fileCacheLifeTime { 43'200s }; // Секунды
    constexpr const bool c_enableStatic { false };
    constexpr const std::wstring_view c_staticDirectory { L"static" };
    constexpr const std::string_view c_indexFile { "index.html" };
    constexpr const std::wstring_view c_mimeMapFile { L"conf\\mime.json" };
    constexpr const bool c_enableUnknownType { false };
    constexpr const std::string_view c_defMimeType { "application/octet-stream" };
    constexpr const size_t c_fileSizeLimit { 2'097'152 }; // Байты
}
