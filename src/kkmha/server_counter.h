// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

// #include "server_variables.h"
#include <cassert>
#include <atomic>

namespace Server {
    class Counter {
        inline static std::atomic<int64_t> s_value { 0 };

    public:
        Counter() { ++s_value; }
        Counter(const Counter &) = delete;
        Counter(Counter &&) = delete;
        ~Counter() { --s_value; assert(s_value >= 0); }

        Counter & operator=(const Counter &) = delete;
        Counter & operator=(Counter &&) = delete;

        [[nodiscard]] static int64_t value() noexcept { return s_value; };
    };
}
