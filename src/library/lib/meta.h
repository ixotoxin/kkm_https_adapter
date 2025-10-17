// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <type_traits>
#include <concepts>
#include <ranges>
#include <string>

namespace Meta {
    #define classTplt template<typename ...> class

    template<classTplt, classTplt> struct templatesComparing : std::false_type {};
    template<classTplt T> struct templatesComparing<T, T> : std::true_type {};

    template<classTplt T, classTplt U>
    [[maybe_unused]]
    constexpr inline bool isSameTemplate = templatesComparing<T, U>::value;

    template<class T, classTplt U>
    concept fromTemplate = requires(T t) { [] <typename ... V>(U<V ...> &) {} (t); };

    template<class T>
    constexpr std::underlying_type_t<T> toUnderlying(T e) noexcept {
        return static_cast<std::underlying_type_t<T>>(e);
    }

    template<typename>
    struct ArrayElement {};

    template<typename T>
    requires std::is_array_v<T>
    struct ArrayElement<T> { using Type = std::remove_cv_t<std::remove_extent_t<T>>; };

    template<typename T>
    requires std::is_pointer_v<T>
    struct ArrayElement<T> { using Type = std::remove_cv_t<std::remove_pointer_t<T>>; };

    template<typename T>
    using ArrayElementType = ArrayElement<T>::Type;

    template<typename T>
    using ContainerElementType = std::remove_cvref_t<T>::value_type;

    template<typename ...>
    struct Rebind;

    template<template<typename ...> class Tmpl, typename ... SrcArgs>
    struct Rebind<Tmpl<SrcArgs...>> {
        template<typename ... NewArgs>
        using With = Tmpl<NewArgs...>;
    };

    template<typename T>
    concept BackSideGrowingRange
        = std::ranges::range<T>
          && requires (T t, typename T::value_type u) { t.emplace_back(u); t.push_back(u); };

    template<typename T, typename U>
    concept Filter = std::is_invocable_r_v<U, T, std::conditional_t<std::is_scalar_v<U>, const U, const U &>>;

    template<typename T, typename U, typename V>
    concept EnumCastStrictMap
        = std::ranges::range<T>
          && requires {
                typename T::key_type;
                typename T::mapped_type;
                typename T::iterator;
             }
          && std::is_same_v<U, typename T::key_type>
          && std::is_same_v<V, typename T::mapped_type>
          && requires (T r, U k) {
                { r.find(k) } -> std::same_as<typename T::iterator>;
                { r.find(k)->second } -> std::same_as<V &>;
             };

    template<typename T, typename U>
    concept EnumCastMap
        = std::ranges::range<T>
          && requires {
                typename T::key_type;
                typename T::mapped_type;
                typename T::iterator;
             }
          && std::is_same_v<U, typename T::mapped_type>
          && requires (T r, typename T::key_type k) {
                { r.find(k) } -> std::same_as<typename T::iterator>;
                { r.find(k)->second } -> std::same_as<U &>;
             };

    template<typename T, typename U>
    concept EnumDomain
        = std::ranges::range<T>
          && requires {
                typename T::value_type;
                typename T::iterator;
             }
          && std::is_same_v<U, typename T::value_type>
          && requires (T r, U k) {
                { std::ranges::find(r, k) } -> std::same_as<typename T::iterator>;
                { *std::ranges::find(r, k) } -> std::same_as<U &>;
             };

    template<typename T>
    struct CastTypeTrait { using CastType = T; };

    template<typename>
    struct CastTrait {};

    template<typename T>
    concept Int8 = !std::is_same_v<T, bool> && std::integral<T> && sizeof(T) == 1;

    template<typename T>
    concept Int8Relying = Int8<std::remove_cvref_t<T>>;

    template<typename T>
    concept Int16 = std::integral<T> && sizeof(T) == 2;

    template<typename T>
    concept Int16Relying = Int16<std::remove_cvref_t<T>>;

    template<typename T>
    concept Int32 = std::integral<T> && sizeof(T) == 4;

    template<typename T>
    concept Int32Relying = Int32<std::remove_cvref_t<T>>;

    template<typename T>
    concept Int64 = std::integral<T> && sizeof(T) == 8;

    template<typename T>
    concept Int64Relying = Int64<std::remove_cvref_t<T>>;

    template<typename T>
    concept Bool = std::is_same_v<T, bool>;

    template<typename T>
    concept BoolRelying = Bool<std::remove_cvref_t<T>>;

    template<typename T>
    concept Integral = std::integral<T> && !Bool<T>;

    template<typename T>
    concept IntegralRelying = Integral<std::remove_cvref_t<T>>;

    template<typename T>
    concept FloatingPoint = std::is_same_v<T, double> || std::is_same_v<T, long double>;

    template<typename T>
    concept FloatingPointRelying = FloatingPoint<std::remove_cvref_t<T>>;

    template<typename T>
    concept Numeric = Integral<T> || FloatingPoint<T>;

    template<typename T>
    concept NumericRelying = Numeric<std::remove_cvref_t<T>>;

    template<> struct CastTrait<signed char> : CastTypeTrait<signed long> {};
    template<> struct CastTrait<unsigned char> : CastTypeTrait<unsigned long> {};
    template<> struct CastTrait<signed short> : CastTypeTrait<signed long> {};
    template<> struct CastTrait<unsigned short> : CastTypeTrait<unsigned long> {};
    template<> struct CastTrait<signed int> : CastTypeTrait<signed long> {};
    template<> struct CastTrait<unsigned int> : CastTypeTrait<unsigned long> {};
    template<> struct CastTrait<signed long> : CastTypeTrait<signed long> {};
    template<> struct CastTrait<unsigned long> : CastTypeTrait<unsigned long> {};
    template<> struct CastTrait<signed long long> : CastTypeTrait<signed long long> {};
    template<> struct CastTrait<unsigned long long> : CastTypeTrait<unsigned long long> {};

    template<typename T>
    concept Char = std::is_same_v<T, wchar_t> || std::is_same_v<T, char>;

    template<typename T>
    concept CharRelying = Char<std::remove_cvref_t<T>>;

    template<typename T>
    concept String = std::is_same_v<T, std::wstring> || std::is_same_v<T, std::string>;

    template<typename T>
    concept StringRelying = String<std::remove_cvref_t<T>>;

    template<typename T>
    concept View = std::is_same_v<T, std::wstring_view> || std::is_same_v<T, std::string_view>;

    template<typename T>
    concept ViewRelying = View<std::remove_cvref_t<T>>;

    template<typename>
    struct WideTypes : std::false_type {};

    template<>
    struct WideTypes<wchar_t> : std::true_type {};

    template<>
    struct WideTypes<wchar_t *> : std::true_type {};

    template<>
    struct WideTypes<std::wstring> : std::true_type {};

    template<>
    struct WideTypes<std::wstring_view> : std::true_type {};

    template<typename T>
    constexpr bool isWide = WideTypes<std::remove_cvref_t<T>>::value;

    struct DaNet {};
    struct YesNo {};
    struct EnaDis {};
    struct TrueFalse {};

    template<typename T>
    concept BooleanLabels
        = std::is_same_v<T, DaNet> || std::is_same_v<T, YesNo>
          || std::is_same_v<T, EnaDis> || std::is_same_v<T, TrueFalse>;

    template<View, BooleanLabels>
    struct BoolLabels {};
}
