// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "meta.h"
#include "except.h"
#include <algorithm>
#include <chrono>
#include <format>
#include <ctime>

namespace Meta {
    template<class T>
    concept Clock = std::chrono::is_clock_v<T>;

    template<class T>
    concept TimePoint
        = requires (T t) {
            typename T::clock;
            typename T::duration;
            typename T::rep;
            typename T::period;
            t.time_since_epoch();
            std::chrono::time_point_cast<std::chrono::seconds>(t);
          }
          && std::chrono::is_clock_v<typename T::clock>;
}

namespace DateTime {
    using namespace std::chrono_literals;

    namespace Wcs {
        constexpr std::wstring_view c_timestamp { L"{:04d}-{:02d}-{:02d} {:02d}:{:02d}:{:02d}" };
    }

    namespace Mbs {
        constexpr std::string_view c_timestamp { "{:04d}-{:02d}-{:02d} {:02d}:{:02d}:{:02d}" };
    }

    using Basic::DataError;

    using Clock = std::chrono::system_clock;
    using Point = std::remove_cvref_t<decltype(Clock::now())>;
    using Offset = std::chrono::seconds;
    using SleepUnit = std::chrono::milliseconds;

    constexpr SleepUnit c_basicSleep { 1'000ms };
    constexpr SleepUnit c_basicSleepQuantum { 200ms };

    [[nodiscard, maybe_unused]] int64_t windows();
    [[nodiscard, maybe_unused]] int64_t unix();
    [[nodiscard, maybe_unused]] std::wostream & iso(std::wostream &);
    [[nodiscard, maybe_unused]] std::ostream & iso(std::ostream &);

    template<Meta::String T>
    [[nodiscard, maybe_unused]]
    T cast(const std::tm & dateTime) {
        if constexpr (Meta::isWide<T>) {
            return
                std::format(
                    Wcs::c_timestamp, dateTime.tm_year + 1'900, dateTime.tm_mon + 1, dateTime.tm_mday,
                    dateTime.tm_hour, dateTime.tm_min, dateTime.tm_sec
                );
        } else {
            return
                std::format(
                    Mbs::c_timestamp, dateTime.tm_year + 1'900, dateTime.tm_mon + 1, dateTime.tm_mday,
                    dateTime.tm_hour, dateTime.tm_min, dateTime.tm_sec
                );
        }
    }

    template<Meta::Clock T, Meta::TimePoint U>
    [[nodiscard, maybe_unused]]
    auto cast(const U & timePoint) {
        return std::chrono::clock_cast<T>(timePoint);
    }

    template<Meta::TimePoint T, Meta::TimePoint U>
    [[nodiscard, maybe_unused]]
    auto cast(const U & timePoint) {
        return std::chrono::clock_cast<typename T::clock>(timePoint);
    }

    template<Meta::fromTemplate<std::chrono::duration> T>
    [[nodiscard, maybe_unused]]
    auto clamp(T min, T max) {
        return [min, max] (const T value) -> T { return std::clamp(value, min, max); };
    }

    template<Meta::fromTemplate<std::chrono::duration> T>
    [[nodiscard, maybe_unused]]
    auto min(T min) {
        return
            [min] (const T value) -> T {
                if (value < min) {
                    throw DataError(Basic::Wcs::c_rangeError); // NOLINT(*-exception-baseclass)
                }
                return value;
            };
    }

    template<Meta::fromTemplate<std::chrono::duration> T>
    [[nodiscard, maybe_unused]]
    auto max(T max) {
        return
            [max] (const T value) -> T {
                if (value > max) {
                    throw DataError(Basic::Wcs::c_rangeError); // NOLINT(*-exception-baseclass)
                }
                return value;
            };
    }

    template<Meta::fromTemplate<std::chrono::duration> T>
    [[nodiscard, maybe_unused]]
    auto between(T min, T max) {
        return
            [min, max] (const T value) -> T {
                if (value < min || value > max) {
                    throw DataError(Basic::Wcs::c_rangeError); // NOLINT(*-exception-baseclass)
                }
                return value;
            };
    }
}

inline void Sleep(const DateTime::SleepUnit & duration) {
    ::Sleep(static_cast<::DWORD>(duration.count()));
}
