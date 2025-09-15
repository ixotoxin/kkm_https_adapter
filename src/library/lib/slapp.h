// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "winapi.h"
#include "strings.h"
#include <memory>
#include <string>
#include <source_location>
#include <stringapiset.h>

namespace Basic {
    [[maybe_unused]]
    inline void append(std::string & message, const std::source_location & location) noexcept try {
        message.append(location.file_name());
        message.append(" [");
        message.append(std::to_string(location.line()));
        message.append(":");
        message.append(std::to_string(location.column()));
        message.append("]");
    } catch (...) {}

    [[maybe_unused]]
    inline void append(std::wstring & message, const std::source_location & location) noexcept try {
        auto mbFilePath = location.file_name();
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
    message.append(Basic::Mbs::c_inFile);
    message.append(" ");
    Basic::append(message, location);
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
    message.append(Basic::Wcs::c_inFile);
    message.append(L" ");
    Basic::append(message, location);
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
