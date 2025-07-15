// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "http.h"

namespace Http {
    class DefaultHandler : public RequestHandler {
    public:
        DefaultHandler() = default;
        DefaultHandler(const DefaultHandler &) = default;
        DefaultHandler(DefaultHandler &&) = default;
        ~DefaultHandler() override = default;

        DefaultHandler & operator=(const DefaultHandler &) = default;
        DefaultHandler & operator=(DefaultHandler &&) = default;

        [[nodiscard]] bool asyncReady() const noexcept override;
        // [[nodiscard]] bool isCaching() const noexcept override;
        void operator()(Request &) const noexcept override;
    };
}
