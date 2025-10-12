// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <lib/datetime.h>
#include <string_view>

namespace Server {
    constexpr const DateTime::SleepUnit c_sleep { DateTime::c_basicSleep }; // Миллисекунды
    constexpr const DateTime::SleepUnit c_sleepQuantum { DateTime::c_basicSleepQuantum }; // Миллисекунды
    constexpr const DateTime::SleepUnit c_controlTimeout { 30 * DateTime::c_basicSleep }; // Миллисекунды
    constexpr const DateTime::SleepUnit c_closingDelay { 3 * DateTime::c_basicSleep }; // Миллисекунды
    constexpr const int64_t c_delayedSockets { 10'000 };
    constexpr const int64_t c_minRequestTimeout { 6 }; // Секунды
    constexpr const int64_t c_maxRequestTimeout { 1'800 }; // Секунды
    constexpr const int64_t c_defRequestTimeout { 180 }; // Секунды
    constexpr const bool c_defIpv4Only { false };
    constexpr const unsigned short c_minPort { 1 };
    constexpr const unsigned short c_maxPort { 65'535 };
    constexpr const unsigned short c_defPort { 5'757 };
    constexpr const int64_t c_minConcurrencyLimit { 2 };
    constexpr const int64_t c_maxConcurrencyLimit { 100 };
    constexpr const int64_t c_defConcurrencyLimit { 10 };
    constexpr const std::wstring_view c_defCertificateChainFile { L"kkmha.crt" };
    constexpr const std::wstring_view c_defPrivateKeyFile { L"kkmha.key" };
    constexpr const size_t c_cacheCleanUpThreshold { 200 };
    constexpr const std::string_view c_defSecret { "!!! don't forget to change me !!!" };
    constexpr const bool c_loopbackWithoutSecret { true };
}
