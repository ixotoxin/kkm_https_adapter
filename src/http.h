// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <lib/basic.h>
#include <unordered_map>
#include "defaults.h"

namespace Http {
    namespace Wcs {
        using Basic::Wcs::c_somethingWrong;
    }

    namespace Mbs {
        using Basic::Mbs::c_ok;
        using Basic::Mbs::c_somethingWrong;
        constexpr std::string_view c_jsonMimeType { "application/json" };
    }

    using RequestHeader = std::unordered_map<std::string, std::string>;

    enum class Method { NotImplemented, Get, Post };

    enum class Status {
        Ok = 200,
        MovedTemporarily = 302,
        BadRequest = 400,
        Unauthorized,
        Forbidden = 403,
        NotFound,
        MethodNotAllowed,
        ImATeapot = 418,
        InternalServerError = 500,
        NotImplemented,
        ServiceUnavailable = 503,
        UnknownError = 520,
    };

    namespace Mbs {
        inline const std::unordered_map<Status, std::string> c_statusStrings {
            { Status::Ok, Mbs::c_ok },
            { Status::MovedTemporarily, "Moved Temporarily" },
            { Status::BadRequest, "Bad Request" },
            { Status::Unauthorized, "Unauthorized" },
            { Status::Forbidden, "Forbidden" },
            { Status::NotFound, "Not Found" },
            { Status::MethodNotAllowed, "Method Not Allowed" },
            { Status::ImATeapot, "I’m a teapot" },
            { Status::InternalServerError, "Internal Server Error" },
            { Status::NotImplemented, "Not Implemented" },
            { Status::ServiceUnavailable, "Service Unavailable" },
            { Status::UnknownError, "Unknown Error" },
        };
    }

    inline std::string s_secret { c_defSecret };
    inline bool s_loopbackWithoutSecret { c_loopbackWithoutSecret };
}
