// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "basic.h"
#include <chrono>
#include "meta.h"

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
    namespace Wcs {
        constexpr std::wstring_view c_timestamp { L"{:04d}-{:02d}-{:02d} {:02d}:{:02d}:{:02d}" };
        constexpr std::wstring_view c_timestampPrefix [[maybe_unused]] { L"{:04d}-{:02d}-{:02d} {:02d}:{:02d}:{:02d}: " };
    }

    namespace Mbs {
        constexpr std::string_view c_timestamp { "{:04d}-{:02d}-{:02d} {:02d}:{:02d}:{:02d}" };
        constexpr std::string_view c_timestampPrefix [[maybe_unused]] { "{:04d}-{:02d}-{:02d} {:02d}:{:02d}:{:02d}: " };
    }

    using Clock = std::chrono::system_clock;
    using Point = decltype(Clock::now());
    using Offset = std::chrono::seconds;
    using OffsetValue = Offset::rep;

    [[nodiscard, maybe_unused]] int64_t windows();
    [[nodiscard, maybe_unused]] int64_t unix();
    [[nodiscard, maybe_unused]] std::wostream & iso(std::wostream &);

    template<Meta::String T>
    [[nodiscard, maybe_unused]]
    inline T cast(const std::tm & dateTime) {
        if constexpr (Meta::isWide<T>) {
            return
                std::format(
                    Wcs::c_timestamp, dateTime.tm_year + 1900, dateTime.tm_mon + 1, dateTime.tm_mday,
                    dateTime.tm_hour, dateTime.tm_min, dateTime.tm_sec
                );
        } else {
            return
                std::format(
                    Mbs::c_timestamp, dateTime.tm_year + 1900, dateTime.tm_mon + 1, dateTime.tm_mday,
                    dateTime.tm_hour, dateTime.tm_min, dateTime.tm_sec
                );
        }
    }

    template<Meta::Clock T, Meta::TimePoint U>
    [[nodiscard, maybe_unused]]
    inline auto cast(const U & timePoint) {
        return std::chrono::clock_cast<T>(timePoint);
    }

    template<Meta::TimePoint T, Meta::TimePoint U>
    [[nodiscard, maybe_unused]]
    inline auto cast(const U & timePoint) {
        return std::chrono::clock_cast<typename T::clock>(timePoint);
    }
}
