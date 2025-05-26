// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "basic.h"
#include <stringapiset.h>

namespace Text {
    namespace Wcs {
        using Basic::Wcs::c_fallbackErrorMessage;
    }

    namespace Mbs {
        using Basic::Mbs::c_fallbackErrorMessage;
    }

    inline bool convert(std::wstring & result, const std::string_view text) noexcept try {
        if (text.empty()) {
            result.clear();
            return true;
        }
        auto count
            = ::MultiByteToWideChar(
                CP_UTF8, MB_ERR_INVALID_CHARS,
                text.data(), static_cast<int>(text.length()), nullptr, 0
            );
        if (count <= 0) {
            return false;
        }
        const int size { count + 1 };
        auto buffer = std::make_shared<wchar_t[]>(static_cast<size_t>(size), L'\0');
        count
            = ::MultiByteToWideChar(
                CP_UTF8, MB_ERR_INVALID_CHARS,
                text.data(), static_cast<int>(text.length()), buffer.get(), size
            );
        if (count <= 0) {
            return false;
        }
        result.assign(buffer.get());
        return true;
    } catch (...) {
        return false;
    }

    inline bool convert(std::string & result, const std::wstring_view text) noexcept try {
        if (text.empty()) {
            result.clear();
            return true;
        }
        auto count
            = ::WideCharToMultiByte(
                CP_UTF8, WC_ERR_INVALID_CHARS | WC_NO_BEST_FIT_CHARS,
                text.data(), static_cast<int>(text.length()), nullptr, 0, nullptr, nullptr
            );
        if (count <= 0) {
            return false;
        }
        const int size { count + 1 };
        auto buffer = std::make_shared<char[]>(static_cast<size_t>(size), '\0');
        count
            = ::WideCharToMultiByte(
                CP_UTF8, WC_ERR_INVALID_CHARS | WC_NO_BEST_FIT_CHARS,
                text.data(), static_cast<int>(text.length()), buffer.get(), size, nullptr, nullptr
            );
        if (count <= 0) {
            return false;
        }
        result.assign(buffer.get());
        return true;
    } catch (...) {
        return false;
    }

    inline std::wstring convert(const std::string_view text) noexcept try {
        std::wstring result {};
        if (!convert(result, text)) {
            result.assign(Wcs::c_fallbackErrorMessage);
        }
        return result;
    } catch (...) {
        return Wcs::c_fallbackErrorMessage;
    }

    inline std::string convert(const std::wstring_view text) noexcept try {
        std::string result {};
        if (!convert(result, text)) {
            result.assign(Mbs::c_fallbackErrorMessage);
        }
        return result;
    } catch (...) {
        return Mbs::c_fallbackErrorMessage;
    }
}
