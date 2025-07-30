// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "http_request.h"

namespace Http {
    class RequestHandler {
    public:
        RequestHandler() = default;
        RequestHandler(const RequestHandler &) = default;
        RequestHandler(RequestHandler &&) = default;
        virtual ~RequestHandler() = default;

        RequestHandler &operator=(const RequestHandler &) = default;
        RequestHandler &operator=(RequestHandler &&) = default;

        [[nodiscard]] virtual bool asyncReady() const noexcept = 0;
        virtual void operator()(Request &) const noexcept = 0;
    };
}
