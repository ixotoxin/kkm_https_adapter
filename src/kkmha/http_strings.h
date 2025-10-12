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

        constexpr Csv c_bodySizeLimitExceeded { L"Запрос [{:04x}]: Превышен допустимый размер тела запроса" };
    }

    namespace Mbs {
        using Csv = const std::string_view;

        constexpr Csv c_bodySizeLimitExceeded { "Превышен допустимый размер тела запроса" };
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
            // { Status::RequestTimeout, "Request Timeout" },
            // { Status::ImATeapot, "I’m a teapot" },
            { Status::InternalServerError, "Internal Server Error" },
            { Status::NotImplemented, "Not Implemented" },
            // { Status::ServiceUnavailable, "Service Unavailable" },
            // { Status::UnknownError, "Unknown Error" },
        };
    }
}
