// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "winstrapi.h"
#include "strings.h"
#include <cmake/options.h>
#include <memory>
#include <string>

namespace Text {
    namespace {
        constexpr int c_convertBufferReserve { 2 };
    }

    [[maybe_unused]]
    inline bool convert(std::wstring & result, const std::string_view text) noexcept try {
        if (text.empty()) {
            result.clear();
            return result.empty();
        }
        auto length = WIN_MB2WC_ESTIMATED(&text[0], static_cast<int>(text.size()));
        if (length <= 0) {
            return false;
        }
#if WITH_SSIAC
        result.resize(length + c_convertBufferReserve);
        length = WIN_MB2WC(&text[0], static_cast<int>(text.size()), &result[0], static_cast<int>(result.size()));
        result.resize(length);
        return length > 0;
#else
        const int size { length + c_convertBufferReserve };
        const auto buffer = std::make_unique_for_overwrite<wchar_t[]>(static_cast<std::size_t>(size));
        length = WIN_MB2WC(&text[0], static_cast<int>(text.size()), buffer.get(), size);
        if (length <= 0) {
            return false;
        }
        result.assign(buffer.get(), length);
        return !result.empty();
#endif
    } catch (...) {
        return false;
    }

    [[maybe_unused]]
    inline bool convert(std::string & result, const std::wstring_view text) noexcept try {
        if (text.empty()) {
            result.clear();
            return result.empty();
        }
        auto length = WIN_WC2MB_ESTIMATED(&text[0], static_cast<int>(text.size()));
        if (length <= 0) {
            return false;
        }
#if WITH_SSIAC
        result.resize(length + c_convertBufferReserve);
        length = WIN_WC2MB(&text[0], static_cast<int>(text.size()), &result[0], static_cast<int>(result.size()));
        result.resize(length);
        return length > 0;
#else
        const int size { length + c_convertBufferReserve };
        const auto buffer = std::make_unique_for_overwrite<char[]>(static_cast<std::size_t>(size));
        length = WIN_WC2MB(&text[0], static_cast<int>(text.size()), buffer.get(), size);
        if (length <= 0) {
            return false;
        }
        result.assign(buffer.get(), length);
        return !result.empty();
#endif
    } catch (...) {
        return false;
    }

    [[nodiscard, maybe_unused]]
    inline std::wstring convert(const std::string_view text) noexcept {
        std::wstring result {};
        try {
            if (!convert(result, text)) {
                result.assign(Basic::Wcs::c_fallbackErrorMessage);
            }
        } catch (...) {
            result.assign(Basic::Wcs::c_fallbackErrorMessage);
        }
        return result;
    }

    [[nodiscard, maybe_unused]]
    inline std::string convert(const std::wstring_view text) noexcept {
        std::string result {};
        try {
            if (!convert(result, text)) {
                result.assign(Basic::Mbs::c_fallbackErrorMessage);
            }
        } catch (...) {
            result.assign(Basic::Mbs::c_fallbackErrorMessage);
        }
        return result;
    }
}
