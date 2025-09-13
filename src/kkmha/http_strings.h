// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "http_types.h"
#include <lib/strings.h>
#include <string>
#include <unordered_map>

namespace Http {
    namespace Wcs {
        using Csv = const std::wstring_view;

        constexpr Csv c_requestPrefix { L"Запрос [{:04x}]: " };
        constexpr Csv c_prefixedText { L"Запрос [{:04x}]: {}" };
        constexpr Csv c_forbidden { L"Запрос [{:04x}]: Сбой авторизации" };
        constexpr Csv c_fromCache { L"Запрос [{:04x}]: Данные взяты из кеша" };
        constexpr Csv c_redirectToStatic { L"Запрос [{:04x}]: Перенаправляем в /static/" };
        constexpr Csv c_bodySizeLimitExceeded { L"Запрос [{:04x}]: Превышен допустимый размер тела запроса" };
    }

    namespace Mbs {
        using Csv = const std::string_view;

        constexpr Csv c_requestPrefix { "Запрос [{:04x}]: " };
        constexpr Csv c_prefixedText { "Запрос [{:04x}]: {}" };
        constexpr Csv c_badRequest { "Некорректный запрос" };
        constexpr Csv c_methodNotAllowed { "Невозможно выполнить данный запрос" };
        constexpr Csv c_unknownMethod { "Неизвестный метод" };
        constexpr Csv c_notImplemented { "Не реализовано" };
        constexpr Csv c_forbidden { "Сбой авторизации" };
        constexpr Csv c_badPath { "Некорректный путь" };
        constexpr Csv c_notFound { "Документ не найден" };
        constexpr Csv c_bodySizeLimitExceeded { "Превышен допустимый размер тела запроса" };
        constexpr Csv c_invalidContentType { "Заголовок 'Content-Type' отсутствует или некорректен" };
        constexpr Csv c_invalidXSecret { "Заголовок 'X-Secret' отсутствует или некорректен" };
        constexpr Csv c_invalidXIdempotencyKey { "Заголовок 'X-Idempotency-Key' отсутствует или некорректен" };
        constexpr Csv c_requestedMethod { "От клиента [{}] поступил запрос [{} {}]" };

        constexpr Csv c_jsonMimeType { "application/json" };

        constexpr Csv c_responseHeaderTemplate {
            "HTTP/1.1 {} {}\r\n"
            "Connection: close\r\n"
            "Pragma: no-cache\r\n"
            "Cache-Control: no-cache, private\r\n"
            "Content-Type: {}\r\n"
            "Content-Length: {}\r\n"
            "\r\n"
        };

        constexpr Csv c_staticResponseHeaderTemplate {
            "HTTP/1.1 {} {}\r\n"
            "Connection: close\r\n"
            "Content-Type: {}\r\n"
            "Content-Length: {}\r\n"
            "\r\n"
        };

        inline const std::unordered_map<Status, std::string> c_statusStrings {
            { Status::Ok, Basic::Mbs::c_ok },
            { Status::MovedTemporarily, "Moved Temporarily" },
            { Status::BadRequest, "Bad Request" },
            // { Status::Unauthorized, "Unauthorized" },
            { Status::Forbidden, "Forbidden" },
            { Status::NotFound, "Not Found" },
            { Status::MethodNotAllowed, "Method Not Allowed" },
            // { Status::ImATeapot, "I’m a teapot" },
            { Status::InternalServerError, "Internal Server Error" },
            { Status::NotImplemented, "Not Implemented" },
            // { Status::ServiceUnavailable, "Service Unavailable" },
            // { Status::UnknownError, "Unknown Error" },
        };
    }
}
