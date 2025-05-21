// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <library/numeric.h>

using namespace std;

namespace UnitTests {
    TEST_CASE("numeric", "[cast]") {
        using Catch::Matchers::Equals;

        int16_t i16 { -123 };
        int16_t i32 { -456 };
        int16_t i64 { -789 };
        int16_t u16 { 123 };
        int16_t u32 { 456 };
        int16_t u64 { 789 };
        double pDouble { 123.456789 };
        double nDouble { -123.456789 };
        std::wstring wcStrNum;

        REQUIRE_THAT(Numeric::cast<std::string>(0), Equals("0"));
        REQUIRE_THAT(Numeric::cast<std::string>(123456789), Equals("123456789"));
        REQUIRE_THAT(Numeric::cast<std::string>(-123456789), Equals("-123456789"));
        REQUIRE_THAT(Numeric::cast<std::string>(i16), Equals("-123"));
        REQUIRE_THAT(Numeric::cast<std::string>(i32), Equals("-456"));
        REQUIRE_THAT(Numeric::cast<std::string>(i64), Equals("-789"));
        REQUIRE_THAT(Numeric::cast<std::string>(u16), Equals("123"));
        REQUIRE_THAT(Numeric::cast<std::string>(u32), Equals("456"));
        REQUIRE_THAT(Numeric::cast<std::string>(u64), Equals("789"));

        wcStrNum.assign(Numeric::cast<std::wstring>(0));
        REQUIRE(wcStrNum.compare(L"0") == 0);
        wcStrNum.assign(Numeric::cast<std::wstring>(123456789));
        REQUIRE(wcStrNum.compare(L"123456789") == 0);
        wcStrNum.assign(Numeric::cast<std::wstring>(-123456789));
        REQUIRE(wcStrNum.compare(L"-123456789") == 0);
        wcStrNum.assign(Numeric::cast<std::wstring>(i16));
        REQUIRE(wcStrNum.compare(L"-123") == 0);
        wcStrNum.assign(Numeric::cast<std::wstring>(i32));
        REQUIRE(wcStrNum.compare(L"-456") == 0);
        wcStrNum.assign(Numeric::cast<std::wstring>(i64));
        REQUIRE(wcStrNum.compare(L"-789") == 0);
        wcStrNum.assign(Numeric::cast<std::wstring>(u16));
        REQUIRE(wcStrNum.compare(L"123") == 0);
        wcStrNum.assign(Numeric::cast<std::wstring>(u32));
        REQUIRE(wcStrNum.compare(L"456") == 0);
        wcStrNum.assign(Numeric::cast<std::wstring>(u64));
        REQUIRE(wcStrNum.compare(L"789") == 0);

        REQUIRE_THAT(
            Numeric::cast<std::string>(0.5),
            Equals("0.5") || Equals("0.50") || Equals("0.500") || Equals("0.5000")
            || Equals("0.50000") || Equals("0.500000") || Equals("0.5000000")
        );
        REQUIRE_THAT(
            Numeric::cast<std::string>(-.5),
            Equals("-0.5") || Equals("-0.50") || Equals("-0.500") || Equals("-0.5000")
            || Equals("-0.50000") || Equals("-0.500000") || Equals("-0.5000000")
        );

        REQUIRE_THAT(Numeric::cast<std::string>(pDouble), Equals("123.456789"));
        REQUIRE_THAT(Numeric::cast<std::string>(nDouble), Equals("-123.456789"));

        wcStrNum.assign(Numeric::cast<std::wstring>(pDouble));
        REQUIRE(wcStrNum.compare(L"123.456789") == 0);
        wcStrNum.assign(Numeric::cast<std::wstring>(nDouble));
        REQUIRE(wcStrNum.compare(L"-123.456789") == 0);
    }
}
