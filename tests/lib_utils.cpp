// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include <catch2/catch_test_macros.hpp>
#include <library/utils.h>

using namespace std;

namespace UnitTests {
    TEST_CASE("utils", "[path_check]") {
        REQUIRE(System::isGoodPath("\\temp\\"));
        REQUIRE(System::isGoodPath(L"\\temp\\"));
        REQUIRE(System::isGoodPath("\\temp/file.ext"));
        REQUIRE(System::isGoodPath(L"\\temp/file.ext"));
        REQUIRE(System::isGoodPath("\\te mp\\"));
        REQUIRE(System::isGoodPath(L"\\te mp\\"));
        REQUIRE(System::isGoodPath("\\temp\\\\subdir//file.ext"));
        REQUIRE(System::isGoodPath(L"\\temp\\\\subdir//file.ext"));
        REQUIRE_FALSE(System::isGoodPath("\\te<mp\\"));
        REQUIRE_FALSE(System::isGoodPath(L"\\te<mp\\"));
        REQUIRE_FALSE(System::isGoodPath("\\te>mp\\file.ext"));
        REQUIRE_FALSE(System::isGoodPath(L"\\te>mp\\file.ext"));
        REQUIRE_FALSE(System::isGoodPath("\\te:mp\\"));
        REQUIRE_FALSE(System::isGoodPath(L"\\te:mp\\"));
        REQUIRE_FALSE(System::isGoodPath("\\te\"mp\\file.ext"));
        REQUIRE_FALSE(System::isGoodPath(L"\\te\"mp\\file.ext"));
        REQUIRE_FALSE(System::isGoodPath("\\te|mp\\"));
        REQUIRE_FALSE(System::isGoodPath(L"\\te|mp\\"));
        REQUIRE_FALSE(System::isGoodPath("\\te?mp\\file.ext"));
        REQUIRE_FALSE(System::isGoodPath(L"\\te?mp\\file.ext"));
        REQUIRE_FALSE(System::isGoodPath("\\te*mp\\"));
        REQUIRE_FALSE(System::isGoodPath(L"\\te*mp\\"));
        REQUIRE_FALSE(System::isGoodPath("\\te\tmp\\file.ext"));
        REQUIRE_FALSE(System::isGoodPath(L"\\te\tmp\\file.ext"));
        REQUIRE_FALSE(System::isGoodPath("\\te\nmp\\"));
        REQUIRE_FALSE(System::isGoodPath(L"\\te\nmp\\"));
        REQUIRE_FALSE(System::isGoodPath("\\te\rmp\\file.ext"));
        REQUIRE_FALSE(System::isGoodPath(L"\\te\rmp\\file.ext"));
        REQUIRE_FALSE(System::isGoodPath("\\.temp"));
        REQUIRE_FALSE(System::isGoodPath(L"\\.temp"));
        REQUIRE_FALSE(System::isGoodPath("/.temp"));
        REQUIRE_FALSE(System::isGoodPath(L"/.temp"));
        REQUIRE_FALSE(System::isGoodPath("\\abc./temp"));
        REQUIRE_FALSE(System::isGoodPath(L"\\abc./temp"));
        REQUIRE_FALSE(System::isGoodPath("/ temp"));
        REQUIRE_FALSE(System::isGoodPath(L"/ temp"));
        REQUIRE_FALSE(System::isGoodPath("\\abc /temp"));
        REQUIRE_FALSE(System::isGoodPath(L"\\abc /temp"));
        REQUIRE_FALSE(System::isGoodPath("\\ temp"));
        REQUIRE_FALSE(System::isGoodPath(L"\\ temp"));
        REQUIRE_FALSE(System::isGoodPath("\\abc \\temp"));
        REQUIRE_FALSE(System::isGoodPath(L"\\abc \\temp"));
        REQUIRE_FALSE(System::isGoodPath("\\..\\temp"));
        REQUIRE_FALSE(System::isGoodPath(L"\\..\\temp"));
        REQUIRE_FALSE(System::isGoodPath("\\abc..temp"));
        REQUIRE_FALSE(System::isGoodPath(L"\\abc..temp"));
        REQUIRE_FALSE(System::isGoodPath("\\temp .\\"));
        REQUIRE_FALSE(System::isGoodPath(L"\\temp .\\"));
        REQUIRE_FALSE(System::isGoodPath("\\temp ."));
        REQUIRE_FALSE(System::isGoodPath(L"\\temp ."));
        REQUIRE(System::isGoodPath("\\temp/file .x"));
        REQUIRE(System::isGoodPath(L"\\temp/file .x"));
        REQUIRE(System::isGoodPath("\\temp/file .ext"));
        REQUIRE(System::isGoodPath(L"\\temp/file .ext"));
    }
}
