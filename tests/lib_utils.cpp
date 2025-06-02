// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include <catch2/catch_test_macros.hpp>
#include <library/utils.h>

using namespace std;

namespace UnitTests {
    TEST_CASE("utils", "[path_check]") {
        REQUIRE(System::isGoodPath("\\temp\\"sv));
        REQUIRE(System::isGoodPath(L"\\temp\\"sv));
        REQUIRE(System::isGoodPath("\\temp/file.ext"sv));
        REQUIRE(System::isGoodPath(L"\\temp/file.ext"sv));
        REQUIRE(System::isGoodPath("\\te mp\\"sv));
        REQUIRE(System::isGoodPath(L"\\te mp\\"sv));
        REQUIRE(System::isGoodPath("\\temp\\\\subdir//file.ext"sv));
        REQUIRE(System::isGoodPath(L"\\temp\\\\subdir//file.ext"sv));
        REQUIRE_FALSE(System::isGoodPath("\\te<mp\\"sv));
        REQUIRE_FALSE(System::isGoodPath(L"\\te<mp\\"sv));
        REQUIRE_FALSE(System::isGoodPath("\\te>mp\\file.ext"sv));
        REQUIRE_FALSE(System::isGoodPath(L"\\te>mp\\file.ext"sv));
        REQUIRE_FALSE(System::isGoodPath("\\te:mp\\"sv));
        REQUIRE_FALSE(System::isGoodPath(L"\\te:mp\\"sv));
        REQUIRE_FALSE(System::isGoodPath("\\te\"mp\\file.ext"sv));
        REQUIRE_FALSE(System::isGoodPath(L"\\te\"mp\\file.ext"sv));
        REQUIRE_FALSE(System::isGoodPath("\\te|mp\\"sv));
        REQUIRE_FALSE(System::isGoodPath(L"\\te|mp\\"sv));
        REQUIRE_FALSE(System::isGoodPath("\\te?mp\\file.ext"sv));
        REQUIRE_FALSE(System::isGoodPath(L"\\te?mp\\file.ext"sv));
        REQUIRE_FALSE(System::isGoodPath("\\te*mp\\"sv));
        REQUIRE_FALSE(System::isGoodPath(L"\\te*mp\\"sv));
        REQUIRE_FALSE(System::isGoodPath("\\te\tmp\\file.ext"sv));
        REQUIRE_FALSE(System::isGoodPath(L"\\te\tmp\\file.ext"sv));
        REQUIRE_FALSE(System::isGoodPath("\\te\nmp\\"sv));
        REQUIRE_FALSE(System::isGoodPath(L"\\te\nmp\\"sv));
        REQUIRE_FALSE(System::isGoodPath("\\te\rmp\\file.ext"sv));
        REQUIRE_FALSE(System::isGoodPath(L"\\te\rmp\\file.ext"sv));
        REQUIRE_FALSE(System::isGoodPath("\\.temp"sv));
        REQUIRE_FALSE(System::isGoodPath(L"\\.temp"sv));
        REQUIRE_FALSE(System::isGoodPath("/.temp"sv));
        REQUIRE_FALSE(System::isGoodPath(L"/.temp"sv));
        REQUIRE_FALSE(System::isGoodPath("\\abc./temp"sv));
        REQUIRE_FALSE(System::isGoodPath(L"\\abc./temp"sv));
        REQUIRE_FALSE(System::isGoodPath("/ temp"sv));
        REQUIRE_FALSE(System::isGoodPath(L"/ temp"sv));
        REQUIRE_FALSE(System::isGoodPath("\\abc /temp"sv));
        REQUIRE_FALSE(System::isGoodPath(L"\\abc /temp"sv));
        REQUIRE_FALSE(System::isGoodPath("\\ temp"sv));
        REQUIRE_FALSE(System::isGoodPath(L"\\ temp"sv));
        REQUIRE_FALSE(System::isGoodPath("\\abc \\temp"sv));
        REQUIRE_FALSE(System::isGoodPath(L"\\abc \\temp"sv));
        REQUIRE_FALSE(System::isGoodPath("\\..\\temp"sv));
        REQUIRE_FALSE(System::isGoodPath(L"\\..\\temp"sv));
        REQUIRE_FALSE(System::isGoodPath("\\abc..temp"sv));
        REQUIRE_FALSE(System::isGoodPath(L"\\abc..temp"sv));
        REQUIRE_FALSE(System::isGoodPath("\\temp .\\"sv));
        REQUIRE_FALSE(System::isGoodPath(L"\\temp .\\"sv));
        REQUIRE_FALSE(System::isGoodPath("\\temp ."sv));
        REQUIRE_FALSE(System::isGoodPath(L"\\temp ."sv));
        REQUIRE(System::isGoodPath("\\temp/file .x"sv));
        REQUIRE(System::isGoodPath(L"\\temp/file .x"sv));
        REQUIRE(System::isGoodPath("\\temp/file .ext"sv));
        REQUIRE(System::isGoodPath(L"\\temp/file .ext"sv));
    }
}
