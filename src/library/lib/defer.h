// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "winapi.h"
#include <concepts>
#include <utility>
#include <functional>

namespace Deferred {
    template<std::invocable T>
    class Exec {
        T m_func;
        bool m_permitted { true };

    public:
        Exec() = delete;
        Exec(const Exec &) = delete;
        Exec(Exec &&) = delete;
        [[maybe_unused]] explicit Exec(const T & func) noexcept : m_func(func) {}
        [[maybe_unused]] explicit Exec(T && func) noexcept : m_func(std::forward<T>(func)) {}

        ~Exec() noexcept {
            if (m_permitted) {
                std::invoke(m_func);
            }
        }

        Exec & operator=(const Exec &) = delete;
        Exec & operator=(Exec &&) = delete;

        [[maybe_unused]]
        void perform(const bool repeatable = false) noexcept {
            if (m_permitted) {
                std::invoke(m_func);
                m_permitted = repeatable;
            }
        }

        [[maybe_unused]]
        void cancel() noexcept {
            m_permitted = false;
        }
    };

    class LocalFree {
        HLOCAL & m_memory;
        bool m_permitted { true };

    public:
        LocalFree() = delete;
        LocalFree(const LocalFree &) = delete;
        LocalFree(LocalFree &&) = delete;
        [[maybe_unused]] explicit LocalFree(auto & memory) noexcept : m_memory(reinterpret_cast<HLOCAL &>(memory)) {}

        ~LocalFree() noexcept {
            perform();
        }

        LocalFree & operator=(const LocalFree &) = delete;
        LocalFree & operator=(LocalFree &&) = delete;

        [[maybe_unused]]
        void perform() const noexcept {
            if (m_permitted && m_memory) {
                ::LocalFree(m_memory);
                m_memory = nullptr;
            }
        }

        [[maybe_unused]]
        void cancel() noexcept {
            m_permitted = false;
        }
    };
}
