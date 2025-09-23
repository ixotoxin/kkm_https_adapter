// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "winapi.h"
#include "strings.h"
#include <concepts>
#include <utility>
#include <functional>
#include <string>
#include <format>

namespace System {
    using namespace std::string_view_literals;

    namespace Wcs {
        constexpr const std::wstring_view c_fault { L"Error {0:#10x}: {1}" };
        constexpr const std::wstring_view c_failed { L"{0} failed with error {1:#010x}: {2}" };
    }

    [[nodiscard, maybe_unused]] std::wstring errorMessage(::DWORD) noexcept;

    [[nodiscard, maybe_unused]]
    inline std::wstring
    explainError(const ::DWORD error = ::GetLastError())
    noexcept try {
        return std::format(Wcs::c_fault, error, errorMessage(error));
    } catch (...) {
        return Basic::Wcs::c_somethingWrong;
    }

    [[nodiscard, maybe_unused]]
    inline std::wstring
    explainError(const std::wstring_view operation, const ::DWORD error = ::GetLastError())
    noexcept try {
        return std::format(Wcs::c_failed, operation, error, errorMessage(error));
    } catch (...) {
        return Basic::Wcs::c_somethingWrong;
    }
}

namespace Deferred {
    template<std::invocable T>
    class Exec {
        T m_func;
        bool m_permitted { true };

    public:
        Exec() = delete;
        Exec(const Exec &) = delete;
        Exec(Exec &&) = delete;
        [[maybe_unused]] explicit Exec(T & func) : m_func(func) {}
        [[maybe_unused]] explicit Exec(T && func) : m_func(std::forward<T>(func)) {}

        ~Exec() noexcept {
            if (m_permitted) {
                std::invoke(m_func);
            }
        }

        Exec & operator=(const Exec &) = delete;
        Exec & operator=(Exec &&) = delete;

        [[maybe_unused]]
        void perform(bool repeatable = false) noexcept {
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

        explicit LocalFree(auto & memory)
        : m_memory(reinterpret_cast<HLOCAL &>(memory)) {}

        ~LocalFree() noexcept {
            perform();
        }

        LocalFree & operator=(const LocalFree &) = delete;
        LocalFree & operator=(LocalFree &&) = delete;

        [[maybe_unused]]
        void perform() noexcept {
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
