// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "winapi.h"
#include "strings.h"
#include <cmake/variables.h>
#include <memory>
#include <string>
#include <source_location>
#include <stringapiset.h>

namespace SrcLoc {
    const auto c_prefixLength = std::string_view(BUILD_SOURCE).length() + 1;

    [[maybe_unused]]
    inline std::string toMbs(const std::source_location & location) {
        std::string result { " (" };
        result.append(Basic::Mbs::c_source);
        result.append(" ");
        result.append(std::string_view(location.file_name()).substr(c_prefixLength));
        result.append(" [");
        result.append(std::to_string(location.line()));
        result.append(":");
        result.append(std::to_string(location.column()));
        result.append("])");
        return result;
    }

    [[maybe_unused]]
    inline std::wstring toWcs(const std::source_location & location) {
        std::wstring result { L" (" };
        result.append(Basic::Wcs::c_source);
        result.append(L" ");
        auto mbFilePath = location.file_name() + c_prefixLength;
        auto size = WIN_MB2WC_ESTIMATED(mbFilePath, -1);
        if (size > 0) {
            auto wcFilePath = std::make_unique_for_overwrite<wchar_t[]>(static_cast<std::size_t>(size));
            size = WIN_MB2WC(mbFilePath, -1, wcFilePath.get(), size);
            if (size > 0) {
                result.append(wcFilePath.get());
                result.append(L" [");
                result.append(std::to_wstring(location.line()));
                result.append(L":");
                result.append(std::to_wstring(location.column()));
                result.append(L"])");
            } else {
                result.append(L"?)");
            }
        } else {
            result.append(L"?)");
        }
        return result;
    }

    [[maybe_unused]]
    inline void append(std::string & message, const std::source_location & location) noexcept try {
        message.append(std::string_view(location.file_name()).substr(c_prefixLength));
        message.append(" [");
        message.append(std::to_string(location.line()));
        message.append(":");
        message.append(std::to_string(location.column()));
        message.append("]");
    } catch (...) {}

    [[maybe_unused]]
    inline void append(std::wstring & message, const std::source_location & location) noexcept try {
        auto mbFilePath = location.file_name() + c_prefixLength;
        auto size = WIN_MB2WC_ESTIMATED(mbFilePath, -1);
        if (size <= 0) {
            return;
        }
        auto wcFilePath = std::make_unique_for_overwrite<wchar_t[]>(static_cast<std::size_t>(size));
        size = WIN_MB2WC(mbFilePath, -1, wcFilePath.get(), size);
        if (size <= 0) {
            return;
        }
        message.append(wcFilePath.get());
        message.append(L" [");
        message.append(std::to_wstring(location.line()));
        message.append(L":");
        message.append(std::to_wstring(location.column()));
        message.append(L"]");
    } catch (...) {}
}

[[maybe_unused]]
inline std::string & operator+=(std::string & message, const std::source_location & location) noexcept {
    message.append(" (");
    message.append(Basic::Mbs::c_source);
    message.append(" ");
    SrcLoc::append(message, location);
    message.append(")");
    return message;
}

[[nodiscard, maybe_unused]]
inline std::string operator+(const std::string & message, const std::source_location & location) noexcept {
    std::string result(message);
    result += location;
    return result;
}

[[nodiscard, maybe_unused]]
inline std::string operator+(const std::string_view message, const std::source_location & location) noexcept {
    std::string result(message);
    result += location;
    return result;
}

[[maybe_unused]]
inline std::wstring & operator+=(std::wstring & message, const std::source_location & location) {
    message.append(L" (");
    message.append(Basic::Wcs::c_source);
    message.append(L" ");
    SrcLoc::append(message, location);
    message.append(L")");
    return message;
}

[[nodiscard, maybe_unused]]
inline std::wstring operator+(const std::wstring & message, const std::source_location & location) {
    std::wstring result(message);
    result += location;
    return result;
}

[[nodiscard, maybe_unused]]
inline std::wstring operator+(const std::wstring_view message, const std::source_location & location) {
    std::wstring result(message);
    result += location;
    return result;
}
