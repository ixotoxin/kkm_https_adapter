// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include <vector>
#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <library/text.h>

using namespace std;

namespace UnitTests {
    template<typename T>
    requires (is_same_v<T, char> || is_same_v<T, wchar_t>)
    struct charTraitSource {
        T m_char;
        T m_lowered;
        bool m_trimmable;
    };

    static vector<charTraitSource<wchar_t>> wcTraitSource {};
    static vector<charTraitSource<char>> mbTraitSource {};

    TEST_CASE("text", "[wcs_trait_functions]") {
        using T = Meta::Wcs;
        for (auto & item: wcTraitSource) {
            REQUIRE(T::trimPredicate(item.m_char) == item.m_trimmable);
            REQUIRE(T::noTrimPredicate(item.m_char) != item.m_trimmable);
            REQUIRE(T::toLower(item.m_char) == item.m_lowered);
        }
    }

    TEST_CASE("text", "[mbs_trait_functions]" ) {
        using T = Meta::Mbs;
        for (auto & item: mbTraitSource) {
            REQUIRE(T::trimPredicate(item.m_char) == item.m_trimmable);
            REQUIRE(T::noTrimPredicate(item.m_char) != item.m_trimmable);
            REQUIRE(T::toLower(item.m_char) == item.m_lowered);
        }
    }

    // TEST(LibText, trim) {
    //     // TODO: Реализовать.
    //     SUCCEED();
    // }
    //
    // TEST(LibText, lower) {
    //     // TODO: Реализовать.
    // }
    //
    // TEST(LibText, cast) {
    //     // TODO: Реализовать.
    //     SUCCEED();
    // }
    //
    // TEST(LibText, convert) {
    //     // TODO: Реализовать.
    //     SUCCEED();
    // }
    //
    // TEST(LibText, splitTo) {
    //     // TODO: Реализовать.
    //     SUCCEED();
    // }
    //
    // TEST(LibText, splitVariable) {
    //     // TODO: Реализовать.
    //     SUCCEED();
    // }
    //
    // TEST(LibText, joinTo) {
    //     // TODO: Реализовать.
    //     SUCCEED();
    // }
    //
    // TEST(LibText, concat) {
    //     // TODO: Реализовать.
    //     SUCCEED();
    // }

    TEST_CASE("text", "[bool_to_str_cast]" ) {
        REQUIRE(Text::Wcs::yesNo(true).compare(L"yes") == 0);
        REQUIRE(Text::Mbs::yesNo(true).compare("yes") == 0);
        REQUIRE(Text::yesNo<Meta::Wcs>(true).compare(L"yes") == 0);
        REQUIRE(Text::yesNo<Meta::Mbs>(true).compare("yes") == 0);
        REQUIRE(Text::Wcs::yesNo(false).compare(L"no") == 0);
        REQUIRE(Text::Mbs::yesNo(false).compare("no") == 0);
        REQUIRE(Text::yesNo<Meta::Wcs>(false).compare(L"no") == 0);
        REQUIRE(Text::yesNo<Meta::Mbs>(false).compare("no") == 0);
        REQUIRE(Text::Wcs::enaDis(true).compare(L"enable") == 0);
        REQUIRE(Text::Mbs::enaDis(true).compare("enable") == 0);
        REQUIRE(Text::enaDis<Meta::Wcs>(true).compare(L"enable") == 0);
        REQUIRE(Text::enaDis<Meta::Mbs>(true).compare("enable") == 0);
        REQUIRE(Text::Wcs::enaDis(false).compare(L"disable") == 0);
        REQUIRE(Text::Mbs::enaDis(false).compare("disable") == 0);
        REQUIRE(Text::enaDis<Meta::Wcs>(false).compare(L"disable") == 0);
        REQUIRE(Text::enaDis<Meta::Mbs>(false).compare("disable") == 0);
        REQUIRE(Text::Wcs::trueFalse(true).compare(L"true") == 0);
        REQUIRE(Text::Mbs::trueFalse(true).compare("true") == 0);
        REQUIRE(Text::trueFalse<Meta::Wcs>(true).compare(L"true") == 0);
        REQUIRE(Text::trueFalse<Meta::Mbs>(true).compare("true") == 0);
        REQUIRE(Text::Wcs::trueFalse(false).compare(L"false") == 0);
        REQUIRE(Text::Mbs::trueFalse(false).compare("false") == 0);
        REQUIRE(Text::trueFalse<Meta::Wcs>(false).compare(L"false") == 0);
        REQUIRE(Text::trueFalse<Meta::Mbs>(false).compare("false") == 0);
    }
}

int main(int argc, char ** argv) {
    for (int i = 0; i < 256; ++i) {
        if ((i >= 'A' && i <= 'Z') || (i >= 'a' && i <= 'z')) {
            continue;
        }
        UnitTests::wcTraitSource.emplace_back(static_cast<wchar_t>(i), static_cast<wchar_t>(i), std::isspace(i));
        UnitTests::mbTraitSource.emplace_back(static_cast<char>(i), static_cast<char>(i), std::isspace(i));
    }
    constexpr std::wstring_view wcEnLetters { L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz" };
    constexpr std::wstring_view wcEnLettersLc { L"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz" };
    constexpr std::string_view mbEnLetters { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz" };
    constexpr std::string_view mbEnLettersLc { "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz" };
    assert(wcEnLetters.size() == wcEnLettersLc.size());
    assert(wcEnLetters.size() == mbEnLetters.size());
    assert(mbEnLetters.size() == mbEnLettersLc.size());
    for (int i = 0; i < wcEnLetters.size(); ++i) {
        UnitTests::wcTraitSource.emplace_back(wcEnLetters[i], wcEnLettersLc[i], false);
        UnitTests::mbTraitSource.emplace_back(mbEnLetters[i], mbEnLettersLc[i], false);
    }
    constexpr std::wstring_view wcRuLetters { L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя" };
    for (auto letter : wcRuLetters) {
        UnitTests::wcTraitSource.emplace_back(letter, letter, false);
    }
    constexpr std::string_view mbRuLetters { "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя" };
    for (auto letter : mbRuLetters) {
        UnitTests::mbTraitSource.emplace_back(letter, letter, false);
    }
    return Catch::Session().run(argc, argv);
}
