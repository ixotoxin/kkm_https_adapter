// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "server.h"
#include <cassert>

namespace Server {
    class Counter {
        inline static std::atomic<int64_t> s_counter { 0 };

    public:
        Counter() {
            assert(s_counter >= 0);
            ++s_counter;
        }

        ~Counter() {
            --s_counter;
            assert(s_counter >= 0);
        }

        Counter(const Counter &) = delete;
        Counter(Counter &&) = default;

        Counter & operator=(const Counter &) = delete;
        Counter & operator=(Counter &&) = delete;

        [[nodiscard]]
        bool invalid() { // NOLINT(*-convert-member-functions-to-static)
            assert(s_counter >= 0);
            return s_counter > s_concurrencyLimit;
        }

        [[nodiscard]]
        static int64_t value() {
            assert(s_counter >= 0);
            return s_counter;
        }
    };
}
