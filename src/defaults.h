// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "library/basic.h"
#include "library/datetime.h"

namespace Basic {
    constexpr size_t c_defaultBufferSize { 2'048 };
}

namespace Config {
    constexpr std::wstring_view c_configDirectoryEnv { L"kkmha_conf_dir" };
    constexpr std::wstring_view c_configDirectory { L"conf" };
    constexpr std::wstring_view c_configFile { L"kkmha.json" };
    constexpr std::wstring_view c_logsDirectoryEnv [[maybe_unused]] { L"kkmha_logs_dir" };
}

namespace Service {
    constexpr const wchar_t * c_systemName { L"KKMHA" };
    constexpr const wchar_t * c_displayName { L"KKM HTTPS Adapter" };
    constexpr DWORD c_startType { SERVICE_AUTO_START };
    constexpr const wchar_t * c_dependencies { L"\0\0" }; // Формат: "dep1\0dep2\0\0"
    constexpr const wchar_t * c_account { L"NT AUTHORITY\\LocalService" };
    constexpr const wchar_t * c_password { L"" };
    constexpr DWORD c_stoppingTimeout { 30'000 }; // Миллисекунды
}

namespace Log {
    constexpr bool c_outputTimestamp { false };
    constexpr bool c_outputLevel { true };
    constexpr std::wstring_view c_directory { L"logs" };
    constexpr const wchar_t * c_eventSource { Service::c_displayName };
    constexpr DWORD c_eventId { 0 };
    constexpr WORD c_eventCategory { 0 };
}

namespace Server {
    constexpr bool c_defIpv4Only { false };
    constexpr unsigned short c_minPort { 1 };
    constexpr unsigned short c_maxPort { 65'535 };
    constexpr unsigned short c_defPort { 5'757 };
    constexpr int64_t c_minConcurrencyLimit { 2 };
    constexpr int64_t c_maxConcurrencyLimit { 100 };
    constexpr int64_t c_defConcurrencyLimit { 2'000 };
    constexpr std::string_view c_defCertificateChainFile { "conf\\kkmha.crt" };
    constexpr std::string_view c_defPrivateKeyFile { "conf\\kkmha.key" };
    constexpr size_t c_stopWaiting { 10 }; // Секунды
}

namespace Http {
    constexpr size_t c_cacheCleanUpThreshold { 200 };
    constexpr std::string_view c_defSecret { "!!! don't forget to change me !!!" };
    constexpr bool c_loopbackWithoutSecret { true };
    constexpr bool c_enableStatic { false };
    constexpr std::wstring_view c_staticDirectory { L"static" };
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
    constexpr size_t c_maxLength { 128 };
    constexpr std::wstring_view c_connParamsSeparator { L"," };
    constexpr wchar_t c_separatorChar { L'-' };
    constexpr std::wstring_view c_defDirectory { L"conf\\kkm" };
    constexpr std::wstring_view c_defBaudRate { L"115200" };
    constexpr size_t c_minLineLength { 24 };
    constexpr size_t c_maxLineLength { c_maxLength };
    constexpr size_t c_defLineLength { 34 };
    constexpr std::wstring_view c_defCliOperatorName { L"Оператор" };
    constexpr std::wstring_view c_defCliOperatorInn {};
    constexpr double c_minQuantity { 0.001 };
    constexpr double c_maxQuantity { 999'999.999 };
    constexpr double c_minPrice { 0.01 };
    constexpr double c_maxPrice { 99'999'999.99 };
    constexpr std::wstring_view c_customerAccountField { L"Лицевой счёт (идентификатор для оплаты):" };
    constexpr size_t c_minClosedCheckingAttempts { 1 };
    constexpr size_t c_maxClosedCheckingAttempts { 30 };
    constexpr size_t c_defClosedCheckingAttempts { 5 };
    constexpr size_t c_minClosedCheckingWaiting { 10 }; // Миллисекунды
    constexpr size_t c_maxClosedCheckingWaiting { 1'000 }; // Миллисекунды
    constexpr size_t c_defClosedCheckingWaiting { 100 }; // Миллисекунды
}
