// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "server_proto_handler.h"

namespace Server::Config {
    class Handler : public ProtoHandler {
    public:
        Handler() = default;
        Handler(const Handler &) = default;
        Handler(Handler &&) = default;
        ~Handler() override = default;

        Handler & operator=(const Handler &) = default;
        Handler & operator=(Handler &&) = default;

        [[nodiscard]] bool asyncReady() const noexcept override;
        void operator()(Http::Request &) const noexcept override;
    };
}
