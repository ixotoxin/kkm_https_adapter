// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "library/utils.h"
#include <iostream>
// #include <map>
// #include <set>
#include <fcntl.h>
#include <io.h>
// #include <fptr10.h>
// #include "log.h"
// #include "asio.h"



// #include "http.h"





using namespace std;
using namespace std::string_literals;

template<typename T>
void info(const std::wstring_view t) {
    wcout << format(L"{:>48} : {:4} bytes : {}", t, sizeof(T), alignof(T)) << std::endl;
}

int wmain(int, wchar_t **, wchar_t **) {
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stderr), _O_U16TEXT);

    // {
    //     Deferred::Exec a { []() {
    //         std::wclog << "throw!\n";
    //         throw 1;
    //     }};
    // }

    // wstring str1 { L"str1 {}" };
    // wstring_view str2 { L"str2 {}" };
    //
    // wcout << endl;
    // ntsLogDebug(str1);
    // ntsLogDebug(str2);
    // ntsLogDebug(L"str3");
    //
    // wcout << endl;
    // ntsLogDebug(str1, 1);
    // ntsLogDebug(str2, 2);
    // ntsLogDebug(L"str3 {}", 3);
    //
    // const char * octets { "1234 / Distributed under the MIT License, see accompanying file LICENSE.txt" };
    // wcout << endl;
    // Log::Nts::dump(L"Dump as uint8", octets, 29);
    // wcout << endl;
    // Log::Nts::dump(L"Dump as uint8", octets, 29, 11);
    // wcout << endl;
    // Log::Nts::dump(L"Dump as uint16", reinterpret_cast<const short *>(octets), 29);
    // wcout << endl;
    // Log::Ts::dump(L"Dump as uint32", reinterpret_cast<const int *>(octets), 13);
    // wcout << endl;
    // Log::Ts::dump(L"Dump as uint64", reinterpret_cast<const long long *>(octets), 7);
    //
    // wcout << endl;
    // info<size_t>(L"size_t");
    // info<double>(L"double");
    // info<long double>(L"long double");
    // wcout << endl;
    // info<std::tm>(L"std::tm");
    // info<DateTime::Point>(L"DateTime::Point");
    // info<DateTime::Offset>(L"DateTime::Offset");
    // info<DateTime::OffsetValue>(L"DateTime::OffsetValue");
    // wcout << endl;
    // info<shared_ptr<int>>(L"shared_ptr<int>");
    // info<shared_ptr<string>>(L"shared_ptr<string>");
    // info<std::wstring>(L"std::wstring");
    // info<std::wstring_view>(L"std::wstring_view");
    // info<std::string>(L"std::string");
    // info<std::string_view>(L"std::string_view");
    // info<std::exception>(L"std::exception");
    // std::exception e;
    // info<std::vector<std::wstring>>(L"std::vector<std::wstring>");
    // info<std::vector<std::string>>(L"std::vector<std::string>");
    // info<std::map<int, std::wstring>>(L"std::map<std::wstring>");
    // info<std::map<int, std::string>>(L"std::map<std::string>");
    // info<std::unordered_map<std::wstring, std::wstring>>(L"std::unordered_map<std::wstring, std::wstring>");
    // info<std::unordered_map<std::string, std::string>>(L"std::unordered_map<std::string, std::string>");
    // info<std::unordered_map<int, std::wstring>>(L"std::unordered_map<int, std::wstring>");
    // info<std::unordered_map<int, std::string>>(L"std::unordered_map<int, std::string>");
    // info<std::set<std::wstring>>(L"std::set<std::wstring>");
    // info<std::set<std::string>>(L"std::set<std::string>");
    // info<std::unordered_set<std::wstring>>(L"std::unordered_set<std::wstring>");
    // info<std::unordered_set<std::string>>(L"std::unordered_set<std::string>");
    // wcout << endl;
    // using func = std::function<std::shared_ptr<vector<string>>(string &, string &, string &)>;
    // info<func>(L"std::function<...>");
    // wcout << endl;
    // info<nlohmann::json>(L"nlohmann::json");
    // wcout << endl;
    // info<Asio::Error>(L"Asio::Error");
    // info<Asio::IpAddress>(L"Asio::IpAddress");
    // info<Asio::IoContext>(L"Asio::IoContext");
    // info<Asio::StreamBuffer>(L"Asio::StreamBuffer");
    // info<Asio::SslContext>(L"Asio::SslContext");
    // info<Asio::Acceptor>(L"Asio::Acceptor");
    // info<Asio::Socket>(L"Asio::Socket");
    // info<Asio::Stream>(L"Asio::Stream");
    // info<Asio::Endpoint>(L"Asio::Endpoint");
    // wcout << endl;
    // info<Atol::Fptr::Fptr>(L"Atol::Fptr::Fptr");
}
