// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <library/datetime.h>

using namespace std;

namespace UnitTests {
    TEST_CASE("datetime", "[cast]") {
        using Catch::Matchers::Equals;

        std::tm dateTime {
            .tm_sec = 1, .tm_min = 2, .tm_hour = 3,
            .tm_mday = 4, .tm_mon = 4, .tm_year = 106,
            .tm_wday = 4, .tm_yday = 124, .tm_isdst = 0
        };

        REQUIRE_THAT(DateTime::cast<std::string>(dateTime), Equals("2006-05-04 03:02:01"));
        REQUIRE(DateTime::cast<std::wstring>(dateTime) == L"2006-05-04 03:02:01");
    }
}
