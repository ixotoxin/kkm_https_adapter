// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "basic.h"
#include <algorithm>
#include "meta.h"
#include "except.h"

namespace Numeric {
    namespace Wcs {
        using Basic::Wcs::c_rangeError;
    }

    using Basic::DataError;

    template<std::signed_integral T>
    inline void doubleClamp(T & a, T & b, const T min, const T max) {
        if (a < min) {
            a = min;
        } else if (a > max) {
            a = max;
        }
        if (b < min || b > max) {
            b = a;
        }
    }

    template<Meta::String T, Meta::Numeric U>
    [[maybe_unused]]
    inline T cast(U num) {
        if constexpr (std::is_same_v<T, std::wstring>) {
            return std::to_wstring(num);
        } else {
            return std::to_string(num);
        }
    }

    template<Meta::View T, Meta::BooleanLabels U>
    [[nodiscard, maybe_unused]]
    inline T boolCast(auto && value) {
        return static_cast<bool>(value) ? Meta::BoolLabels<T, U>::c_true : Meta::BoolLabels<T, U>::c_false;
    }

    template<Meta::Numeric T>
    [[nodiscard, maybe_unused]]
    inline Meta::Filter<T> clamp(T min, T max) {
        return [min, max] (const T value) -> T { return std::clamp(value, min, max); };
    }

    template<Meta::Numeric T>
    [[nodiscard, maybe_unused]]
    inline Meta::Filter<T> min(T min) {
        return
            [min] (const T value) -> T {
                if (value < min) {
                    throw DataError(Wcs::c_rangeError);
                }
                return value;
            };
    }

    template<Meta::Numeric T>
    [[nodiscard, maybe_unused]]
    inline Meta::Filter<T> max(T max) {
        return
            [max] (const T value) -> T {
                if (value > max) {
                    throw DataError(Wcs::c_rangeError);
                }
                return value;
            };
    }

    template<Meta::Numeric T>
    [[nodiscard, maybe_unused]]
    inline Meta::Filter<T> between(T min, T max) {
        return
            [min, max] (const T value) -> T {
                if (value < min || value > max) {
                    throw DataError(Wcs::c_rangeError);
                }
                return value;
            };
    }
}
