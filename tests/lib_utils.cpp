// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include <catch2/catch_test_macros.hpp>
#include <lib/utils.h>

namespace UnitTests {
    using namespace std::string_view_literals;

    static int s_var { 987 };

    void modifyVar() {
        s_var = 654;
    }

    struct Invocable {
        inline static int s_var { 1000 };

        static void modifyStaticVar() {
            ++s_var;
        }

        int m_var { 2000 };

        void operator()() {
            ++m_var;
        }
    };

    TEST_CASE("utils", "[deferred]") {
        {
            int var { 123 };

            {
                Deferred::Exec scopeGuard([&var] () { var = 456; });
                REQUIRE(var == 123);
            }

            REQUIRE(var == 456);

            {
                Deferred::Exec scopeGuard(&modifyVar);
                REQUIRE(s_var == 987);
            }

            REQUIRE(s_var == 654);

            {
                Deferred::Exec scopeGuard(&Invocable::modifyStaticVar);
                REQUIRE(Invocable::s_var == 1000);
            }

            {
                Deferred::Exec scopeGuard(&Invocable::modifyStaticVar);
                REQUIRE(Invocable::s_var == 1001);
                scopeGuard.perform(true);
                REQUIRE(Invocable::s_var == 1002);
                scopeGuard.perform();
                REQUIRE(Invocable::s_var == 1003);
            }

            {
                Deferred::Exec scopeGuard(&Invocable::modifyStaticVar);
                REQUIRE(Invocable::s_var == 1003);
                scopeGuard.cancel();
                REQUIRE(Invocable::s_var == 1003);
            }

            {
                Deferred::Exec scopeGuard(&Invocable::modifyStaticVar);
                REQUIRE(Invocable::s_var == 1003);
                scopeGuard.perform();
                REQUIRE(Invocable::s_var == 1004);
            }

            REQUIRE(Invocable::s_var == 1004);

            Invocable invocable {};

            {
                Deferred::Exec scopeGuard(invocable);
                REQUIRE(invocable.m_var == 2000);
            }

            REQUIRE(invocable.m_var == 2000);

            {
                Deferred::Exec scopeGuard(std::ref(invocable));
                REQUIRE(invocable.m_var == 2000);
                scopeGuard.perform(true);
                REQUIRE(invocable.m_var == 2001);
            }

            REQUIRE(invocable.m_var == 2002);
        }

        {
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
