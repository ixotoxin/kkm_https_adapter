// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "winstrapi.h"
#include "strings.h"
#include <cmake/variables.h>
#include <algorithm>
#include <memory>
#include <array>
#include <string>

#if !WITH_RELSL || !defined(_MSC_VER) || defined(__clang__)
#include <source_location>
#endif

namespace SrcLoc {
#if WITH_RELSL && defined(_MSC_VER) && !defined(__clang__)
    // ISSUE: Оптимизировать, если возможно.
    class Point {
        size_t m_line {};
        std::array<char, MAX_PATH> m_file {};

        static constexpr std::string_view c_prefix { BUILD_PREFIX };
        static constexpr std::string_view c_invalidPath { "[invalid-path]" };

    public:
        Point() = default;
        Point(const Point &) = default;
        Point(Point &&) = default;
        ~Point() = default;

        Point & operator=(const Point &) = default;
        Point & operator=(Point &&) = default;

        [[nodiscard]] static consteval Point current(
            const size_t line = __builtin_LINE(),
            const char * file = __builtin_FILE()
        ) noexcept {
            Point result;
            result.m_line = line;
            if (const std::string_view view0 { file }; view0.starts_with(c_prefix)) {
                std::ranges::copy(c_invalidPath, result.m_file.begin());
                result.m_file[c_invalidPath.size()] = 0;
            } else {
                const std::string_view view { view0.substr(c_prefix.length()) };
                if (view.length() >= MAX_PATH - 1 || view.empty()) {
                    std::ranges::copy(c_invalidPath, result.m_file.begin());
                    result.m_file[c_invalidPath.size()] = 0;
                } else {
                    result.m_file[0] = '.';
                    std::ranges::copy(view, result.m_file.begin() + 1);
                    result.m_file[view.size() + 1] = 0;
                }
            }
            return result;
        }

        [[nodiscard, maybe_unused]] constexpr size_t line() const noexcept { return m_line; }
        [[nodiscard, maybe_unused]] constexpr const char * file_name() const noexcept { return &m_file[0]; }
    };
#else
    using Point = std::source_location;
#endif

    [[maybe_unused]]
    inline void append(std::wstring & message, const Point & slp) noexcept try {
        const auto mbFilePath = slp.file_name();
        auto size = WIN_MB2WC_ESTIMATED(mbFilePath, -1);
        if (size <= 0) {
            return;
        }
        const auto wcFilePath = std::make_unique_for_overwrite<wchar_t[]>(static_cast<std::size_t>(size));
        size = WIN_MB2WC(mbFilePath, -1, wcFilePath.get(), size);
        if (size <= 0) {
            return;
        }
        message.append(wcFilePath.get());
        message.append(L":");
        message.append(std::to_wstring(slp.line()));
    } catch (...) {}

    [[maybe_unused]]
    inline std::wstring toWcs(const Point & slp) {
        std::wstring result { Basic::Wcs::c_source };
        result.append(L" ");
        append(result, slp);
        return result;
    }

    [[maybe_unused]]
    inline void append(std::string & message, const Point & slp) noexcept try {
        message.append(slp.file_name());
        message.append(":");
        message.append(std::to_string(slp.line()));
    } catch (...) {}

    [[maybe_unused]]
    inline std::string toMbs(const Point & slp) {
        std::string result { Basic::Mbs::c_source };
        result.append(" ");
        append(result, slp);
        return result;
    }
}

[[maybe_unused]]
inline std::string & operator+=(std::string & message, const SrcLoc::Point & slp) noexcept {
    message.append(" (");
    message.append(Basic::Mbs::c_source);
    message.append(" ");
    SrcLoc::append(message, slp);
    message.append(")");
    return message;
}

[[nodiscard, maybe_unused]]
inline std::string operator+(const std::string & message, const SrcLoc::Point & slp) noexcept {
    std::string result { message };
    result += slp;
    return result;
}

[[nodiscard, maybe_unused]]
inline std::string operator+(const std::string_view message, const SrcLoc::Point & slp) noexcept {
    std::string result { message };
    result += slp;
    return result;
}

[[maybe_unused]]
inline std::wstring & operator+=(std::wstring & message, const SrcLoc::Point & slp) {
    message.append(L" (");
    message.append(Basic::Wcs::c_source);
    message.append(L" ");
    SrcLoc::append(message, slp);
    message.append(L")");
    return message;
}

[[nodiscard, maybe_unused]]
inline std::wstring operator+(const std::wstring & message, const SrcLoc::Point & slp) {
    std::wstring result { message };
    result += slp;
    return result;
}

[[nodiscard, maybe_unused]]
inline std::wstring operator+(const std::wstring_view message, const SrcLoc::Point & slp) {
    std::wstring result { message };
    result += slp;
    return result;
}
