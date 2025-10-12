// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <string>
#include <unordered_map>

namespace Http {
    using Header = std::unordered_map<std::string, std::string>;

    enum class Method { NotImplemented, Get, Post };

    enum class Status {
        Ok = 200,
        MovedTemporarily = 302,
        BadRequest = 400,
        // Unauthorized,
        Forbidden = 403,
        NotFound,
        MethodNotAllowed,
        // RequestTimeout = 408,
        // ImATeapot = 418,
        InternalServerError = 500,
        NotImplemented,
        // ServiceUnavailable = 503,
        // UnknownError = 520,
    };
}
