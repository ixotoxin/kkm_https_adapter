// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <lib/winapi.h>
#include <lib/datetime.h>

namespace Service {
    constexpr const DateTime::SleepUnit c_sleepQuantum { DateTime::c_basicSleepQuantum }; // Миллисекунды
    constexpr const wchar_t * c_systemName { L"KKMHA" };
    constexpr const wchar_t * c_displayName { L"KKM HTTPS Adapter" };
    constexpr const ::DWORD c_startType { SERVICE_AUTO_START };
    constexpr const wchar_t * c_dependencies { L"\0\0" }; // Формат: "dep1\0dep2\0\0"
    constexpr const wchar_t * c_account { L"NT AUTHORITY\\LocalService" };
    constexpr const wchar_t * c_password { L"" };
    constexpr const ::DWORD c_waitHint { 35 * DateTime::c_basicSleep.count() }; // Миллисекунды
    constexpr const ::DWORD c_controlTimeout { 40 * DateTime::c_basicSleep.count() }; // Миллисекунды
}
