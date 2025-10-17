// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <lib/datetime.h>
#include <string_view>

namespace Server {
    constexpr DateTime::SleepUnit c_sleep { DateTime::c_basicSleep }; // Миллисекунды
    constexpr DateTime::SleepUnit c_sleepQuantum { DateTime::c_basicSleepQuantum }; // Миллисекунды
    constexpr DateTime::SleepUnit c_controlTimeout { 30 * DateTime::c_basicSleep }; // Миллисекунды
    constexpr DateTime::SleepUnit c_closingDelay { 3 * DateTime::c_basicSleep }; // Миллисекунды
    constexpr int64_t c_delayedSockets { 10'000 };
    constexpr int64_t c_minRequestTimeout { 6 }; // Секунды
    constexpr int64_t c_maxRequestTimeout { 1'800 }; // Секунды
    constexpr int64_t c_defRequestTimeout { 180 }; // Секунды
    constexpr bool c_defIpv4Only { false };
    constexpr unsigned short c_minPort { 1 };
    constexpr unsigned short c_maxPort { 65'535 };
    constexpr unsigned short c_defPort { 5'757 };
    constexpr int64_t c_minConcurrencyLimit { 2 };
    constexpr int64_t c_maxConcurrencyLimit { 100 };
    constexpr int64_t c_defConcurrencyLimit { 10 };
    constexpr std::wstring_view c_defCertificateChainFile { L"kkmha.crt" };
    constexpr std::wstring_view c_defPrivateKeyFile { L"kkmha.key" };
    constexpr size_t c_cacheCleanUpThreshold { 200 };
    constexpr std::string_view c_defSecret { "!!! don't forget to change me !!!" };
    constexpr bool c_loopbackWithoutSecret { true };
}
