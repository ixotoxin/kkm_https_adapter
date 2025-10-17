// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "server_cache_types.h"
#include <optional>

namespace Server::Cache {
    [[nodiscard, maybe_unused]]
    inline DateTime::Point expiresAfter(const DateTime::Offset seconds) {
        return DateTime::Clock::now() + seconds;
    }

    [[maybe_unused]] void store(const Key &, const Entry &);
    [[maybe_unused]] void store(const Key &, Entry &&);
    [[maybe_unused]] void store(const Key &, DateTime::Point, Http::Status, std::shared_ptr<Http::ProtoResponse>);
    [[nodiscard, maybe_unused]] std::optional<Entry> load(const Key &);
    [[maybe_unused]] void maintain();
}
