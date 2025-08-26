// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "basic.h"
#include <cassert>
#include <algorithm>
#include <unordered_set>
#include "meta.h"
#include "except.h"

namespace Meta {
    struct Mbs;

    struct Wcs {
        using Char = wchar_t;
        using String = std::wstring;
        using View = std::wstring_view;
        using Opposite = Mbs;

        static constexpr Char c_minus [[maybe_unused]] { L'-' };
        static constexpr Char c_terminator [[maybe_unused]] { L'\0' };
        static constexpr View c_assignator [[maybe_unused]] { L"=:" };

        inline static const std::unordered_set<String> c_trueValueStrings [[maybe_unused]] {
            L"true", L"t", L"yes", L"y", L"on", L"enable", L"ena", L"en", L"e", L"allow", L"allowed", L"a", L"+"
        };

        inline static const std::unordered_set<String> c_falseValueStrings [[maybe_unused]] {
            L"false", L"f", L"no", L"n", L"off", L"disable", L"dis", L"d", L"deny", L"denied", L"-"
        };

        [[nodiscard, maybe_unused]]
        static bool trimPredicate(wchar_t);

        [[nodiscard, maybe_unused]]
        static bool noTrimPredicate(wchar_t);

        [[nodiscard, maybe_unused]]
        static wchar_t toLower(wchar_t);

        template<typename T>
        [[maybe_unused]]
        inline static T toNumeric(const Char * str, Char ** end);

        template<>
        [[maybe_unused]]
        inline signed long toNumeric<signed long>(const Char * str, Char ** end) {
            return std::wcstol(str, end, 10);
        }

        template<>
        [[maybe_unused]]
        inline unsigned long toNumeric<unsigned long>(const Char * str, Char ** end) {
            return std::wcstoul(str, end, 10);
        }

        template<>
        [[maybe_unused]]
        inline signed long long toNumeric<signed long long>(const Char * str, Char ** end) {
            return std::wcstoll(str, end, 10);
        }

        template<>
        [[maybe_unused]]
        inline unsigned long long toNumeric<unsigned long long>(const Char * str, Char ** end) {
            return std::wcstoull(str, end, 10);
        }

        template<>
        [[maybe_unused]]
        inline double toNumeric<double>(const Char * str, Char ** end) {
            return std::wcstod(str, end);
        }

        template<>
        [[maybe_unused]]
        inline long double toNumeric<long double>(const Char * str, Char ** end) {
            return std::wcstold(str, end);
        }

        [[nodiscard, maybe_unused]]
        inline static bool contains(const Char * str, Char ch) {
            return std::wcschr(str, ch) != nullptr;
        }

        [[nodiscard, maybe_unused]]
        inline static auto length(const Char * str) {
            return std::wcslen(str);
        }

        [[nodiscard, maybe_unused]]
        inline static auto length(const String & str) {
            return str.length();
        }

        [[nodiscard, maybe_unused]]
        inline static auto length(const View str) {
            return str.length();
        }
    };

    struct Mbs {
        using Char = char;
        using String = std::string;
        using View = std::string_view;
        using Opposite = Wcs;

        static constexpr Char c_minus [[maybe_unused]] { '-' };
        static constexpr Char c_terminator [[maybe_unused]] { '\0' };
        static constexpr View c_assignator [[maybe_unused]] { "=:" };

        inline static const std::unordered_set<String> c_trueValueStrings {
            "true", "t", "yes", "y", "on", "enable", "ena", "en", "e", "allow", "allowed", "a", "+"
        };

        inline static const std::unordered_set<String> c_falseValueStrings {
            "false", "f", "no", "n", "off", "disable", "dis", "d", "deny", "denied", "-"
        };

        [[nodiscard, maybe_unused]]
        static bool trimPredicate(unsigned char);

        [[nodiscard, maybe_unused]]
        static bool noTrimPredicate(unsigned char);

        [[nodiscard, maybe_unused]]
        static char toLower(unsigned char);

        template<typename T>
        [[maybe_unused]]
        inline static T toNumeric(const Char * str, Char ** end);

        template<>
        [[maybe_unused]]
        inline signed long toNumeric<signed long>(const Char * str, Char ** end) {
            return std::strtol(str, end, 10);
        }

        template<>
        [[maybe_unused]]
        inline unsigned long toNumeric<unsigned long>(const Char * str, Char ** end) {
            return std::strtoul(str, end, 10);
        }

        template<>
        [[maybe_unused]]
        inline signed long long toNumeric<signed long long>(const Char * str, Char ** end) {
            return std::strtoll(str, end, 10);
        }

        template<>
        [[maybe_unused]]
        inline unsigned long long toNumeric<unsigned long long>(const Char * str, Char ** end) {
            return std::strtoull(str, end, 10);
        }

        template<>
        [[maybe_unused]]
        inline double toNumeric<double>(const Char * str, Char ** end) {
            return std::strtod(str, end);
        }

        template<>
        [[maybe_unused]]
        inline long double toNumeric<long double>(const Char * str, Char ** end) {
            return std::strtold(str, end);
        }

        [[nodiscard, maybe_unused]]
        inline static bool contains(const Char * str, Char ch) {
            return std::strchr(str, ch) != nullptr;
        }

        [[nodiscard, maybe_unused]]
        inline static auto length(const Char * str) {
            return std::strlen(str);
        }

        [[nodiscard, maybe_unused]]
        inline static auto length(const String & str) {
            return str.length();
        }

        [[nodiscard, maybe_unused]]
        inline static auto length(const View str) {
            return str.length();
        }
    };

    template<>
    struct CalcIsWide<Wcs> : std::true_type {};

    template<typename T>
    concept Wideness = std::is_same_v<T, Wcs> || std::is_same_v<T, Mbs>;

    template<typename T>
    struct TextTrait {};

    template<>
    struct TextTrait<Wcs::Char> : Wcs { using Wideness = Wcs; };

    template<>
    struct TextTrait<Mbs::Char> : Mbs { using Wideness = Mbs; };

    template<typename T>
    requires std::is_same_v<ArrayElementType<T>, Wcs::Char> || std::is_same_v<ArrayElementType<T>, Mbs::Char>
    struct TextTrait<T> : TextTrait<ArrayElementType<T>> {};

    template<typename T>
    requires String<std::remove_cvref_t<T>> || View<std::remove_cvref_t<T>>
    struct TextTrait<T> : TextTrait<ContainerElementType<T>> {};

    template<typename T>
    concept TextualContainer
        = BackSideGrowingRange<T>
          && (String<typename T::value_type> || View<typename T::value_type>)
          && requires (
                T t,
                typename T::value_type u,
                TextTrait<typename T::value_type>::View v,
                TextTrait<typename T::value_type>::View::size_type z
             ) {
                t.emplace_back(u, z, z);
                t.emplace_back(v, z, z);
             };
}

namespace Text {
    namespace Wcs {
        using Basic::Wcs::c_invalidValue;
        using Basic::Wcs::c_rangeError;
    }

    using Basic::DataError;

    template<Meta::Char T>
    [[nodiscard, maybe_unused]]
    inline T * trim(T * text) {
        using Trait = Meta::TextTrait<T>;
        if (text && *text) {
            while (Trait::trimPredicate(*text)) { ++text; }
            if (*text) {
                auto end = text + Trait::length(text) - 1;
                while (Trait::trimPredicate(*end)) { --end; }
                end[1] = Trait::c_terminator;
            }
        }
        return text;
    }

    template<Meta::String T>
    [[maybe_unused]]
    inline void trim(T & text) {
        using Trait = Meta::TextTrait<T>;
        text.erase(text.begin(), std::find_if(text.begin(), text.end(), Trait::noTrimPredicate));
        text.erase(std::find_if(text.rbegin(), text.rend(), Trait::noTrimPredicate).base(), text.end());
    }

    template<Meta::View T>
    [[nodiscard, maybe_unused]]
    inline Meta::TextTrait<T>::String trimmed(const T text) {
        using Trait = Meta::TextTrait<T>;
        auto end = std::find_if(text.rbegin(), text.rend(), Trait::noTrimPredicate).base();
        return { std::find_if(text.begin(), end, Trait::noTrimPredicate), end };
    }

    template<Meta::Char T>
    [[nodiscard, maybe_unused]]
    inline auto trimmed(const T * text) {
        return trimmed<typename Meta::TextTrait<T>::View>(text);
    }

    template<Meta::String T>
    [[nodiscard, maybe_unused]]
    inline auto trimmed(const T & text) {
        return trimmed<typename Meta::TextTrait<T>::View>(text);
    }

    template<Meta::Char T>
    [[maybe_unused]]
    inline T * lower(T * text) {
        using Trait = Meta::TextTrait<T>;
        if (text && *text) {
            std::transform(text, text + Trait::length(text), text, Trait::toLower);
        }
        return text;
    }

    template<Meta::String T>
    [[maybe_unused]]
    inline void lower(T & text) {
        std::transform(text.begin(), text.end(), text.begin(), Meta::TextTrait<T>::toLower);
    }

    template<Meta::String T>
    [[nodiscard, maybe_unused]]
    inline T lowered(T text) {
        std::transform(text.begin(), text.end(), text.begin(), Meta::TextTrait<T>::toLower);
        return text;
    }

    template<Meta::Char T>
    [[nodiscard, maybe_unused]]
    inline auto lowered(const T * text) {
        return lowered<typename Meta::TextTrait<T>::String>(text);
    }

    template<Meta::View T>
    [[nodiscard, maybe_unused]]
    inline auto lowered(T text) {
        return lowered<typename Meta::TextTrait<T>::String>({ text.data(), text.length() });
    }

    template<Meta::Bool T, Meta::String U>
    [[nodiscard, maybe_unused]]
    inline T cast(U text) {
        using Txt = Meta::TextTrait<U>;
        if (text.empty()) {
            throw DataError(Wcs::c_invalidValue); // NOLINT(*-exception-baseclass)
        }
        std::transform(text.begin(), text.end(), text.begin(), Txt::toLower);
        if (Txt::c_trueValueStrings.contains(text)) {
            return true;
        } else if (Txt::c_falseValueStrings.contains(text)) {
            return false;
        }
        throw DataError(Wcs::c_invalidValue); // NOLINT(*-exception-baseclass)
    }

    template<Meta::Bool T, Meta::Char U>
    [[nodiscard, maybe_unused]]
    inline T cast(const U * text) {
        return cast<T, typename Meta::TextTrait<U>::String>(text);
    }

    template<Meta::Bool T, Meta::View U>
    [[nodiscard, maybe_unused]]
    inline T cast(const U text) {
        return cast<T, typename Meta::TextTrait<U>::String>(text);
    }

    template<Meta::Integral T, Meta::Char U>
    [[nodiscard, maybe_unused]]
    inline T cast(const U * text) {
        using Num = Meta::CastTrait<T>;
        using Txt = Meta::TextTrait<U>;
        if constexpr (std::is_signed_v<T>) {
            if (Txt::length(text) == 0) {
                throw DataError(Wcs::c_invalidValue); // NOLINT(*-exception-baseclass)
            }
        } else {
            if (Txt::length(text) == 0 || Txt::contains(text, Txt::c_minus)) {
                throw DataError(Wcs::c_invalidValue); // NOLINT(*-exception-baseclass)
            }
        }
        typename Txt::Char * end {};
        typename Num::CastType value = Txt::template toNumeric<typename Num::CastType>(text, &end);
        if (end == text || *end) {
            throw DataError(Wcs::c_invalidValue); // NOLINT(*-exception-baseclass)
        }
        if constexpr (sizeof(T) != sizeof(typename Num::CastType)) {
            if (
                value < static_cast<typename Num::CastType>(std::numeric_limits<T>::min())
                || value > static_cast<typename Num::CastType>(std::numeric_limits<T>::max())
            ) {
                throw DataError(Wcs::c_invalidValue); // NOLINT(*-exception-baseclass)
            }
        }
        return static_cast<T>(value);
    }

    template<Meta::FloatingPoint T, Meta::Char U>
    [[nodiscard, maybe_unused]]
    inline T cast(const U * text) {
        using Txt = Meta::TextTrait<U>;
        if (Txt::length(text) == 0) {
            throw DataError(Wcs::c_invalidValue); // NOLINT(*-exception-baseclass)
        }
        typename Txt::Char * end {};
        T value = Txt::template toNumeric<T>(text, &end);
        if (end == text || *end) {
            throw DataError(Wcs::c_invalidValue); // NOLINT(*-exception-baseclass)
        }
        return value;
    }

    template<Meta::Numeric T, Meta::String U>
    [[nodiscard, maybe_unused]]
    inline T cast(const U & text) {
        return cast<T, typename Meta::TextTrait<U>::Char>(text.c_str());
    }

    template<Meta::Numeric T, Meta::View U>
    [[nodiscard, maybe_unused]]
    inline T cast(const U text) {
        return cast<T>(typename Meta::TextTrait<U>::String { text });
    }

    template<class T>
    requires (Meta::BackSideGrowingRange<T> && Meta::String<typename T::value_type>)
    [[maybe_unused]]
    inline Meta::Rebind<T>::template With<typename Meta::TextTrait<typename T::value_type>::Opposite::String>
    convert(const T & container) {
        using Type = typename Meta::TextTrait<typename T::value_type>::Opposite::String;
        typename Meta::Rebind<T>::template With<Type> result;
        for (auto & item : container) {
            result.emplace_back(convert(item));
        }
        return result;
    }

    template<Meta::String T>
    [[nodiscard, maybe_unused]]
    inline auto trim() {
        return [] (const T & value) -> T { return Text::trimmed(value); };
    }

    template<Meta::String T>
    [[nodiscard, maybe_unused]]
    inline auto trim(Meta::Filter<T> auto && subFilter0) {
        return
            [subFilter = std::forward<decltype(subFilter0)>(subFilter0)]
            (const T & value) -> T {
                return Text::trimmed(subFilter(value));
            };
    }

    template<Meta::String T>
    [[nodiscard, maybe_unused]]
    inline auto lower() {
        return [] (const T & value) -> T { return Text::lowered(value); };
    }

    template<Meta::String T>
    [[nodiscard, maybe_unused]]
    inline auto lower(Meta::Filter<T> auto && subFilter0) {
        return
            [subFilter = std::forward<decltype(subFilter0)>(subFilter0)]
            (const T & value) -> T {
                return Text::lowered(subFilter(value));
            };
    }

    template<Meta::String T>
    [[nodiscard, maybe_unused]]
    inline auto noEmpty() {
        return
            [] (const T & value) -> T {
                if (value.empty()) {
                    throw DataError(Wcs::c_rangeError); // NOLINT(*-exception-baseclass)
                }
                return value;
            };
    }

    template<Meta::String T>
    [[nodiscard, maybe_unused]]
    inline auto noEmpty(Meta::Filter<T> auto && subFilter0) {
        return
            [subFilter = std::forward<decltype(subFilter0)>(subFilter0)]
            (const T & value) -> T {
                T filtered { subFilter(value) };
                if (filtered.empty()) {
                    throw DataError(Wcs::c_rangeError); // NOLINT(*-exception-baseclass)
                }
                return filtered;
            };
    }

    template<Meta::String T>
    [[nodiscard, maybe_unused]]
    inline auto length(size_t min, size_t max) {
        assert(max > min);
        return
            [min, max] (const T & value) -> T {
                if (value.length() < min || value.length() > max) {
                    throw DataError(Wcs::c_rangeError); // NOLINT(*-exception-baseclass)
                }
                return value;
            };
    }

    template<Meta::String T>
    [[nodiscard, maybe_unused]]
    inline auto maxLength(size_t max) {
        assert(max > 0);
        return
            [max] (const T & value) -> T {
                if (value.length() > max) {
                    throw DataError(Wcs::c_rangeError); // NOLINT(*-exception-baseclass)
                }
                return value;
            };
    }

    template<Meta::String T>
    [[nodiscard, maybe_unused]]
    inline auto length(size_t min, size_t max, Meta::Filter<T> auto && subFilter0) {
        assert(max > min);
        return
            [min, max, subFilter = std::forward<decltype(subFilter0)>(subFilter0)]
            (const T & value) -> T {
                T filtered { subFilter(value) };
                if (filtered.length() < min || filtered.length() > max) {
                    throw DataError(Wcs::c_rangeError); // NOLINT(*-exception-baseclass)
                }
                return filtered;
            };
    }

    template<Meta::String T>
    [[nodiscard, maybe_unused]]
    inline auto maxLength(size_t max, Meta::Filter<T> auto && subFilter0) {
        assert(max > 0);
        return
            [max, subFilter = std::forward<decltype(subFilter0)>(subFilter0)]
            (const T & value) -> T {
                T filtered { subFilter(value) };
                if (filtered.length() > max) {
                    throw DataError(Wcs::c_rangeError); // NOLINT(*-exception-baseclass)
                }
                return filtered;
            };
    }

    template<Meta::TextualContainer T>
    [[maybe_unused]]
    inline typename T::size_type splitTo(
        T & receiver,
        const typename Meta::TextTrait<typename T::value_type>::View text,
        const typename Meta::TextTrait<typename T::value_type>::View delims,
        bool clear = false
    ) {
        if (clear && !receiver.empty()) {
            receiver.clear();
        }
        if (text.empty() || delims.empty()) {
            return 0;
        }
        auto first = text.find_first_not_of(delims);
        while (first != T::value_type::npos) {
            auto last = text.find_first_of(delims, first + 1);
            if (last == T::value_type::npos) {
                receiver.emplace_back(text, first, text.length() - first);
                break;
            } else {
                receiver.emplace_back(text, first, last - first);
            }
            first = text.find_first_not_of(delims, last + 1);
        }
        return receiver.size();
    }

    template<Meta::String T>
    [[maybe_unused]]
    inline void splitVariable(
        const typename Meta::TextTrait<T>::View text,
        T & name,
        T & value,
        bool lowerName = true,
        bool lowerValue = false,
        const typename Meta::TextTrait<T>::View separator = Meta::TextTrait<T>::c_assignator
    ) {
        name.clear();
        value.clear();
        if (text.empty() || separator.empty()) {
            return;
        }
        auto position = text.find_first_of(separator);
        if (position == T::npos) {
            name.assign(trimmed(text));
            if (lowerName) {
                lower(name);
            }
            return;
        }
        name.assign(text, 0, position);
        if (position <= text.length()) {
            value.assign(text, position + 1, text.length());
        }
        trim(name);
        if (lowerName) {
            lower(name);
        }
        trim(value);
        if (lowerValue) {
            lower(value);
        }
    }

    template<Meta::TextualContainer T>
    [[maybe_unused]]
    inline typename T::value_type::size_type joinTo(
        typename Meta::TextTrait<typename T::value_type>::String & receiver,
        const T & container,
        const typename Meta::TextTrait<typename T::value_type>::View glue,
        bool clear = false
    ) {
        if (clear && !receiver.empty()) {
            receiver.clear();
        }
        for (const auto & str : container) {
            if (!str.empty()) {
                if (!receiver.empty()) {
                    receiver.append(glue);
                }
                receiver.append(str);
            }
        }
        return receiver.length();
    }

    template<Meta::String T>
    [[maybe_unused]]
    inline typename T::size_type joinTo(
        T & receiver,
        T && text,
        const typename Meta::TextTrait<T>::View glue
    ) {
        if (receiver.empty()) {
            receiver.assign(std::forward<T>(text));
        } else {
            receiver.append(glue);
            receiver.append(text);
        }
        return receiver.length();
    }

    template<Meta::String T>
    [[maybe_unused]]
    inline typename T::size_type joinTo(
        T & receiver,
        const typename Meta::TextTrait<T>::View text,
        const typename Meta::TextTrait<T>::View glue
    ) {
        if (receiver.empty()) {
            receiver.assign(text);
        } else {
            receiver.append(glue);
            receiver.append(text);
        }
        return receiver.length();
    }

    template<typename ... T>
    requires (std::is_convertible_v<T, Meta::Wcs::View> && ...)
    [[nodiscard, maybe_unused]]
    inline Meta::Wcs::String concat(T && ... text) {
        Meta::Wcs::String result;
        (result.append(text), ...);
        return result;
    }

    template<typename ... T>
    requires (std::is_convertible_v<T, Meta::Mbs::View> && ...)
    [[nodiscard, maybe_unused]]
    inline Meta::Mbs::String concat(T && ... text) {
        Meta::Mbs::String result;
        (result.append(text), ...);
        return result;
    }

    template<Meta::String T, typename ... U>
    requires (std::is_convertible_v<U, typename Meta::TextTrait<T>::View> && ...)
    [[maybe_unused]]
    inline typename T::size_type concatTo(T & receiver, U && ... text) {
        (receiver.append(text), ...);
        return receiver.length();
    }

    template<Meta::Wideness T>
    [[nodiscard, maybe_unused]]
    inline typename T::View yesNo(bool value) {
        return static_cast<bool>(value)
           ? Meta::BoolLabels<typename T::View, Meta::YesNo>::c_true
           : Meta::BoolLabels<typename T::View, Meta::YesNo>::c_false;
    }

    template<Meta::Wideness T>
    [[nodiscard, maybe_unused]]
    inline typename T::View enaDis(bool value) {
        return static_cast<bool>(value)
            ? Meta::BoolLabels<typename T::View, Meta::EnaDis>::c_true
            : Meta::BoolLabels<typename T::View, Meta::EnaDis>::c_false;
    }

    template<Meta::Wideness T>
    [[nodiscard, maybe_unused]]
    inline typename T::View trueFalse(bool value) {
        return static_cast<bool>(value)
           ? Meta::BoolLabels<typename T::View, Meta::TrueFalse>::c_true
           : Meta::BoolLabels<typename T::View, Meta::TrueFalse>::c_false;
    }

    namespace Wcs {
        [[nodiscard, maybe_unused]]
        inline auto trim() {
            return [] (const std::wstring & value) -> std::wstring { return Text::trimmed(value); };
        }

        [[nodiscard, maybe_unused]]
        inline auto trim(Meta::Filter<std::wstring> auto && subFilter0) {
            return
                [subFilter = std::forward<decltype(subFilter0)>(subFilter0)]
                (const std::wstring & value) -> std::wstring {
                    return Text::trimmed(subFilter(value));
                };
        }

        [[nodiscard, maybe_unused]]
        inline auto lower() {
            return [] (const std::wstring & value) -> std::wstring { return Text::lowered(value); };
        }

        [[nodiscard, maybe_unused]]
        inline auto lower(Meta::Filter<std::wstring> auto && subFilter0) {
            return
                [subFilter = std::forward<decltype(subFilter0)>(subFilter0)]
                (const std::wstring & value) -> std::wstring {
                    return Text::lowered(subFilter(value));
                };
        }

        [[nodiscard, maybe_unused]]
        inline auto noEmpty() {
            return
                [] (const std::wstring & value) -> std::wstring {
                    if (value.empty()) {
                        throw DataError(Wcs::c_rangeError); // NOLINT(*-exception-baseclass)
                    }
                    return value;
                };
        }

        [[nodiscard, maybe_unused]]
        inline auto noEmpty(Meta::Filter<std::wstring> auto && subFilter0) {
            return
                [subFilter = std::forward<decltype(subFilter0)>(subFilter0)]
                (const std::wstring & value) -> std::wstring {
                    std::wstring filtered { subFilter(value) };
                    if (filtered.empty()) {
                        throw DataError(Wcs::c_rangeError); // NOLINT(*-exception-baseclass)
                    }
                    return filtered;
                };
        }

        [[nodiscard, maybe_unused]]
        inline auto length(size_t min, size_t max) {
            assert(max > min);
            return
                [min, max] (const std::wstring & value) -> std::wstring {
                    if (value.length() < min || value.length() > max) {
                        throw DataError(Wcs::c_rangeError); // NOLINT(*-exception-baseclass)
                    }
                    return value;
                };
        }

        [[nodiscard, maybe_unused]]
        inline auto maxLength(size_t max) {
            assert(max > 0);
            return
                [max] (const std::wstring & value) -> std::wstring {
                    if (value.length() > max) {
                        throw DataError(Wcs::c_rangeError); // NOLINT(*-exception-baseclass)
                    }
                    return value;
                };
        }

        [[nodiscard, maybe_unused]]
        inline auto length(size_t min, size_t max, Meta::Filter<std::wstring> auto && subFilter0) {
            assert(max > min);
            return
                [min, max, subFilter = std::forward<decltype(subFilter0)>(subFilter0)]
                (const std::wstring & value) -> std::wstring {
                    std::wstring filtered { subFilter(value) };
                    if (filtered.length() < min || filtered.length() > max) {
                        throw DataError(Wcs::c_rangeError); // NOLINT(*-exception-baseclass)
                    }
                    return filtered;
                };
        }

        [[nodiscard, maybe_unused]]
        inline auto maxLength(size_t max, Meta::Filter<std::wstring> auto && subFilter0) {
            assert(max > 0);
            return
                [max, subFilter = std::forward<decltype(subFilter0)>(subFilter0)]
                (const std::wstring & value) -> std::wstring {
                    std::wstring filtered { subFilter(value) };
                    if (filtered.length() > max) {
                        throw DataError(Wcs::c_rangeError); // NOLINT(*-exception-baseclass)
                    }
                    return filtered;
                };
        }

        [[nodiscard, maybe_unused]]
        inline std::wstring_view yesNo(bool value) {
            return static_cast<bool>(value)
                ? Meta::BoolLabels<std::wstring_view, Meta::YesNo>::c_true
                : Meta::BoolLabels<std::wstring_view, Meta::YesNo>::c_false;
        }

        [[nodiscard, maybe_unused]]
        inline std::wstring_view enaDis(bool value) {
            return static_cast<bool>(value)
                ? Meta::BoolLabels<std::wstring_view, Meta::EnaDis>::c_true
                : Meta::BoolLabels<std::wstring_view, Meta::EnaDis>::c_false;
        }

        [[nodiscard, maybe_unused]]
        inline std::wstring_view trueFalse(bool value) {
            return static_cast<bool>(value)
                ? Meta::BoolLabels<std::wstring_view, Meta::TrueFalse>::c_true
                : Meta::BoolLabels<std::wstring_view, Meta::TrueFalse>::c_false;
        }
    }

    namespace Mbs {
        [[nodiscard, maybe_unused]]
        inline auto trim() {
            return [] (const std::string & value) -> std::string { return Text::trimmed(value); };
        }

        [[nodiscard, maybe_unused]]
        inline auto trim(Meta::Filter<std::string> auto && subFilter0) {
            return
                [subFilter = std::forward<decltype(subFilter0)>(subFilter0)]
                (const std::string & value) -> std::string {
                    return Text::trimmed(subFilter(value));
                };
        }

        [[nodiscard, maybe_unused]]
        inline auto lower() {
            return [] (const std::string & value) -> std::string { return Text::lowered(value); };
        }

        [[nodiscard, maybe_unused]]
        inline auto lower(Meta::Filter<std::string> auto && subFilter0) {
            return
                [subFilter = std::forward<decltype(subFilter0)>(subFilter0)]
                (const std::string & value) -> std::string {
                    return Text::lowered(subFilter(value));
                };
        }

        [[nodiscard, maybe_unused]]
        inline auto noEmpty() {
            return
                [] (const std::string & value) -> std::string {
                    if (value.empty()) {
                        throw DataError(Wcs::c_rangeError); // NOLINT(*-exception-baseclass)
                    }
                    return value;
                };
        }

        [[nodiscard, maybe_unused]]
        inline auto noEmpty(Meta::Filter<std::string> auto && subFilter0) {
            return
                [subFilter = std::forward<decltype(subFilter0)>(subFilter0)]
                (const std::string & value) -> std::string {
                    std::string filtered { subFilter(value) };
                    if (filtered.empty()) {
                        throw DataError(Wcs::c_rangeError); // NOLINT(*-exception-baseclass)
                    }
                    return filtered;
                };
        }

        [[nodiscard, maybe_unused]]
        inline auto length(size_t min, size_t max) {
            assert(max > min);
            return
                [min, max] (const std::string & value) -> std::string {
                    if (value.length() < min || value.length() > max) {
                        throw DataError(Wcs::c_rangeError); // NOLINT(*-exception-baseclass)
                    }
                    return value;
                };
        }

        [[nodiscard, maybe_unused]]
        inline auto maxLength(size_t max) {
            assert(max > 0);
            return
                [max] (const std::string & value) -> std::string {
                    if (value.length() > max) {
                        throw DataError(Wcs::c_rangeError); // NOLINT(*-exception-baseclass)
                    }
                    return value;
                };
        }

        [[nodiscard, maybe_unused]]
        inline auto length(size_t min, size_t max, Meta::Filter<std::string> auto && subFilter0) {
            assert(max > min);
            return
                [min, max, subFilter = std::forward<decltype(subFilter0)>(subFilter0)]
                (const std::string & value) -> std::string {
                    std::string filtered { subFilter(value) };
                    if (filtered.length() < min || filtered.length() > max) {
                        throw DataError(Wcs::c_rangeError); // NOLINT(*-exception-baseclass)
                    }
                    return filtered;
                };
        }

        [[nodiscard, maybe_unused]]
        inline auto maxLength(size_t max, Meta::Filter<std::string> auto && subFilter0) {
            assert(max > 0);
            return
                [max, subFilter = std::forward<decltype(subFilter0)>(subFilter0)]
                (const std::string & value) -> std::string {
                    std::string filtered { subFilter(value) };
                    if (filtered.length() > max) {
                        throw DataError(Wcs::c_rangeError); // NOLINT(*-exception-baseclass)
                    }
                    return filtered;
                };
        }

        [[nodiscard, maybe_unused]]
        inline std::string_view yesNo(bool value) {
            return static_cast<bool>(value)
                ? Meta::BoolLabels<std::string_view, Meta::YesNo>::c_true
                : Meta::BoolLabels<std::string_view, Meta::YesNo>::c_false;
        }

        [[nodiscard, maybe_unused]]
        inline std::string_view enaDis(bool value) {
            return static_cast<bool>(value)
                ? Meta::BoolLabels<std::string_view, Meta::EnaDis>::c_true
                : Meta::BoolLabels<std::string_view, Meta::EnaDis>::c_false;
        }

        [[nodiscard, maybe_unused]]
        inline std::string_view trueFalse(bool value) {
            return static_cast<bool>(value)
                ? Meta::BoolLabels<std::string_view, Meta::TrueFalse>::c_true
                : Meta::BoolLabels<std::string_view, Meta::TrueFalse>::c_false;
        }
    }
}
