// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "server_proto_handler.h"
#include "http_constant_response.h"
#include <memory>
#include <string_view>

namespace Server::Ping {
    using namespace std::string_view_literals;

    class Handler : public ProtoHandler {
        inline static auto s_pongResponse = std::make_shared<Http::ConstantResponse>(
            "HTTP/1.1 200 OK\r\n"
            "Connection: close\r\n"
            "Pragma: no-cache\r\n"
            "Cache-Control: no-cache, private\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: 35\r\n\r\n"
            "{\"!message\":\"Pong\",\"!success\":true}"sv
        );

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
