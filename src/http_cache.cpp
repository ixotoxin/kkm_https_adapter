// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "http_cache.h"
#include <mutex>
#include <unordered_map>
#include "log.h"

namespace Http::Cache {
    static std::unordered_map<std::string, Entry> s_cache {};
    static std::mutex s_cacheMutex {};
    static std::atomic<size_t> s_counter { 0 };

    void store(const Key & key, Entry & entry) {
        std::scoped_lock cacheLock(s_cacheMutex);
        entry.m_cachedAt = DateTime::Clock::now();
        s_cache[key] = entry;
    }

    void store(const Key & key, Entry && entry) {
        std::scoped_lock cacheLock(s_cacheMutex);
        entry.m_cachedAt = DateTime::Clock::now();
        s_cache[key] = std::move(entry);
    }

    void store(
        const Key & key,
        DateTime::Point expiredAt,
        Status status,
        std::shared_ptr<ResponseData> data
    ) {
        std::scoped_lock cacheLock(s_cacheMutex);
        s_cache[key] = {
            .m_data = std::forward<std::shared_ptr<ResponseData>>(data),
            .m_cachedAt = DateTime::Clock::now(),
            .m_expiredAt = expiredAt,
            .m_status = status
        };
    }

    std::optional<Entry> load(const Key & key) {
        std::scoped_lock cacheLock(s_cacheMutex);
        auto it = s_cache.find(key);
        if (it == s_cache.end()) {
            return std::nullopt;
        }
        return it->second;
    }

    void maintain() {
        if (++s_counter == c_cacheCleanUpThreshold) {
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
