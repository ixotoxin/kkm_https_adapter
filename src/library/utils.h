// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "basic.h"
#include <functional>
#include <iostream>
#include <cassert>
#include <utility>

namespace System {
    using namespace std::string_view_literals;

    namespace Wcs {
        using Basic::Wcs::c_somethingWrong;

        constexpr std::wstring_view c_fault { L"Ошибка {:#10x}: {}" };
        constexpr std::wstring_view c_failed { L"{} failed with error {:#010x}: {}" };
    }

    [[nodiscard, maybe_unused]] std::wstring errorMessage(DWORD) noexcept;

    [[nodiscard, maybe_unused]]
    inline std::wstring explainError(const DWORD error = ::GetLastError()) noexcept try {
        return std::format(Wcs::c_fault, error, errorMessage(error));
    } catch (...) {
        return Wcs::c_somethingWrong;
    }

    [[nodiscard, maybe_unused]]
    inline std::wstring
    explainError(const std::wstring_view operation, const DWORD error = ::GetLastError()) noexcept try {
        return std::format(Wcs::c_failed, operation, error, errorMessage(error));
    } catch (...) {
        return Wcs::c_somethingWrong;
    }

    [[nodiscard, maybe_unused]]
    inline bool isGoodPath(const std::wstring_view path) {
        return
            path.find_first_of(
                L"\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f"
                L"\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f"
                // L"<>:\"/\\|?*"
                L"<>:\"|?*"sv
            ) == std::wstring_view::npos
            && path.find(L"\\."sv) == std::wstring_view::npos
            && path.find(L"/."sv) == std::wstring_view::npos
            && path.find(L".\\"sv) == std::wstring_view::npos
            && path.find(L"./"sv) == std::wstring_view::npos
            && path.find(L"\\ "sv) == std::wstring_view::npos
            && path.find(L"/ "sv) == std::wstring_view::npos
            && path.find(L" \\"sv) == std::wstring_view::npos
            && path.find(L" /"sv) == std::wstring_view::npos
            && path.find(L".."sv) == std::wstring_view::npos
            && path.find_last_not_of(L" ."sv) == path.size() - 1;
    }

    [[nodiscard, maybe_unused]]
    inline bool isGoodPath(const std::string_view path) {
        return
            path.find_first_of(
                "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f"
                "\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f"
                // "<>:\"/\\|?*"
                "<>:\"|?*"sv
            ) == std::string_view::npos
            && path.find("\\."sv) == std::string_view::npos
            && path.find("/."sv) == std::string_view::npos
            && path.find(".\\"sv) == std::string_view::npos
            && path.find("./"sv) == std::string_view::npos
            && path.find("\\ "sv) == std::string_view::npos
            && path.find("/ "sv) == std::string_view::npos
            && path.find(" \\"sv) == std::string_view::npos
            && path.find(" /"sv) == std::string_view::npos
            && path.find(".."sv) == std::string_view::npos
            && path.find_last_not_of(" ."sv) == path.size() - 1;
    }
}

namespace Deferred {
    namespace Wcs {
        using Basic::Wcs::c_somethingWrong;
    }

    class Exec {
        std::function<void()> m_func;
        bool * m_except;
        bool m_permitted { true };

    public:
        Exec() = delete;
        Exec(const Exec &) = delete;
        Exec(Exec &&) = delete;

        [[maybe_unused]]
        explicit Exec(std::function<void()> && func, bool * except = nullptr)
        : m_func(std::forward<std::function<void()>>(func)), m_except(except) {}

        ~Exec() noexcept {
            try {
                if (m_permitted) {
                    m_func();
                }
                if (m_except) {
                    *m_except = false;
                }
            } catch (...) {
                if (m_except) {
                    *m_except = true;
                } else {
                    std::wclog << Wcs::c_somethingWrong << std::endl;
#ifdef DEBUG
                    __debugbreak();
#endif
                }
            }
        }

        Exec & operator=(const Exec &) = delete;
        Exec & operator=(Exec &&) = delete;

        void perform(bool repeatable = false) {
            if (m_permitted) {
                if (m_except) {
                    try {
                        m_func();
                        *m_except = false;
                    } catch (...) {
                        *m_except = true;
                    }
                } else {
                    m_func();
                }
                m_permitted = repeatable;
            }
        }

        void cancel() {
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

        ~LocalFree() {
            perform();
        }

        LocalFree & operator=(const LocalFree &) = delete;
        LocalFree & operator=(LocalFree &&) = delete;

        void perform() {
            if (m_permitted && m_memory) {
                ::LocalFree(m_memory);
                m_memory = nullptr;
            }
        }

        void cancel() {
            m_permitted = false;
        }
    };
}
