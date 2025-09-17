// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "server_cache_core.h"
#include "server_defaults.h"
#include "server_strings.h"
#include <log/write.h>
#include <utility>
#include <atomic>
#include <mutex>
#include <unordered_map>

namespace Server::Cache {
    static std::unordered_map<std::string, Entry> s_cache {};
    static std::mutex s_cacheMutex {};
    static std::atomic<size_t> s_counter { 0 };

    [[maybe_unused]]
    void store(const Key & key, Entry & entry) {
        std::scoped_lock cacheLock(s_cacheMutex);
        entry.m_cachedAt = DateTime::Clock::now();
        s_cache[key] = entry;
    }

    [[maybe_unused]]
    void store(const Key & key, Entry && entry) {
        std::scoped_lock cacheLock(s_cacheMutex);
        entry.m_cachedAt = DateTime::Clock::now();
        s_cache[key] = std::forward<Entry>(entry);
    }

    [[maybe_unused]]
    void store(
        const Key & key,
        DateTime::Point expiredAt,
        Http::Status status,
        std::shared_ptr<Http::ProtoResponse> data
    ) {
        std::scoped_lock cacheLock(s_cacheMutex);
        s_cache[key] = {
            .m_data = std::forward<std::shared_ptr<Http::ProtoResponse>>(data),
            .m_cachedAt = DateTime::Clock::now(),
            .m_expiredAt = expiredAt,
            .m_status = status
        };
    }

    [[nodiscard, maybe_unused]]
    std::optional<Entry> load(const Key & key) {
        std::scoped_lock cacheLock(s_cacheMutex);
        auto it = s_cache.find(key);
        if (it == s_cache.end()) {
            return std::nullopt;
        }
        return it->second;
    }

    [[maybe_unused]]
    void maintain() {
        if (++s_counter >= c_cacheCleanUpThreshold) {
            std::scoped_lock cacheLock(s_cacheMutex);
            auto oldSize = s_cache.size();
            std::erase_if(
                s_cache,
                [] (const auto & item) { return item.second.m_expiredAt < DateTime::Clock::now(); }
            );
            tsLogDebug(Wcs::c_cacheMaintain, oldSize, s_cache.size());
            s_counter = 0;
        }
    }
}
