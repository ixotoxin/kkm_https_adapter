// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "datetime.h"
#include "winapi.h"

namespace DateTime {
    [[nodiscard, maybe_unused]]
    int64_t windows() {
        ::FILETIME time;
        ::GetSystemTimeAsFileTime(&time);
        ::LARGE_INTEGER time64 {
            .LowPart = time.dwLowDateTime,
            .HighPart = static_cast<decltype(time64.HighPart)>(time.dwHighDateTime)
        };
        return time64.QuadPart;
    }

    [[nodiscard, maybe_unused]]
    int64_t unix() {
        constexpr int64_t unixTimeStart = 0x019db1ded53e8000;
        constexpr int64_t ticksPerSecond = 10000000;
        ::FILETIME time;
        ::GetSystemTimeAsFileTime(&time);
        ::LARGE_INTEGER time64 {
            .LowPart = time.dwLowDateTime,
            .HighPart = static_cast<decltype(time64.HighPart)>(time.dwHighDateTime)
        };
        return (time64.QuadPart - unixTimeStart) / ticksPerSecond;
    }

    [[nodiscard, maybe_unused]]
    std::wostream & iso(std::wostream & stream) {
        ::SYSTEMTIME localTime;
        ::GetLocalTime(&localTime);
        stream
            << std::format(
                Wcs::c_timestamp,
                localTime.wYear, localTime.wMonth, localTime.wDay,
                localTime.wHour, localTime.wMinute, localTime.wSecond
            );
        return stream;
    }

    [[nodiscard, maybe_unused]]
    std::ostream & iso(std::ostream & stream) {
        ::SYSTEMTIME localTime;
        ::GetLocalTime(&localTime);
        stream
            << std::format(
                Mbs::c_timestamp,
                localTime.wYear, localTime.wMonth, localTime.wDay,
                localTime.wHour, localTime.wMinute, localTime.wSecond
            );
        return stream;
    }
}
