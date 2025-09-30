// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "device.h"
#include "impex.h"
#include <type_traits>

namespace Kkm {
    template<std::derived_from<Result> R>
    using UndetailedMethod = void (Device::*)(R &);

    template<class R, class K>
    requires std::derived_from<std::remove_cvref_t<K>, Device>
    [[maybe_unused]]
    inline void callMethod(K && kkm, UndetailedMethod<R> method, Nln::Json & result) {
        assert(result.is_object());
        R callResult {};
        (kkm.*method)(callResult);
        result << callResult;
    }

    template<class K, typename ... METHODS>
    requires std::derived_from<std::remove_cvref_t<K>, Device>
    [[maybe_unused]]
    inline void collectDataFromMethods(Nln::Json & result, K && kkm, METHODS ... method) {
        (callMethod(std::forward<std::remove_cvref_t<K>>(kkm), method, result), ...);
    }

    template<std::derived_from<Result> R, std::derived_from<Details> D>
    using DetailedMethod = void (Device::*)(const D &, R &);

    template<class R, class D, class K>
    requires std::derived_from<std::remove_cvref_t<K>, Device>
    [[maybe_unused]]
    inline void callMethod(K && kkm, DetailedMethod<R, D> method, const D & details, Nln::Json & result) {
        assert(result.is_object());
        R callResult {};
        (kkm.*method)(details, callResult);
        result << callResult;
    }

    template<class R, class D, class K>
    requires std::derived_from<std::remove_cvref_t<K>, Device>
    [[maybe_unused]]
    inline void callMethod(K && kkm, DetailedMethod<R, D> method, const Nln::Json & details, Nln::Json & result) {
        assert(details.is_object());
        D callDetails {};
        details >> callDetails;
        callMethod(std::forward<std::remove_cvref_t<K>>(kkm), method, callDetails, result);
    }
}
