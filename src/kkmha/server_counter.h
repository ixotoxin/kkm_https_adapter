// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <cassert>
#include <atomic>

namespace Server {
    class Counter {
    public:
        using Type = std::atomic<int64_t>;

        Counter() = delete;
        Counter(const Counter &) = delete;
        Counter(Counter &&) = delete;
        explicit Counter(Type & counter) : s_value { counter } { ++s_value; }
        ~Counter() { --s_value; assert(s_value >= 0); }

        Counter & operator=(const Counter &) = delete;
        Counter & operator=(Counter &&) = delete;

    private:
        Type & s_value;
    };
}
