// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "errexp.h"
#include "defer.h"

namespace System {
    [[nodiscard, maybe_unused]]
    std::wstring errorMessage(const ::DWORD error) noexcept {
        wchar_t * text { nullptr };
        const Deferred::LocalFree freeTextBuffer { text };
        try {
            const auto status
                = ::FormatMessageW(
                    FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
                    nullptr,
                    error,
                    LANG_SYSTEM_DEFAULT, // MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                    reinterpret_cast<::LPWSTR>(&text),
                    0,
                    nullptr
                );
            if (status) {
                wchar_t * end { text + std::wcslen(text) };
                if (end != text) {
                    --end;
                    while (
                        end != text
                        && (*end == L' ' || *end == L'\n' || *end == L'\r' || *end == L'\t' || *end == L'\v' || *end == L'\f')
                    ) {
                        *end-- = L'\0';
                    }
                    while (--end != text) {
                        if (*end == L'\n' || *end == L'\r' /*|| *end == L'\t' || *end == L'\v' || *end == L'\f'*/) {
                            *end = L' ';
                        }
                    }
                }
            } else {
                freeTextBuffer.perform();
                text = const_cast<wchar_t *>(Basic::Wcs::c_somethingWrong);
            }
        } catch (...) {
            freeTextBuffer.perform();
            text = const_cast<wchar_t *>(Basic::Wcs::c_somethingWrong);
        }
        return text;
    }
}
