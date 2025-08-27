// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "basic.h"
#include <source_location>

namespace Basic {
    inline void append(std::string & message, const std::source_location & location) noexcept try {
        message.append(location.file_name());
        message.append(" [");
        message.append(std::to_string(location.line()));
        message.append(":");
        message.append(std::to_string(location.column()));
        message.append("]");
    } catch (...) {}

    inline void append(std::wstring & message, const std::source_location & location) noexcept try {
        auto mbFilePath = location.file_name();
        auto size = ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, mbFilePath, -1, nullptr, 0);
        if (size <= 0) {
            return;
        }
        auto wcFilePath = std::make_shared<wchar_t[]>(static_cast<size_t>(size));
        size = ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, mbFilePath, -1, wcFilePath.get(), size);
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

inline std::string & operator+=(std::string & message, const std::source_location & location) noexcept {
    message.append(" (в файле ");
    Basic::append(message, location);
    message.append(")");
    return message;
}

inline std::string operator+(const std::string & message, const std::source_location & location) noexcept {
    std::string result(message);
    result += location;
    return result;
}

inline std::string operator+(const std::string_view message, const std::source_location & location) noexcept {
    std::string result(message);
    result += location;
    return result;
}

inline std::wstring & operator+=(std::wstring & message, const std::source_location & location) {
    message.append(L" (в файле ");
    Basic::append(message, location);
    message.append(L")");
    return message;
}

inline std::wstring operator+(const std::wstring & message, const std::source_location & location) {
    std::wstring result(message);
    result += location;
    return result;
}

inline std::wstring operator+(const std::wstring_view message, const std::source_location & location) {
    std::wstring result(message);
    result += location;
    return result;
}

namespace Log {
#ifdef DEBUG
    inline bool s_appendLocation { true };
#else
    inline bool s_appendLocation { false };
#endif
}
