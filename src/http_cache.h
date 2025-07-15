// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "http.h"
#include <optional>

namespace Http::Cache {
    using Key = std::string;

    struct Entry {
        std::shared_ptr<ResponseData> m_data;
        DateTime::Point m_cachedAt;
        DateTime::Point m_expiredAt;
        Status m_status;
    };

    [[nodiscard, maybe_unused]]
    inline DateTime::Point expiresAfter(DateTime::OffsetValue seconds) {
        return DateTime::Clock::now() + DateTime::Offset(seconds);
    }

    [[maybe_unused]] void store(const Key &, const Entry &);
    [[maybe_unused]] void store(const Key &, Entry &&);
    [[maybe_unused]] void store(const Key &, DateTime::Point, Status, std::shared_ptr<ResponseData>);
    [[nodiscard, maybe_unused]] std::optional<Entry> load(const Key &);
    [[maybe_unused]] void maintain();
}
