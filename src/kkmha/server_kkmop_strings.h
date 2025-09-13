// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <string_view>

namespace Server::KkmOp {
    namespace Wcs {
        using Csv = const std::wstring_view;

        constexpr Csv c_requestPrefix { L"Запрос [{:04x}]: " };
        constexpr Csv c_selectKkm { L"Запрос [{:04x}]: Выбрана ККМ [{}] (параметры подключения: {})" };
        constexpr Csv c_getKkmInfo { L"Запрос [{:04x}]: ККМ [{}]: Получение информации об устройстве" };
        constexpr Csv c_connParamsSaved { L"Запрос [{:04x}]: Параметры подключения ККМ [{}] успешно сохранены" };
    }

    namespace Mbs {
        using Csv = const std::string_view;

        constexpr Csv c_notFound { "Запрос [{:04x}]: ККМ [{}] не доступна" };
        constexpr Csv c_cantClearRegistry { "Не удалось очистить реестр параметров подключения" };
    }
}
