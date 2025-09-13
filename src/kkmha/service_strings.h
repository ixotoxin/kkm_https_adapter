// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <string_view>

namespace Service::Wcs {
    using Csv = const std::wstring_view;

    constexpr Csv c_starting { L"Запуск службы..." };
    constexpr Csv c_startingFailed { L"Не удалось запустить службу" };
    constexpr Csv c_startingTimeout { L"Истекло время ожидания запуска службы" };
    constexpr Csv c_alreadyStarted { L"Служба уже запущена" };
    constexpr Csv c_started { L"Служба запущена" };
    constexpr Csv c_stopping { L"Остановка службы..." };
    constexpr Csv c_stoppingFailed { L"Не удалось остановить службу" };
    constexpr Csv c_stoppingTimeout { L"Истекло время ожидания остановки службы" };
    constexpr Csv c_alreadyStopped { L"Служба уже остановлена" };
    constexpr Csv c_stopped { L"Служба остановлена" };
    constexpr Csv c_installed { L"Служба установлена" };
    constexpr Csv c_uninstalled { L"Служба удалена" };
}
