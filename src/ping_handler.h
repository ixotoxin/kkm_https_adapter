// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "http.h"

namespace Http {
    class PingHandler : public RequestHandler {
    public:
        PingHandler() = default;
        PingHandler(const PingHandler &) = default;
        PingHandler(PingHandler &&) = default;
        ~PingHandler() override = default;

        PingHandler & operator=(const PingHandler &) = default;
        PingHandler & operator=(PingHandler &&) = default;

        [[nodiscard]] bool asyncReady() const noexcept override;
        void operator()(Request &) const noexcept override;
    };
}
