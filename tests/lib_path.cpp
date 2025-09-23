// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include <catch2/catch_test_macros.hpp>
#include <lib/path.h>

namespace UnitTests {
    using namespace std::string_view_literals;

    TEST_CASE("path", "[test_path]") {
        REQUIRE(Path::isGood("\\temp\\"sv));
        REQUIRE(Path::isGood(L"\\temp\\"sv));
        REQUIRE(Path::isGood("\\temp/file.ext"sv));
        REQUIRE(Path::isGood(L"\\temp/file.ext"sv));
        REQUIRE(Path::isGood("\\te mp\\"sv));
        REQUIRE(Path::isGood(L"\\te mp\\"sv));
        REQUIRE(Path::isGood("\\temp\\\\subdir//file.ext"sv));
        REQUIRE(Path::isGood(L"\\temp\\\\subdir//file.ext"sv));
        REQUIRE_FALSE(Path::isGood("\\te<mp\\"sv));
        REQUIRE_FALSE(Path::isGood(L"\\te<mp\\"sv));
        REQUIRE_FALSE(Path::isGood("\\te>mp\\file.ext"sv));
        REQUIRE_FALSE(Path::isGood(L"\\te>mp\\file.ext"sv));
        REQUIRE_FALSE(Path::isGood("\\te:mp\\"sv));
        REQUIRE_FALSE(Path::isGood(L"\\te:mp\\"sv));
        REQUIRE_FALSE(Path::isGood("\\te\"mp\\file.ext"sv));
        REQUIRE_FALSE(Path::isGood(L"\\te\"mp\\file.ext"sv));
        REQUIRE_FALSE(Path::isGood("\\te|mp\\"sv));
        REQUIRE_FALSE(Path::isGood(L"\\te|mp\\"sv));
        REQUIRE_FALSE(Path::isGood("\\te?mp\\file.ext"sv));
        REQUIRE_FALSE(Path::isGood(L"\\te?mp\\file.ext"sv));
        REQUIRE_FALSE(Path::isGood("\\te*mp\\"sv));
        REQUIRE_FALSE(Path::isGood(L"\\te*mp\\"sv));
        REQUIRE_FALSE(Path::isGood("\\te\tmp\\file.ext"sv));
        REQUIRE_FALSE(Path::isGood(L"\\te\tmp\\file.ext"sv));
        REQUIRE_FALSE(Path::isGood("\\te\nmp\\"sv));
        REQUIRE_FALSE(Path::isGood(L"\\te\nmp\\"sv));
        REQUIRE_FALSE(Path::isGood("\\te\rmp\\file.ext"sv));
        REQUIRE_FALSE(Path::isGood(L"\\te\rmp\\file.ext"sv));
        REQUIRE_FALSE(Path::isGood("\\.temp"sv));
        REQUIRE_FALSE(Path::isGood(L"\\.temp"sv));
        REQUIRE_FALSE(Path::isGood("/.temp"sv));
        REQUIRE_FALSE(Path::isGood(L"/.temp"sv));
        REQUIRE_FALSE(Path::isGood("\\abc./temp"sv));
        REQUIRE_FALSE(Path::isGood(L"\\abc./temp"sv));
        REQUIRE_FALSE(Path::isGood("/ temp"sv));
        REQUIRE_FALSE(Path::isGood(L"/ temp"sv));
        REQUIRE_FALSE(Path::isGood("\\abc /temp"sv));
        REQUIRE_FALSE(Path::isGood(L"\\abc /temp"sv));
        REQUIRE_FALSE(Path::isGood("\\ temp"sv));
        REQUIRE_FALSE(Path::isGood(L"\\ temp"sv));
        REQUIRE_FALSE(Path::isGood("\\abc \\temp"sv));
        REQUIRE_FALSE(Path::isGood(L"\\abc \\temp"sv));
        REQUIRE_FALSE(Path::isGood("\\..\\temp"sv));
        REQUIRE_FALSE(Path::isGood(L"\\..\\temp"sv));
        REQUIRE_FALSE(Path::isGood("\\abc..temp"sv));
        REQUIRE_FALSE(Path::isGood(L"\\abc..temp"sv));
        REQUIRE_FALSE(Path::isGood("\\temp .\\"sv));
        REQUIRE_FALSE(Path::isGood(L"\\temp .\\"sv));
        REQUIRE_FALSE(Path::isGood("\\temp ."sv));
        REQUIRE_FALSE(Path::isGood(L"\\temp ."sv));
        REQUIRE(Path::isGood("\\temp/file .x"sv));
        REQUIRE(Path::isGood(L"\\temp/file .x"sv));
        REQUIRE(Path::isGood("\\temp/file .ext"sv));
        REQUIRE(Path::isGood(L"\\temp/file .ext"sv));
    }

    TEST_CASE("path", "[test_name]") {
        REQUIRE(Path::isGoodFileName("file"sv));
        REQUIRE(Path::isGoodFileName(L"file"sv));
        REQUIRE(Path::isGoodFileName("file.ext"sv));
        REQUIRE(Path::isGoodFileName(L"file.ext"sv));
        REQUIRE(Path::isGoodFileName("fi le"sv));
        REQUIRE(Path::isGoodFileName(L"fi le"sv));
        REQUIRE(Path::isGoodFileName("!file.ext'"sv));
        REQUIRE(Path::isGoodFileName(L"!file.ext'"sv));
        REQUIRE_FALSE(Path::isGoodFileName(""sv));
        REQUIRE_FALSE(Path::isGoodFileName(L""sv));
        REQUIRE_FALSE(Path::isGoodFileName("\\"sv));
        REQUIRE_FALSE(Path::isGoodFileName(L"\\"sv));
        REQUIRE_FALSE(Path::isGoodFileName("/"sv));
        REQUIRE_FALSE(Path::isGoodFileName(L"/"sv));
        REQUIRE_FALSE(Path::isGoodFileName("fi|le"sv));
        REQUIRE_FALSE(Path::isGoodFileName(L"fi|le"sv));
        REQUIRE_FALSE(Path::isGoodFileName(".file.ext"sv));
        REQUIRE_FALSE(Path::isGoodFileName(L".file.ext"sv));
        REQUIRE_FALSE(Path::isGoodFileName("file."sv));
        REQUIRE_FALSE(Path::isGoodFileName(L"file."sv));
        REQUIRE_FALSE(Path::isGoodFileName(" file.ext"sv));
        REQUIRE_FALSE(Path::isGoodFileName(L" file.ext"sv));
        REQUIRE_FALSE(Path::isGoodFileName("file.ext "sv));
        REQUIRE_FALSE(Path::isGoodFileName(L"file.ext "sv));
    }
}
