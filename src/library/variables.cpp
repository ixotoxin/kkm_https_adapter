// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include <lib/winapi.h>
#include <lib/datetime.h>
#include <log/types.h>
#include <log/variables.h>
#include <kkm/types.h>
#include <kkm/variables.h>
#include <config/defaults.h>
#include <config/variables.h>
#include <string>
#include <filesystem>

namespace Main {
    std::filesystem::path s_file {};
    std::filesystem::path s_directory {};
}

namespace Config {
    const std::wstring_view c_directoryEnv { L"kkmha_conf_dir" };
    const std::wstring_view c_directory { L"conf" };
    const std::wstring_view c_file { L"kkmha.json" };
    std::filesystem::path s_directory {};
    std::filesystem::path s_file {};
}

namespace Log {
    const std::wstring_view c_directoryEnv { L"kkmha_logs_dir" };
    const std::wstring_view c_directory { L"logs" };
    const ::DWORD c_eventId { 0 };
    const ::WORD c_eventCategory { 0 };

    namespace Console {
        LevelUnderlying s_level { c_levelDebug };
        bool s_outputTimestamp { false };
        bool s_outputLevel { true };
    }

    namespace File {
        LevelUnderlying s_bgLevel { c_levelNone };
        LevelUnderlying s_fgLevel { c_levelInfo };
        std::filesystem::path s_directory { L"logs" };
    }

    namespace EventLog {
        LevelUnderlying s_bgLevel { c_levelNone };
        LevelUnderlying s_fgLevel { c_levelInfo };
    }

#ifdef DEBUG
    bool s_appendLocation { true };
#else
    bool s_appendLocation { false };
#endif
}

namespace Kkm {
    const DateTime::SleepUnit c_sleepQuantum { DateTime::c_basicSleepQuantum }; // Миллисекунды
    const std::wstring_view c_connParamsSeparator { L"," };
    const wchar_t c_separatorChar { L'-' };
    std::filesystem::path s_dbDirectory { L"kkm" };
    std::wstring s_defaultBaudRate { L"115200" };
    const size_t c_minLineLength { 24 };
    const size_t c_maxLineLength { 192 };
    size_t s_defaultLineLength { 42 };
    TimeZone s_timeZone { TimeZone::Device };
    bool s_timeZoneConfigured { false };
    const DateTime::SleepUnit c_minDocumentClosingTimeout { DateTime::c_basicSleepQuantum }; // Миллисекунды
    const DateTime::SleepUnit c_maxDocumentClosingTimeout { 10 * DateTime::c_basicSleep }; // Миллисекунды
    DateTime::SleepUnit s_documentClosingTimeout { DateTime::c_basicSleep }; // Миллисекунды
    std::wstring s_cliOperatorName { L"Оператор" };
    std::wstring s_cliOperatorInn {};
    std::wstring s_customerAccountField { L"Лицевой счёт (идентификатор для оплаты):" };
    const double c_minCashInOut { 0.01 };
    const double c_minMaxCashInOut { 1 };
    const double c_maxMaxCashInOut { 1e9 };
    double s_maxCashInOut { 5e5 };
    const double c_minPrice { 0.01 };
    const double c_minMaxPrice { 1 };
    const double c_maxMaxPrice { 1e9 };
    double s_maxPrice { 3e5 };
    const double c_minQuantity { 0.001 };
    const double c_minMaxQuantity { 1 };
    const double c_maxMaxQuantity { 1e9 };
    double s_maxQuantity { 1e3 };
}
