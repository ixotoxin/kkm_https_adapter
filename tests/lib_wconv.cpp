// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include <catch2/catch_test_macros.hpp>
#include <library/wconv.h>

namespace UnitTests {
    using namespace std::string_view_literals;

    TEST_CASE("wconv", "[convert]") {
        std::wstring wcString;
        std::string mbString;

        REQUIRE(Text::convert(mbString, L""sv));
        REQUIRE(mbString.empty());
        REQUIRE(Text::convert(wcString, ""sv));
        REQUIRE(wcString.empty());
        REQUIRE(Text::convert(mbString, L"Test string"sv));
        REQUIRE(mbString == "Test string"sv);
        REQUIRE(Text::convert(wcString, "Test string"sv));
        REQUIRE(wcString == L"Test string"sv);
        REQUIRE(Text::convert(mbString, L"Тестовая строка"sv));
        REQUIRE(mbString == "Тестовая строка"sv);
        REQUIRE(Text::convert(wcString, "Тестовая строка"sv));
        REQUIRE(wcString == L"Тестовая строка"sv);
        REQUIRE(Text::convert(L"").empty());
        REQUIRE(Text::convert("").empty());
        REQUIRE(Text::convert(L"Test string") == "Test string"sv);
        REQUIRE(Text::convert("Test string") == L"Test string"sv);
        REQUIRE(Text::convert(L"Тестовая строка") == "Тестовая строка"sv);
        REQUIRE(Text::convert("Тестовая строка") == L"Тестовая строка"sv);

        std::wstring_view wcString0 {
            L"\r\n\t\n\v\f !\"#$%&'()*+,-./0123456789:;<=>?@[\\]^_`{|}~"
            L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
            L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя"
        };

        std::string_view mbString0 {
            "\r\n\t\n\v\f !\"#$%&'()*+,-./0123456789:;<=>?@[\\]^_`{|}~"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
            "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя"
        };

        REQUIRE(Text::convert(mbString, wcString0));
        REQUIRE(mbString0 == mbString);
        REQUIRE(Text::convert(wcString, mbString0));
        REQUIRE(wcString0 == wcString);
        REQUIRE(Text::convert(wcString0) == mbString0);
        REQUIRE(Text::convert(mbString0) == wcString0);
    }
}
