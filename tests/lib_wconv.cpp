// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include <catch2/catch_test_macros.hpp>
#include <library/wconv.h>

using namespace std;

namespace UnitTests {
    TEST_CASE("wconv", "[convert]") {
        std::wstring wcString;
        std::string mbString;
        REQUIRE(Text::convert(mbString, L""));
        REQUIRE(mbString.empty());
        REQUIRE(Text::convert(wcString, ""));
        REQUIRE(wcString.empty());
        REQUIRE(Text::convert(mbString, L"Test string"));
        REQUIRE(mbString == "Test string");
        REQUIRE(Text::convert(wcString, "Test string"));
        REQUIRE(wcString == L"Test string");
        REQUIRE(Text::convert(mbString, L"Тестовая строка"));
        REQUIRE(mbString == "Тестовая строка");
        REQUIRE(Text::convert(wcString, "Тестовая строка"));
        REQUIRE(wcString == L"Тестовая строка");
        REQUIRE(Text::convert(L"").empty());
        REQUIRE(Text::convert("").empty());
        REQUIRE(Text::convert(L"Test string") == "Test string");
        REQUIRE(Text::convert("Test string") == L"Test string");
        REQUIRE(Text::convert(L"Тестовая строка") == "Тестовая строка");
        REQUIRE(Text::convert("Тестовая строка") == L"Тестовая строка");
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
