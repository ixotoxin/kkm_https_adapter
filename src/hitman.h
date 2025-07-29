// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "library/basic.h"
#include <functional>
#include <mutex>

namespace Server {
    namespace Hitman {
        std::mutex s_mutex {};
        std::function<void()> s_gun { [] {} };
        int64_t s_counter { 0 };

        void placeOrder(std::function<void()> gun, std::integral auto counter) {
            assert(counter > 0);
            std::scoped_lock lock { s_mutex };
            s_gun = gun;
            s_counter = static_cast<int64_t>(counter);
        }

        [[maybe_unused]]
        void lastChance(std::integral auto counter) {
            assert(counter > 0);
            std::scoped_lock lock { s_mutex };
            if (s_counter >= 0) {
                s_counter = static_cast<int64_t>(counter);
            }
        }

        [[maybe_unused]]
        void cancelOrder() {
            std::scoped_lock lock { s_mutex };
            s_gun = [] {};
            s_counter = -1;
        }

        void completeOrder() {
            std::scoped_lock lock { s_mutex };
            s_gun();
            s_gun = [] {};
            s_counter = -1;
        }

        [[maybe_unused]]
        void countDown() {
            std::scoped_lock lock { s_mutex };
            if (s_counter > 0) {
                --s_counter;
            }
        }

        [[nodiscard, maybe_unused]]
        bool awaiting() {
            std::scoped_lock lock { s_mutex };
            return s_counter > 0;
        }

        [[nodiscard, maybe_unused]]
        bool ready() {
            std::scoped_lock lock { s_mutex };
            return s_counter == 0;
        }

        [[nodiscard, maybe_unused]]
        int64_t counter() {
            std::scoped_lock lock { s_mutex };
            return s_counter;
        }
    }
}
