// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "http_types.h"
#include "http_proto_response.h"
#include <utility>
#include <memory>
#include <string_view>
#include <ostream>

namespace Http {
    using namespace std::string_view_literals;

    struct ConstantResponse : public ProtoResponse {
        inline static auto s_okResponse = std::make_shared<Http::ConstantResponse>(
            "HTTP/1.1 200 OK\r\n"
            "Connection: close\r\n"
            "Pragma: no-cache\r\n"
            "Cache-Control: no-cache, private\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: 33\r\n\r\n"
            "{\"!message\":\"OK\",\"!success\":true}"sv
        );

        const std::string_view m_data;

        ConstantResponse() = delete;

        [[maybe_unused]]
        explicit constexpr ConstantResponse(const std::string_view data)
        : ProtoResponse(), m_data { data } {}

        ConstantResponse(const ConstantResponse &) = delete;
        ConstantResponse(ConstantResponse &&) = delete;
        ~ConstantResponse() override = default;

        ConstantResponse & operator=(const ConstantResponse &) = delete;
        ConstantResponse & operator=(ConstantResponse &&) = delete;

        explicit operator bool() override {
            return !m_data.empty();
        }

        void render(Asio::StreamBuffer & buffer, Status) override {
            std::ostream output { &buffer };
            output << m_data;
        }
    };
}
