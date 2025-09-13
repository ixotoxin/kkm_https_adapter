// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "http_types.h"
#include "http_proto_response.h"
#include <lib/datetime.h>
#include <memory>

namespace Server::Cache {
    using Key = std::string;

    struct Entry {
        std::shared_ptr<Http::ProtoResponse> m_data;
        DateTime::Point m_cachedAt;
        DateTime::Point m_expiredAt;
        Http::Status m_status;
    };
}
