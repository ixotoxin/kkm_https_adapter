// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "server_variables.h"
#include <cassert>
#include <atomic>

namespace Server {
    class Counter {
        inline static std::atomic<int64_t> s_counter { 0 };
        bool m_exceeded;

    public:
        Counter() : m_exceeded { s_counter.fetch_add(1, std::memory_order_acquire) >= s_concurrencyLimit } {}

        Counter(const Counter &) = delete;
        Counter(Counter &&) = delete;

        ~Counter() {
            s_counter.fetch_sub(1, std::memory_order_release);
            assert(s_counter >= 0);
        }

        Counter & operator=(const Counter &) = delete;
        Counter & operator=(Counter &&) = delete;

        [[nodiscard]]
        bool exceeded() const { // NOLINT(*-convert-member-functions-to-static)
            return m_exceeded;
        }

        [[nodiscard]]
        static int64_t value() {
            return s_counter.load();
        }
    };
}
