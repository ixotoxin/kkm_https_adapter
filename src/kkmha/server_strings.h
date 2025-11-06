// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <string_view>

namespace Server {
    namespace Wcs {
        using Csv = const std::wstring_view;

        constexpr Csv c_prefixedText { L"Запрос [{:04x}]: {}" };
        constexpr Csv c_forbidden { L"Запрос [{:04x}]: Сбой авторизации" };
        constexpr Csv c_redirectToStatic { L"Запрос [{:04x}]: Перенаправляем в /static/" };

        constexpr Csv c_starting { L"Запуск сервера..." };
        constexpr Csv c_startingFailed { L"Не удалось запустить сервер" };
        constexpr Csv c_started { L"Сервер запущен" };
        constexpr Csv c_stopping { L"Остановка сервера..." };
        constexpr Csv c_stoppingFailed { L"Не удалось остановить сервер" };
        constexpr Csv c_stopped { L"Сервер остановлен" };
        constexpr Csv c_servicingFailed { L"Сбой сервера" };
        constexpr Csv c_socketOpeningError { L"Ошибка открытия сокета" };
        constexpr Csv c_maximumIsExceeded { L"Превышено разрешенное количество одновременных запросов" };
        constexpr Csv c_timeoutExpired { L"Превышена разрешенная длительность выполнения запроса" };
        constexpr Csv c_processingSuccess { L"Запрос успешно обработан" };
        constexpr Csv c_processingFailed { L"Не удалось обработать запрос" };
        constexpr Csv c_cacheMaintain { L"Обслуживание кэша (размер {} => {})" };
    }

    namespace Mbs {
        using Csv = const std::string_view;

        constexpr Csv c_prefixedText { "Запрос [{:04x}]: {}" };
        constexpr Csv c_prefixedTextWithSource { "Запрос [{:04x}]: {} ({})" };
        constexpr Csv c_prefixedOperation { "Запрос [{:04x}]: {}: {}" };
        constexpr Csv c_prefixedOperationWithSource { "Запрос [{:04x}]: {}: {} ({})" };

        constexpr Csv c_badRequest { "Некорректный запрос" };
        constexpr Csv c_methodNotAllowed { "Невозможно выполнить данный запрос" };
        constexpr Csv c_unknownMethod { "Неизвестный метод" };
        constexpr Csv c_notImplemented { "Не реализовано" };
        constexpr Csv c_forbidden { "Сбой авторизации" };
        constexpr Csv c_badPath { "Некорректный путь" };
        constexpr Csv c_notFound { "Документ не найден" };
        constexpr Csv c_invalidContentType { "Заголовок 'Content-Type' отсутствует или некорректен" };
        constexpr Csv c_invalidXSecret { "Заголовок 'X-Secret' отсутствует или некорректен" };
        constexpr Csv c_invalidXIdempotencyKey { "Заголовок 'X-Idempotency-Key' отсутствует или некорректен" };
        constexpr Csv c_connectionWith { "Установлено соединение с [{}]" };
        constexpr Csv c_requestedMethod { "От клиента [{}] поступил запрос [{} {}]" };

        constexpr Csv c_connectionAcceptStatus { "Connection accept: {}" };
        constexpr Csv c_acceptorCancelStatus { "Acceptor cancel: {}" };
        constexpr Csv c_acceptorCloseStatus { "Acceptor close: {}" };
        constexpr Csv c_sslHandshakeOperation { "SSL handshake" };
        constexpr Csv c_sslReadOperation { "SSL read" };
        constexpr Csv c_sslWriteOperation { "SSL write" };
        constexpr Csv c_sslShutdownOperation { "SSL shutdown" };
        constexpr Csv c_socketCloseOperation { "Socket close" };
    }
}
