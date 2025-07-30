// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "http_req_handler.h"

namespace Kkm {
    class HttpHandler : public Http::RequestHandler {
    public:
        HttpHandler() = default;
        HttpHandler(const HttpHandler &) = default;
        HttpHandler(HttpHandler &&) = default;
        ~HttpHandler() override = default;

        HttpHandler & operator=(const HttpHandler &) = default;
        HttpHandler & operator=(HttpHandler &&) = default;

        [[nodiscard]] bool asyncReady() const noexcept override;
        void operator()(Http::Request &) const noexcept override;
    };
}
