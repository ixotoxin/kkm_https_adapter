// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <string_view>

namespace KkmJsonLoader {
#if !BUILD_SEPARATED
    namespace Wcs {
        using Csv = const std::wstring_view;

        constexpr Csv c_usage1 {
            L"    exec {сн} {вф}      Выполнить операцию описанную в файле (json)\n"
        };

        constexpr Csv c_usage2 {
            L"    {вф}                Входной файл в JSON-формате\n"
            L"    (json)              Вывод в JSON-формате\n"
        };
    }
#endif

    namespace Mbs {
        using Csv = const std::string_view;

        constexpr Csv c_id { "!id" };
        constexpr Csv c_query { "!query" };
    }
}
