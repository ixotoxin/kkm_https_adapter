// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include <catch2/catch_test_macros.hpp>
#include <library/wconv.h>

using namespace std;

namespace UnitTests {
    TEST_CASE("wconv", "[convert]") {
        std::wstring wcString;
        std::string mbString;
        REQUIRE(Basic::convert(mbString, L""));
        REQUIRE(mbString.empty());
        REQUIRE(Basic::convert(wcString, ""));
        REQUIRE(wcString.empty());
        REQUIRE(Basic::convert(mbString, L"Test string"));
        REQUIRE(mbString == "Test string");
        REQUIRE(Basic::convert(wcString, "Test string"));
        REQUIRE(wcString == L"Test string");
        REQUIRE(Basic::convert(mbString, L"Тестовая строка"));
        REQUIRE(mbString == "Тестовая строка");
        REQUIRE(Basic::convert(wcString, "Тестовая строка"));
        REQUIRE(wcString == L"Тестовая строка");
        REQUIRE(Basic::convert(L"").empty());
        REQUIRE(Basic::convert("").empty());
        REQUIRE(Basic::convert(L"Test string") == "Test string");
        REQUIRE(Basic::convert("Test string") == L"Test string");
        REQUIRE(Basic::convert(L"Тестовая строка") == "Тестовая строка");
        REQUIRE(Basic::convert("Тестовая строка") == L"Тестовая строка");
    }
}
