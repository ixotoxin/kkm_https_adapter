// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "server_defaults.h"
#include <lib/datetime.h>
#include <cassert>
#include <functional>
#include <atomic>
#include <mutex>

namespace Server {
    using namespace std::chrono_literals;

    class Hitman {
        std::mutex m_mutex {};
        std::function<void()> m_gun { [] {} };
        std::atomic<int64_t> m_counter { -1 };

    public:
        Hitman() = default;
        Hitman(const Hitman &) = delete;
        Hitman(Hitman &&) = delete;

        template<typename T>
        requires std::is_convertible_v<T, std::function<void()>>
        [[maybe_unused]]
        explicit Hitman(T && gun)
        : m_gun { gun }, m_counter { 0 } {}

        template<typename T>
        requires std::is_convertible_v<T, std::function<void()>>
        [[maybe_unused]]
        Hitman(T && gun, DateTime::SleepUnit waitingTime)
        : m_gun { gun }, m_counter { waitingTime / c_sleepQuantum } {
            assert(waitingTime >= 0ms);
        }

        ~Hitman() = default;

        Hitman & operator=(const Hitman &) = delete;
        Hitman & operator=(Hitman &&) = delete;

        template<typename T>
        requires std::is_convertible_v<T, std::function<void()>>
        [[maybe_unused]]
        void placeOrder(T && gun) {
            std::scoped_lock lock { m_mutex };
            m_gun = gun;
            m_counter.store(0);
        }

        template<typename T>
        requires std::is_convertible_v<T, std::function<void()>>
        [[maybe_unused]]
        void placeOrder(T && gun, DateTime::SleepUnit waitingTime) {
            assert(waitingTime >= 0ms);
            std::scoped_lock lock { m_mutex };
            m_gun = gun;
            m_counter.store(waitingTime / c_sleepQuantum);
        }

        [[maybe_unused]]
        void cancelOrder() {
            std::scoped_lock lock { m_mutex };
            m_gun = [] {};
            m_counter.store(-1);
        }

        [[maybe_unused]]
        void await() {
            while (m_counter.load() > 0) {
                std::this_thread::sleep_for(c_sleepQuantum);
                --m_counter;
            }
        }

        [[maybe_unused]]
        void await(DateTime::SleepUnit waitingTime) {
            assert(waitingTime.count() >= 0);
            m_counter.store(waitingTime / c_sleepQuantum);
            while (m_counter.load() > 0) {
                std::this_thread::sleep_for(c_sleepQuantum);
                --m_counter;
            }
        }

        template<typename T>
        requires std::is_invocable_r_v<bool, T>
        [[maybe_unused]]
        void await(T && safetyLock) {
            while (m_counter.load() > 0 && std::invoke(safetyLock)) {
                std::this_thread::sleep_for(c_sleepQuantum);
                --m_counter;
            }
            m_counter.store(0);
        }

        template<typename T>
        requires std::is_invocable_r_v<bool, T>
        [[maybe_unused]]
        void await(DateTime::SleepUnit waitingTime, T && safetyLock) {
            assert(waitingTime >= 0ms);
            m_counter.store(waitingTime / c_sleepQuantum);
            while (m_counter.load() > 0 && std::invoke(safetyLock)) {
                std::this_thread::sleep_for(c_sleepQuantum);
                --m_counter;
            }
            m_counter.store(0);
        }

        // [[nodiscard, maybe_unused]]
        // bool awaiting() {
        //     return m_counter.load() > 0;
        // }

        [[maybe_unused]]
        void completeOrder() {
            if (m_counter.load() < 0) {
                return;
            }
            std::scoped_lock lock { m_mutex };
            m_gun();
            m_gun = [] {};
            m_counter.store(-1);
        }
    };
}
