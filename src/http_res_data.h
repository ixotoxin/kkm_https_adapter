// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <lib/json.h>
#include "http.h"
#include "asio.h"

namespace Http {
    namespace Mbs {
        using Json::Mbs::c_successKey;
        using Json::Mbs::c_messageKey;
    }

    struct ResponseData {
        ResponseData() = default;
        ResponseData(const ResponseData &) = default;
        ResponseData(ResponseData &&) = default;
        virtual ~ResponseData() = default;

        ResponseData & operator=(const ResponseData &) = default;
        ResponseData & operator=(ResponseData &&) = default;

        virtual explicit operator bool() = 0;

        virtual void render(Asio::StreamBuffer &, Status) = 0;
    };
}
