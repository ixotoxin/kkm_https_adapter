// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <lib/datetime.h>
#include <string_view>

namespace Server {
    constexpr const DateTime::SleepUnit c_sleep { DateTime::c_basicSleep }; // Миллисекунды
    constexpr const DateTime::SleepUnit c_sleepQuantum { DateTime::c_basicSleepQuantum }; // Миллисекунды
    constexpr const DateTime::SleepUnit c_controlTimeout { 30 * c_sleep };
    constexpr const bool c_defIpv4Only { false };
    constexpr const unsigned short c_minPort { 1 };
    constexpr const unsigned short c_maxPort { 65'535 };
    constexpr const unsigned short c_defPort { 5'757 };
    constexpr const int64_t c_minConcurrencyLimit { 2 };
    constexpr const int64_t c_maxConcurrencyLimit { 2'000 };
    constexpr const int64_t c_defConcurrencyLimit { 10 };
    constexpr const std::string_view c_defCertificateChainFile { "conf\\kkmha.crt" };
    constexpr const std::string_view c_defPrivateKeyFile { "conf\\kkmha.key" };
    constexpr const size_t c_cacheCleanUpThreshold { 200 };
    constexpr const std::string_view c_defSecret { "!!! don't forget to change me !!!" };
    constexpr const bool c_loopbackWithoutSecret { true };
}
