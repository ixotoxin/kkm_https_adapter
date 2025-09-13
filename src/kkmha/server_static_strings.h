// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <string_view>

namespace Server::Static {
    namespace Wcs {
        using Csv = const std::wstring_view;

        constexpr Csv c_incorrectStructure { L"Конфигурационный файл '{}' содержит ошибки" };
    }

    namespace Mbs {
        using Csv = const std::string_view;

        constexpr Csv c_fileTooLarge { "Размер файла превышает разрешённый" };
        constexpr Csv c_unknownMimeType { "Неизвестный тип файла" };
        constexpr Csv c_badFilesystemEntityType { "Неподходящий тип объекта файловой системы" };

        constexpr Csv c_redirectResponseTemplate {
            "HTTP/1.1 302 Moved Temporarily\r\n"
            "Connection: close\r\n"
            "Location: {}\r\n"
            "Pragma: no-cache\r\n"
            "Cache-Control: no-cache, private\r\n"
            "Content-Length: 0\r\n"
            "\r\n"
        };
    }
}
