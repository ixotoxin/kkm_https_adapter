// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "http_request.h"

namespace Server {
    class ProtoHandler {
    public:
        ProtoHandler() = default;
        ProtoHandler(const ProtoHandler &) = default;
        ProtoHandler(ProtoHandler &&) = default;
        virtual ~ProtoHandler() = default;

        ProtoHandler & operator=(const ProtoHandler &) = default;
        ProtoHandler & operator=(ProtoHandler &&) = default;

        [[nodiscard]] virtual bool asyncReady() const noexcept = 0;
        virtual void operator()(Http::Request &) const noexcept = 0;
    };
}
