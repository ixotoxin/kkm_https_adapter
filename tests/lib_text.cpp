// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include <cwctype>
#include <vector>
#include <deque>
#include <list>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <library/text.h>

namespace UnitTests {
    using namespace std::string_literals;
    using namespace std::string_view_literals;

    TEST_CASE("text", "[trait]") {
        for (int ch = 0; ch < 128; ++ch) {
            REQUIRE(Meta::Wcs::trimPredicate(static_cast<wchar_t>(ch)) == static_cast<bool>(std::iswspace(ch)));
            REQUIRE(Meta::Wcs::noTrimPredicate(static_cast<wchar_t>(ch)) != static_cast<bool>(std::iswspace(ch)));
            REQUIRE(Meta::Wcs::toLower(static_cast<wchar_t>(ch)) == std::towlower(ch));
        }

        for (auto ch : L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя"sv) {
            REQUIRE(Meta::Wcs::toLower(ch) == std::towlower(ch));
        }

        for (int ch = 0; ch < 128; ++ch) {
            REQUIRE(Meta::Mbs::trimPredicate(static_cast<char>(ch)) == static_cast<bool>(std::isspace(ch)));
            REQUIRE(Meta::Mbs::noTrimPredicate(static_cast<char>(ch)) != static_cast<bool>(std::isspace(ch)));
            REQUIRE(Meta::Mbs::toLower(static_cast<char>(ch)) == std::tolower(ch));
        }

        for (auto ch : "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя"sv) {
            REQUIRE(Meta::Mbs::toLower(ch) == std::tolower(ch));
        }
    }

    TEST_CASE("text", "[trim]" ) {
        {
            std::unordered_map<std::wstring_view, std::wstring_view> strings {
                { L"", L"" },
                { L"\n", L"" },
                { L"\r", L"" },
                { L"\t", L"" },
                { L"\v", L"" },
                { L"\f", L"" },
                { L" ", L"" },
                { L"\0", L"" },
                { L"\r\n\t\n\v\f ", L"" },
                { L"Te\r\n\t\n\v\f xt", L"Te\r\n\t\n\v\f xt" },
                { L"\r\n\t\n\v\f Text", L"Text" },
                { L"Text\r\n\t\n\v\f ", L"Text" }
            };

            for (const auto & s : strings) {
                {
                    auto t = new wchar_t[s.first.size() + 1] {};
                    if (!s.first.empty()) {
                        std::wmemcpy(t, s.first.data(), s.first.size());
                    }
                    auto u = Text::trimmed<wchar_t>(t);
                    auto v = Text::trimmed<std::wstring_view>(t);
                    auto w = Text::trimmed<std::wstring>(t);
                    auto r = Text::trim<wchar_t>(t);

                    REQUIRE(s.second == u);
                    REQUIRE(s.second == v);
                    REQUIRE(s.second == w);
                    REQUIRE(s.second == r);

                    delete[] t;
                }

                {
                    std::wstring t { s.first };
                    Text::trim<std::wstring>(t);

                    REQUIRE(s.second == t);
                }
            }
        }

        {
            std::unordered_map<std::string_view, std::string_view> strings {
                { "", "" },
                { "\n", "" },
                { "\r", "" },
                { "\t", "" },
                { "\v", "" },
                { "\f", "" },
                { " ", "" },
                { "\0", "" },
                { "\r\n\t\n\v\f ", "" },
                { "Te\r\n\t\n\v\f xt", "Te\r\n\t\n\v\f xt" },
                { "\r\n\t\n\v\f Text", "Text" },
                { "Text\r\n\t\n\v\f ", "Text" }
            };

            for (const auto & s : strings) {
                {
                    auto t = new char[s.first.size() + 1] {};
                    if (!s.first.empty()) {
                        std::memcpy(t, s.first.data(), s.first.size());
                    }
                    auto u = Text::trimmed<char>(t);
                    auto v = Text::trimmed<std::string_view>(t);
                    auto w = Text::trimmed<std::string>(t);
                    auto r = Text::trim<char>(t);

                    REQUIRE(s.second == u);
                    REQUIRE(s.second == v);
                    REQUIRE(s.second == w);
                    REQUIRE(s.second == r);

                    delete[] t;
                }

                {
                    std::string t { s.first };
                    Text::trim<std::string>(t);

                    REQUIRE(s.second == t);
                }
            }
        }
    }

    TEST_CASE("text", "[lower]" ) {
        {
            std::wstring_view sv {
                L"\r\n\t\n\v\f !\"#$%&'()*+,-./0123456789:;<=>?@[\\]^_`{|}~"
                L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
                L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя"
            };

            std::wstring_view lv {
                L"\r\n\t\n\v\f !\"#$%&'()*+,-./0123456789:;<=>?@[\\]^_`{|}~"
                L"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
                L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя"
            };

            {
                auto t = new wchar_t[sv.size() + 1] {};
                std::wmemcpy(t, sv.data(), sv.size());
                auto u = Text::lowered<wchar_t>(t);
                auto v = Text::lowered<std::wstring_view>(t);
                auto w = Text::lowered<std::wstring>(t);
                auto r = Text::lower<wchar_t>(t);

                REQUIRE(lv == u);
                REQUIRE(lv == v);
                REQUIRE(lv == w);
                REQUIRE(lv == r);

                delete[] t;
            }

            {
                std::wstring t { sv };
                Text::lower<std::wstring>(t);

                REQUIRE(lv == t);
            }

            {
                std::wstring t { L"Z" };
                Text::lower<wchar_t>(const_cast<wchar_t *>(t.c_str()));

                REQUIRE(t == L"z");
            }
        }

        {
            std::string_view sv {
                "\r\n\t\n\v\f !\"#$%&'()*+,-./0123456789:;<=>?@[\\]^_`{|}~"
                "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
                "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя"
            };

            std::string_view lv {
                "\r\n\t\n\v\f !\"#$%&'()*+,-./0123456789:;<=>?@[\\]^_`{|}~"
                "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
                "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя"
            };

            {
                auto t = new char[sv.size() + 1] {};
                std::memcpy(t, sv.data(), sv.size());
                auto u = Text::lowered<char>(t);
                auto v = Text::lowered<std::string_view>(t);
                auto w = Text::lowered<std::string>(t);
                auto r = Text::lower<char>(t);

                REQUIRE(lv == u);
                REQUIRE(lv == v);
                REQUIRE(lv == w);
                REQUIRE(lv == r);

                delete[] t;
            }

            {
                std::string t { sv };
                Text::lower<std::string>(t);

                REQUIRE(lv == t);
            }

            {
                std::string t { "Z" };
                Text::lower<char>(const_cast<char *>(t.c_str()));

                REQUIRE(t == "z");
            }
        }
    }

    TEST_CASE("text", "[cast]" ) {
        using Catch::Matchers::WithinRel;

        {
            REQUIRE_THROWS(Text::cast<bool>(L""));
            REQUIRE(Text::cast<bool>(L"yes"));
            REQUIRE_FALSE(Text::cast<bool>(L"no"));
            REQUIRE_THROWS(Text::cast<bool>(L"xyz"));

            REQUIRE_THROWS(Text::cast<int>(L""));
            REQUIRE(Text::cast<int>(L"123") == 123);
            REQUIRE(Text::cast<int>(L"-123") == -123);
            REQUIRE(Text::cast<int>(L"  123") == 123);
            REQUIRE_THROWS(Text::cast<int>(L"123 "));
            REQUIRE_THROWS(Text::cast<int>(L"12z"));

            REQUIRE_THROWS(Text::cast<unsigned int>(L""));
            REQUIRE(Text::cast<unsigned int>(L"123") == 123);
            REQUIRE_THROWS(Text::cast<unsigned int>(L"-123"));
            REQUIRE(Text::cast<unsigned int>(L"  123") == 123);
            REQUIRE_THROWS(Text::cast<unsigned int>(L"123 "));
            REQUIRE_THROWS(Text::cast<unsigned int>(L"12z"));

            REQUIRE_THROWS(Text::cast<long long>(L""));
            REQUIRE(Text::cast<long long>(L"123") == 123);
            REQUIRE(Text::cast<long long>(L"-123") == -123);
            REQUIRE(Text::cast<long long>(L"  123") == 123);
            REQUIRE_THROWS(Text::cast<long long>(L"123 "));
            REQUIRE_THROWS(Text::cast<long long>(L"12z"));

            REQUIRE_THROWS(Text::cast<unsigned long long>(L""));
            REQUIRE(Text::cast<unsigned long long>(L"123") == 123);
            REQUIRE_THROWS(Text::cast<unsigned long long>(L"-123"));
            REQUIRE(Text::cast<unsigned long long>(L"  123") == 123);
            REQUIRE_THROWS(Text::cast<unsigned long long>(L"123 "));
            REQUIRE_THROWS(Text::cast<unsigned long long>(L"12z"));

            REQUIRE_THROWS(Text::cast<double>(L""));
            REQUIRE_THAT(Text::cast<double>(L"123.45"), WithinRel(123.45, 0.001));
            REQUIRE_THAT(Text::cast<double>(L"-123.45"), WithinRel(-123.45, 0.001));
            REQUIRE_THAT(Text::cast<double>(L"  123.45"), WithinRel(123.45, 0.001));
            REQUIRE_THROWS(Text::cast<double>(L"123.45 "));
            REQUIRE_THROWS(Text::cast<double>(L"12z"));
        }

        {
            REQUIRE_THROWS(Text::cast<bool>(""));
            REQUIRE(Text::cast<bool>("yes"));
            REQUIRE_FALSE(Text::cast<bool>("no"));
            REQUIRE_THROWS(Text::cast<bool>("xyz"));

            REQUIRE_THROWS(Text::cast<int>(""));
            REQUIRE(Text::cast<int>("123") == 123);
            REQUIRE(Text::cast<int>("-123") == -123);
            REQUIRE(Text::cast<int>("  123") == 123);
            REQUIRE_THROWS(Text::cast<int>("123 "));
            REQUIRE_THROWS(Text::cast<int>("12z"));

            REQUIRE_THROWS(Text::cast<unsigned int>(""));
            REQUIRE(Text::cast<unsigned int>("123") == 123);
            REQUIRE_THROWS(Text::cast<unsigned int>("-123"));
            REQUIRE(Text::cast<unsigned int>("  123") == 123);
            REQUIRE_THROWS(Text::cast<unsigned int>("123 "));
            REQUIRE_THROWS(Text::cast<unsigned int>("12z"));

            REQUIRE_THROWS(Text::cast<long long>(""));
            REQUIRE(Text::cast<long long>("123") == 123);
            REQUIRE(Text::cast<long long>("-123") == -123);
            REQUIRE(Text::cast<long long>("  123") == 123);
            REQUIRE_THROWS(Text::cast<long long>("123 "));
            REQUIRE_THROWS(Text::cast<long long>("12z"));

            REQUIRE_THROWS(Text::cast<unsigned long long>(""));
            REQUIRE(Text::cast<unsigned long long>("123") == 123);
            REQUIRE_THROWS(Text::cast<unsigned long long>("-123"));
            REQUIRE(Text::cast<unsigned long long>("  123") == 123);
            REQUIRE_THROWS(Text::cast<unsigned long long>("123 "));
            REQUIRE_THROWS(Text::cast<unsigned long long>("12z"));

            REQUIRE_THROWS(Text::cast<double>(""));
            REQUIRE_THAT(Text::cast<double>("123.45"), WithinRel(123.45, 0.000001));
            REQUIRE_THAT(Text::cast<double>("-123.45"), WithinRel(-123.45, 0.000001));
            REQUIRE_THAT(Text::cast<double>("  123.45"), WithinRel(123.45, 0.000001));
            REQUIRE_THROWS(Text::cast<double>("123.45 "));
            REQUIRE_THROWS(Text::cast<double>("12z"));
        }
    }

    TEST_CASE("text", "[convert]" ) {
        {
            {
                std::vector<std::wstring> wcc { L"", L""s, L"12", L"3"s, L"45", L""s };
                auto mbc = Text::convert(wcc);

                REQUIRE(std::is_same_v<decltype(Text::convert(wcc)), std::vector<std::string>>);
                REQUIRE(mbc.size() == 6);
                REQUIRE(mbc[0].empty());
                REQUIRE(mbc[1].empty());
                REQUIRE(mbc[2] == "12");
                REQUIRE(mbc[3] == "3");
                REQUIRE(mbc[4] == "45");
                REQUIRE(mbc[5].empty());
            }

            {
                std::deque<std::wstring> wcc { L"", L""s, L"12", L"3"s, L"45", L""s };
                auto mbc = Text::convert(wcc);

                REQUIRE(std::is_same_v<decltype(Text::convert(wcc)), std::deque<std::string>>);
                REQUIRE(mbc.size() == 6);
                REQUIRE(mbc[0].empty());
                REQUIRE(mbc[1].empty());
                REQUIRE(mbc[2] == "12");
                REQUIRE(mbc[3] == "3");
                REQUIRE(mbc[4] == "45");
                REQUIRE(mbc[5].empty());
            }

            {
                std::list<std::wstring> wcc { L"", L""s, L"12", L"3"s, L"45", L""s };
                auto mbc = Text::convert(wcc);

                REQUIRE(std::is_same_v<decltype(Text::convert(wcc)), std::list<std::string>>);
                REQUIRE(mbc.size() == 6);

                auto it { mbc.begin() };

                REQUIRE(it++->empty());
                REQUIRE(it++->empty());
                REQUIRE(*it++ == "12");
                REQUIRE(*it++ == "3");
                REQUIRE(*it++ == "45");
                REQUIRE(it->empty());
            }
        }

        {
            {
                std::vector<std::string> mbc { "", ""s, "12", "3"s, "45", ""s };
                auto wcc = Text::convert(mbc);

                REQUIRE(std::is_same_v<decltype(Text::convert(mbc)), std::vector<std::wstring>>);
                REQUIRE(wcc.size() == 6);
                REQUIRE(wcc[0].empty());
                REQUIRE(wcc[1].empty());
                REQUIRE(wcc[2] == L"12");
                REQUIRE(wcc[3] == L"3");
                REQUIRE(wcc[4] == L"45");
                REQUIRE(wcc[5].empty());
            }

            {
                std::deque<std::string> mbc { "", ""s, "12", "3"s, "45", ""s };
                auto wcc = Text::convert(mbc);

                REQUIRE(std::is_same_v<decltype(Text::convert(mbc)), std::deque<std::wstring>>);
                REQUIRE(wcc.size() == 6);
                REQUIRE(wcc[0].empty());
                REQUIRE(wcc[1].empty());
                REQUIRE(wcc[2] == L"12");
                REQUIRE(wcc[3] == L"3");
                REQUIRE(wcc[4] == L"45");
                REQUIRE(wcc[5].empty());
            }

            {
                std::list<std::string> mbc { "", ""s, "12", "3"s, "45", ""s };
                auto wcc = Text::convert(mbc);

                REQUIRE(std::is_same_v<decltype(Text::convert(mbc)), std::list<std::wstring>>);
                REQUIRE(wcc.size() == 6);

                auto it { wcc.begin() };

                REQUIRE(it++->empty());
                REQUIRE(it++->empty());
                REQUIRE(*it++ == L"12");
                REQUIRE(*it++ == L"3");
                REQUIRE(*it++ == L"45");
                REQUIRE(it->empty());
            }
        }
    }

    TEST_CASE("text", "[split_to]" ) {
        {
            std::vector<std::wstring> c;
            Text::splitTo(c, L"  1,2 3 -4--", L" ,-"sv);

            REQUIRE(c.size() == 4);
            REQUIRE(c[0] == L"1");
            REQUIRE(c[1] == L"2");
            REQUIRE(c[2] == L"3");
            REQUIRE(c[3] == L"4");

            Text::splitTo(c, L";;;5;:6 7;::;"s, L" :;,-"s);

            REQUIRE(c.size() == 7);
            REQUIRE(c[4] == L"5");
            REQUIRE(c[5] == L"6");
            REQUIRE(c[6] == L"7");

            Text::splitTo(c, L";;8;:9---;"sv, L" :;,-");

            REQUIRE(c.size() == 9);
            REQUIRE(c[7] == L"8");
            REQUIRE(c[8] == L"9");

            Text::splitTo(c, L";--;abc;:def---;ghi"sv, L" :;,-", true);

            REQUIRE(c.size() == 3);
            REQUIRE(c[0] == L"abc");
            REQUIRE(c[1] == L"def");
            REQUIRE(c[2] == L"ghi");
        }

        {
            std::vector<std::string> c;
            Text::splitTo(c, "  1,2 3 -4--", " ,-"sv);

            REQUIRE(c.size() == 4);
            REQUIRE(c[0] == "1");
            REQUIRE(c[1] == "2");
            REQUIRE(c[2] == "3");
            REQUIRE(c[3] == "4");

            Text::splitTo(c, ";;;5;:6 7;::;"s, " :;,-"s);

            REQUIRE(c.size() == 7);
            REQUIRE(c[4] == "5");
            REQUIRE(c[5] == "6");
            REQUIRE(c[6] == "7");

            Text::splitTo(c, ";;8;:9---;"sv, " :;,-");

            REQUIRE(c.size() == 9);
            REQUIRE(c[7] == "8");
            REQUIRE(c[8] == "9");

            Text::splitTo(c, ";--;abc;:def---;ghi"sv, " :;,-", true);

            REQUIRE(c.size() == 3);
            REQUIRE(c[0] == "abc");
            REQUIRE(c[1] == "def");
            REQUIRE(c[2] == "ghi");
        }
    }

    TEST_CASE("text", "[split_variable]" ) {
        {
            std::wstring varName, varValue;
            Text::splitVariable(L"var=val"sv, varName, varValue);

            REQUIRE(varName == L"var"sv);
            REQUIRE(varValue == L"val"sv);

            Text::splitVariable(L"\r\n var\t=\tval \r\n"s, varName, varValue);

            REQUIRE(varName == L"var"sv);
            REQUIRE(varValue == L"val"sv);

            Text::splitVariable(L"\r\n var\t:\tval \r\n", varName, varValue);

            REQUIRE(varName == L"var"sv);
            REQUIRE(varValue == L"val"sv);

            Text::splitVariable(L"\r\n var\t \tval \r\n"sv, varName, varValue);

            REQUIRE(varName == L"var\t \tval"sv);
            REQUIRE(varValue.empty());

            Text::splitVariable(L"\r\n :var\t \tval \r\n"s, varName, varValue);

            REQUIRE(varName.empty());
            REQUIRE(varValue == L"var\t \tval"sv);

            Text::splitVariable(L"var="s, varName, varValue);

            REQUIRE(varName == L"var"sv);
            REQUIRE(varValue.empty());

            Text::splitVariable(L":val"s, varName, varValue);

            REQUIRE(varName.empty());
            REQUIRE(varValue == L"val"sv);
        }

        {
            std::string varName, varValue;
            Text::splitVariable("var=val"sv, varName, varValue);

            REQUIRE(varName == "var"sv);
            REQUIRE(varValue == "val"sv);

            Text::splitVariable("\r\n var\t=\tval \r\n"s, varName, varValue);

            REQUIRE(varName == "var"sv);
            REQUIRE(varValue == "val"sv);

            Text::splitVariable("\r\n var\t:\tval \r\n", varName, varValue);

            REQUIRE(varName == "var"sv);
            REQUIRE(varValue == "val"sv);

            Text::splitVariable("\r\n var\t \tval \r\n"sv, varName, varValue);

            REQUIRE(varName == "var\t \tval"sv);
            REQUIRE(varValue.empty());

            Text::splitVariable("\r\n :var\t \tval \r\n"s, varName, varValue);

            REQUIRE(varName.empty());
            REQUIRE(varValue == "var\t \tval"sv);

            Text::splitVariable("var="s, varName, varValue);

            REQUIRE(varName == "var"sv);
            REQUIRE(varValue.empty());

            Text::splitVariable(":val"s, varName, varValue);

            REQUIRE(varName.empty());
            REQUIRE(varValue == "val"sv);
        }
    }

    TEST_CASE("text", "[join_to]" ) {
        {
            std::wstring s { L"Expect " };

            SECTION("join std::vector<std::wstring>") {
                std::vector<std::wstring> c {
                    L"", L""s, L"12", L"34"s, L"5", L""s, L"", L""s, L"ab", L"cd"s, L"e", L""s, L""
                };
                Text::joinTo(s, c, L"");

                REQUIRE(s == L"Expect 12345abcde"sv);
            }

            SECTION("join std::deque<std::wstring_view>") {
                std::deque<std::wstring_view> c {
                    L"", L""sv, L"12", L"34"sv, L"5", L""sv, L"", L""sv, L"ab", L"cd"sv, L"e", L""sv, L""
                };
                Text::joinTo(s, c, L"*");

                REQUIRE(s == L"Expect *12*34*5*ab*cd*e"sv);
            }

            SECTION("join std::list<std::wstring>") {
                std::list<std::wstring> c {
                    L"", L""s, L"12", L"34"s, L"5", L""s, L"", L""s, L"ab", L"cd"s, L"e", L""s, L""
                };
                Text::joinTo(s, c, L"ZZ");

                REQUIRE(s == L"Expect ZZ12ZZ34ZZ5ZZabZZcdZZe"sv);
            }

            SECTION("append scalar") {
                Text::joinTo(s, L"YZ"s, L"X");

                REQUIRE(s == L"Expect XYZ"sv);

                std::wstring c { L"BC" };
                Text::joinTo(s, c, L"A");

                REQUIRE(s == L"Expect XYZABC"sv);
                REQUIRE(c == L"BC"sv);

                Text::joinTo(s, std::move(c), L"A");

                REQUIRE(s == L"Expect XYZABCABC"sv);
                REQUIRE(c == L"BC"sv);

                std::wstring d;
                Text::joinTo(d, std::move(c), L"A");

                REQUIRE(d == L"BC"sv);
                REQUIRE(c.empty());
            }
        }

        {
            std::string s { "Expect " };

            SECTION("join std::vector<std::string>") {
                std::vector<std::string> c {
                    "", ""s, "12", "34"s, "5", ""s, "", ""s, "ab", "cd"s, "e", ""s, ""
                };
                Text::joinTo(s, c, "");

                REQUIRE(s == "Expect 12345abcde"sv);
            }

            SECTION("join std::deque<std::string_view>") {
                std::deque<std::string_view> c {
                    "", ""sv, "12", "34"sv, "5", ""sv, "", ""sv, "ab", "cd"sv, "e", ""sv, ""
                };
                Text::joinTo(s, c, "*");

                REQUIRE(s == "Expect *12*34*5*ab*cd*e"sv);
            }

            SECTION("join std::list<std::string>") {
                std::list<std::string> c {
                    "", ""s, "12", "34"s, "5", ""s, "", ""s, "ab", "cd"s, "e", ""s, ""
                };
                Text::joinTo(s, c, "ZZ");

                REQUIRE(s == "Expect ZZ12ZZ34ZZ5ZZabZZcdZZe"sv);
            }

            SECTION("append scalar") {
                Text::joinTo(s, "YZ"s, "X");

                REQUIRE(s == "Expect XYZ"sv);

                std::string c { "BC" };
                Text::joinTo(s, c, "A");

                REQUIRE(s == "Expect XYZABC"sv);
                REQUIRE(c == "BC"sv);

                Text::joinTo(s, std::move(c), "A");

                REQUIRE(s == "Expect XYZABCABC"sv);
                REQUIRE(c == "BC"sv);

                std::string d;
                Text::joinTo(d, std::move(c), "A");

                REQUIRE(d == "BC"sv);
                REQUIRE(c.empty());
            }
        }
    }

    TEST_CASE("text", "[concat]" ) {
        {
            auto s = Text::concat(
                L"", L""s, L""sv, L"12", L"34"s, L"5"sv, L"", L""s, L""sv, L"ab"sv, L"cd"s, L"e", L""sv, L""s, L""
            );

            REQUIRE(s == L"12345abcde"sv);
        }

        {
            auto s = Text::concat(
                "", ""s, ""sv, "12", "34"s, "5"sv, "", ""s, ""sv, "ab"sv, "cd"s, "e", ""sv, ""s, ""
            );

            REQUIRE(s == "12345abcde"sv);
        }
    }

    TEST_CASE("text", "[bool_to_str_cast]" ) {
        REQUIRE(Text::Wcs::yesNo(true) == L"yes"sv);
        REQUIRE(Text::Mbs::yesNo(true) == "yes"sv);
        REQUIRE(Text::yesNo<Meta::Wcs>(true) == L"yes"sv);
        REQUIRE(Text::yesNo<Meta::Mbs>(true) == "yes"sv);
        REQUIRE(Text::Wcs::yesNo(false) == L"no"sv);
        REQUIRE(Text::Mbs::yesNo(false) == "no"sv);
        REQUIRE(Text::yesNo<Meta::Wcs>(false) == L"no"sv);
        REQUIRE(Text::yesNo<Meta::Mbs>(false) == "no"sv);

        REQUIRE(Text::Wcs::enaDis(true) == L"enable"sv);
        REQUIRE(Text::Mbs::enaDis(true) == "enable"sv);
        REQUIRE(Text::enaDis<Meta::Wcs>(true) == L"enable"sv);
        REQUIRE(Text::enaDis<Meta::Mbs>(true) == "enable"sv);
        REQUIRE(Text::Wcs::enaDis(false) == L"disable"sv);
        REQUIRE(Text::Mbs::enaDis(false) == "disable"sv);
        REQUIRE(Text::enaDis<Meta::Wcs>(false) == L"disable"sv);
        REQUIRE(Text::enaDis<Meta::Mbs>(false) == "disable"sv);

        REQUIRE(Text::Wcs::trueFalse(true) == L"true"sv);
        REQUIRE(Text::Mbs::trueFalse(true) == "true"sv);
        REQUIRE(Text::trueFalse<Meta::Wcs>(true) == L"true"sv);
        REQUIRE(Text::trueFalse<Meta::Mbs>(true) == "true"sv);
        REQUIRE(Text::Wcs::trueFalse(false) == L"false"sv);
        REQUIRE(Text::Mbs::trueFalse(false) == "false"sv);
        REQUIRE(Text::trueFalse<Meta::Wcs>(false) == L"false"sv);
        REQUIRE(Text::trueFalse<Meta::Mbs>(false) == "false"sv);
    }
}
