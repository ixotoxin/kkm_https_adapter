// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <string_view>

namespace KkmJsonLoader {
    namespace Wcs {
        using Csv = const std::wstring_view;

        constexpr Csv c_usage1 {
    #if !BUILD_SEPARATED
            L"    exec <сн> <file>    Выполнить операцию описанную в файле (json)\n"
    #endif
        };

        constexpr Csv c_usage2 {
    #if !BUILD_SEPARATED
            L"    <file>              Входной файл в JSON-формате\n"
            L"    (json)              Вывод в JSON-формате\n"
    #endif
        };
    }

    namespace Mbs {
        using Csv = const std::string_view;

        constexpr Csv c_id { "!id" };
        constexpr Csv c_query { "!query" };
    }
}
