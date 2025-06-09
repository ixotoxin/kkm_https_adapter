// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include <catch2/catch_test_macros.hpp>
#include <library/utils.h>

using namespace std;

namespace UnitTests {
    TEST_CASE("utils", "[deferred]") {
        int var { 123 };
        {
            Deferred::Exec scopeGuard([&var] () { var = 456; });
            REQUIRE(var == 123);
        }
        bool except { false };
        {
            Deferred::Exec scopeGuard([&var] () { var = 789; throw std::runtime_error("Error"); }, &except);
            var = 321;
        }
        REQUIRE(var == 789);
        REQUIRE(except);
        char * buffer { nullptr };
        {
            Deferred::LocalFree scopeGuard(buffer);
            REQUIRE(buffer == nullptr);
            buffer = (char *)::LocalAlloc(LPTR, 2048);
            bool zeroed { true };
            for (size_t i {}; i < 2048; ++i) {
                zeroed = zeroed && !buffer[i];
            }
            REQUIRE(buffer != nullptr);
            REQUIRE(zeroed);
        }
        REQUIRE(buffer == nullptr);
    }

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
