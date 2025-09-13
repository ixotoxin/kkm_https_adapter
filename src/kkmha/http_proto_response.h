// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "http_types.h"
#include "asio.h"

namespace Http {
    struct ProtoResponse {
        ProtoResponse() = default;
        ProtoResponse(const ProtoResponse &) = default;
        ProtoResponse(ProtoResponse &&) = default;
        virtual ~ProtoResponse() = default;

        ProtoResponse & operator=(const ProtoResponse &) = default;
        ProtoResponse & operator=(ProtoResponse &&) = default;

        virtual explicit operator bool() = 0;

        virtual void render(Asio::StreamBuffer &, Status) = 0;
    };
}
