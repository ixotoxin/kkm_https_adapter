// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "http_types.h"
#include "http_proto_response.h"
#include <utility>
#include <ostream>

namespace Http {
    struct SolidResponse : public ProtoResponse {
        std::string m_data;

        SolidResponse() = default;

        [[maybe_unused]]
        explicit SolidResponse(const std::string_view data)
        : ProtoResponse(), m_data { data } {}

        [[maybe_unused]]
        explicit SolidResponse(std::string && data)
        : ProtoResponse(), m_data { std::forward<std::string>(data) } {}

        SolidResponse(const SolidResponse &) = delete;
        SolidResponse(SolidResponse &&) = delete;
        ~SolidResponse() override = default;

        SolidResponse & operator=(const SolidResponse &) = delete;
        SolidResponse & operator=(SolidResponse &&) = delete;

        explicit operator bool() override {
            return !m_data.empty();
        }

        void render(Asio::StreamBuffer & buffer, Status) override {
            std::ostream output { &buffer };
            output << m_data;
        }
    };
}
