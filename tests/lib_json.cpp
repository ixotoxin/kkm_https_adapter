// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <library/json.h>

using namespace std;

// FIXME: Реализовать полноценное тестирование.

namespace UnitTests {
    nlohmann::json null_j;
    nlohmann::json bool_j(true);
    nlohmann::json int_j(-123);
    nlohmann::json dbl_j(-123.00);
    nlohmann::json str1_j("yes");
    nlohmann::json str2_j("-123");

    nlohmann::json obj_j(
        "{\"null\": null, \"bool\": true, \"int\": -123, \"dbl\": -123.000, \"str1\": \"yes\", \"str2\": -123}"_json
    );

    TEST_CASE("json", "[cast]") {
        using Catch::Matchers::WithinRel;

        REQUIRE(Json::cast<bool>(bool_j) == true);
        REQUIRE(Json::cast<bool>(int_j) == true);
        REQUIRE(Json::cast<bool>(dbl_j) == true);
        REQUIRE(Json::cast<bool>(str1_j) == true);
        REQUIRE_THROWS(Json::cast<bool>(str2_j));

        REQUIRE(Json::cast<int>(bool_j) == static_cast<int>(true));
        REQUIRE(Json::cast<int>(int_j) == -123);
        REQUIRE(Json::cast<int>(dbl_j) == -123);
        REQUIRE_THROWS(Json::cast<int>(str1_j));
        REQUIRE(Json::cast<int>(str2_j) == -123);

        REQUIRE(Json::cast<std::wstring>(bool_j) == L"yes");
        REQUIRE(Json::cast<std::wstring>(int_j) == L"-123");
        REQUIRE_THAT(Text::cast<double>(Json::cast<std::wstring>(dbl_j)), WithinRel(-123.0, 0.000001));
        REQUIRE(Json::cast<std::wstring>(str1_j) == L"yes");
        REQUIRE(Json::cast<std::wstring>(str2_j) == L"-123");
    }

    TEST_CASE("json", "[handle]") {
        bool bool_v { false };

        REQUIRE(
            Json::handle(
                bool_j,
                [& bool_v] (nlohmann::json v, std::wstring) {
                    bool_v = Json::cast<bool>(bool_j);
                    return true;
                }
            )
        );
        REQUIRE(bool_v);
    }

    TEST_CASE("json", "[handleKey]") {
        bool bool_v { false };

        REQUIRE_FALSE(
            Json::handleKey(
                obj_j,
                "--bool",
                [& bool_v] (nlohmann::json v, std::wstring) {
                    bool_v = Json::cast<bool>(bool_j);
                    return true;
                }
            )
        );
        REQUIRE(
            Json::handleKey(
                obj_j,
                "bool",
                [& bool_v] (nlohmann::json v, std::wstring) {
                    bool_v = Json::cast<bool>(bool_j);
                    return true;
                }
            )
        );
        REQUIRE(bool_v);
    }
}
