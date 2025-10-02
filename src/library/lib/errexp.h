// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "winapi.h"
#include "strings.h"
#include <string>
#include <format>

namespace System {
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
