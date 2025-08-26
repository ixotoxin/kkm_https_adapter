// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <lib/basic.h>
#include <lib/datetime.h>

namespace Basic {
    constexpr size_t c_defaultBufferSize { 2'048 };
    constexpr std::chrono::milliseconds::rep c_sleep { 1000 }; // Миллисекунды
    constexpr std::chrono::milliseconds::rep c_sleepQuantum { 200 }; // Миллисекунды
}

namespace Config {
    constexpr std::wstring_view c_configDirectoryEnv { L"kkmha_conf_dir" };
    constexpr std::wstring_view c_configDirectory { L"conf" };
    constexpr std::wstring_view c_configFile { L"kkmha.json" };
}

namespace Service {
    constexpr const wchar_t * c_systemName { L"KKMHA" };
    constexpr const wchar_t * c_displayName { L"KKM HTTPS Adapter" };
    constexpr ::DWORD c_startType { SERVICE_AUTO_START };
    constexpr const wchar_t * c_dependencies { L"\0\0" }; // Формат: "dep1\0dep2\0\0"
    constexpr const wchar_t * c_account { L"NT AUTHORITY\\LocalService" };
    constexpr const wchar_t * c_password { L"" };
    constexpr ::DWORD c_waitHint { 35'000 }; // Миллисекунды
    constexpr ::DWORD c_controlTimeout { 40'000 }; // Миллисекунды
}

namespace Log {
    constexpr bool c_outputTimestamp { false };
    constexpr bool c_outputLevel { true };
    constexpr std::wstring_view c_directory { L"logs" };
    constexpr std::wstring_view c_logFileFormat { L"kkmha-{:04d}-{:02d}.log" };
    constexpr const wchar_t * c_eventSource { Service::c_displayName };
    constexpr ::DWORD c_eventId { 0 };
    constexpr ::WORD c_eventCategory { 0 };
}

namespace Server {
    constexpr bool c_defIpv4Only { false };
    constexpr unsigned short c_minPort { 1 };
    constexpr unsigned short c_maxPort { 65'535 };
    constexpr unsigned short c_defPort { 5'757 };
    constexpr int64_t c_minConcurrencyLimit { 2 };
    constexpr int64_t c_maxConcurrencyLimit { 2'000 };
    constexpr int64_t c_defConcurrencyLimit { 10 };
    constexpr std::string_view c_defCertificateChainFile { "conf\\kkmha.crt" };
    constexpr std::string_view c_defPrivateKeyFile { "conf\\kkmha.key" };
    constexpr std::chrono::milliseconds::rep c_controlTimeout { 30'000 }; // Миллисекунды
}

namespace Http {
    constexpr size_t c_cacheCleanUpThreshold { 200 };
    constexpr std::string_view c_defSecret { "!!! don't forget to change me !!!" };
    constexpr bool c_loopbackWithoutSecret { true };
    constexpr bool c_enableStatic { false };
    constexpr std::wstring_view c_staticDirectory { L"static" };
    constexpr std::string_view c_indexFile { "index.html" };
    constexpr std::wstring_view c_mimeMapFile { L"conf\\mime.json" };
    constexpr bool c_enableUnknownType { false };
    constexpr std::string_view c_defMimeType { "application/octet-stream" };
    constexpr size_t c_requestBodySizeLimit { 131'072 };
    constexpr size_t c_fileSizeLimit { 2'097'152 };
    constexpr DateTime::OffsetValue c_reportCacheLifeTime { 5 };
    constexpr DateTime::OffsetValue c_receiptCacheLifeTime { 345'600 };
    constexpr DateTime::OffsetValue c_fileCacheLifeTime { 43'200 };
}

namespace Kkm {
    constexpr size_t c_maxLength { 192 };
    constexpr std::wstring_view c_connParamsSeparator { L"," };
    constexpr wchar_t c_separatorChar { L'-' };
    constexpr std::wstring_view c_defDbDirectory { L"conf\\kkm" };
    constexpr std::wstring_view c_defBaudRate { L"115200" };
    constexpr size_t c_minLineLength { 24 };
    constexpr size_t c_maxLineLength { c_maxLength };
    constexpr size_t c_defLineLength { 42 };
    constexpr auto c_minDocumentClosingTimeout = Basic::c_sleepQuantum; // Миллисекунды
    constexpr auto c_maxDocumentClosingTimeout = Basic::c_sleep * 10; // Миллисекунды
    constexpr auto c_defDocumentClosingTimeout = Basic::c_sleep; // Миллисекунды
    constexpr std::wstring_view c_defCliOperatorName { L"Оператор" };
    constexpr std::wstring_view c_defCliOperatorInn {};
    constexpr std::wstring_view c_customerAccountField { L"Лицевой счёт (идентификатор для оплаты):" };
    constexpr double c_minCashInOut { 0.01 };
    constexpr double c_minMaxCashInOut { 1 };
    constexpr double c_maxMaxCashInOut { 1e9 };
    constexpr double c_defMaxCashInOut { 5e5 };
    constexpr double c_minPrice { 0.01 };
    constexpr double c_minMaxPrice { 1 };
    constexpr double c_maxMaxPrice { 1e9 };
    constexpr double c_defMaxPrice { 3e5 };
    constexpr double c_minQuantity { 0.001 };
    constexpr double c_minMaxQuantity { 1 };
    constexpr double c_maxMaxQuantity { 1e9 };
    constexpr double c_defMaxQuantity { 1e3 };
}
