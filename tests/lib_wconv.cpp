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
        REQUIRE(mbString.compare("") == 0);
        REQUIRE(Basic::convert(wcString, ""));
        REQUIRE(wcString.empty());
        REQUIRE(wcString.compare(L"") == 0);
        REQUIRE(Basic::convert(mbString, L"Test string"));
        REQUIRE(mbString.compare("Test string") == 0);
        REQUIRE(Basic::convert(wcString, "Test string"));
        REQUIRE(wcString.compare(L"Test string") == 0);
        REQUIRE(Basic::convert(mbString, L"Тестовая строка"));
        REQUIRE(mbString.compare("Тестовая строка") == 0);
        REQUIRE(Basic::convert(wcString, "Тестовая строка"));
        REQUIRE(wcString.compare(L"Тестовая строка") == 0);
        REQUIRE(Basic::convert(L"").empty());
        REQUIRE(Basic::convert("").empty());
        REQUIRE(Basic::convert(L"").compare("") == 0);
        REQUIRE(Basic::convert("").compare(L"") == 0);
        REQUIRE(Basic::convert(L"Test string").compare("Test string") == 0);
        REQUIRE(Basic::convert("Test string").compare(L"Test string") == 0);
        REQUIRE(Basic::convert(L"Тестовая строка").compare("Тестовая строка") == 0);
        REQUIRE(Basic::convert("Тестовая строка").compare(L"Тестовая строка") == 0);
    }
}
