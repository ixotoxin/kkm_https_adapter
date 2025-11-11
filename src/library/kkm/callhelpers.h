// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "device.h"
#include "impex.h"
#include <cassert>
#include <type_traits>

namespace Kkm {
    template<class R>
    concept AutoResult = std::is_same_v<R, Nln::Json> || std::is_same_v<R, OptionalResult>;

    template<std::derived_from<Result> R>
    using UndetailedMethod = void (Device::*)(R &);

    template<class R, class K, AutoResult AR>
    requires std::derived_from<std::remove_cvref_t<K>, Device>
    [[maybe_unused]]
    void callMethod(K && kkm, UndetailedMethod<R> method, AR & result) {
        if constexpr (std::is_same_v<AR, Nln::Json>) {
            assert(result.is_object());
        } else {
            assert(!result.has_value() || result.value().is_object());
        }
        R callResult {};
        (kkm.*method)(callResult);
        result << callResult;
    }

    template<class K, AutoResult AR, typename ... METHODS>
    requires std::derived_from<std::remove_cvref_t<K>, Device>
    [[maybe_unused]]
    void collectDataFromMethods(AR & result, K && kkm, METHODS ... method) {
        (callMethod(kkm, method, result), ...);
    }

    template<std::derived_from<Result> R, std::derived_from<Details> D>
    using DetailedMethod = void (Device::*)(const D &, R &);

    template<class R, class D, class K, AutoResult AR>
    requires std::derived_from<std::remove_cvref_t<K>, Device>
    [[maybe_unused]]
    void callMethod(K && kkm, DetailedMethod<R, D> method, const D & details, AR & result) {
        if constexpr (std::is_same_v<AR, Nln::Json>) {
            assert(result.is_object());
        } else {
            assert(!result.has_value() || result.value().is_object());
        }
        R callResult {};
        (kkm.*method)(details, callResult);
        result << callResult;
    }

    template<class R, class D, class K, AutoResult AR>
    requires std::derived_from<std::remove_cvref_t<K>, Device>
    [[maybe_unused]]
    void callMethod(K && kkm, DetailedMethod<R, D> method, const Nln::Json & details, AR & result) {
        if constexpr (std::is_same_v<AR, Nln::Json>) {
            assert(result.is_object());
        } else {
            assert(!result.has_value() || result.value().is_object());
        }
        D callDetails {};
        details >> callDetails;
        callMethod(std::forward<K>(kkm), method, callDetails, result);
    }
}
