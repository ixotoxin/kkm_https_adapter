// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <string_view>

namespace Server::Cache::Wcs {
    using Csv = const std::wstring_view;

    constexpr Csv c_fromCache { L"Запрос [{:04x}]: Данные взяты из кеша" };
}
