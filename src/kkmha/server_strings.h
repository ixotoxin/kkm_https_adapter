// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "http_strings.h"

namespace Server::Wcs {
    using Csv = const std::wstring_view;

    using Http::Wcs::c_prefixedText;

    constexpr Csv c_starting { L"Запуск сервера..." };
    constexpr Csv c_startingFailed { L"Не удалось запустить сервер" };
    constexpr Csv c_started { L"Сервер запущен" };
    constexpr Csv c_stopping { L"Остановка сервера..." };
    constexpr Csv c_stoppingFailed { L"Не удалось остановить сервер" };
    constexpr Csv c_stopped { L"Сервер остановлен" };
    constexpr Csv c_servicingFailed { L"Сбой сервера" };
    constexpr Csv c_socketOpeningError { L"Ошибка открытия сокета" };
    constexpr Csv c_maximumIsExceeded { L"Превышено количество одновременных запросов" };
    constexpr Csv c_processingSuccess { L"Запрос успешно обработан" };
    constexpr Csv c_processingFailed { L"Не удалось обработать запрос" };
    constexpr Csv c_cacheMaintain { L"Обслуживание кэша (размер {} => {})" };
}
