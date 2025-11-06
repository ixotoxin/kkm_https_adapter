// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "macro.h"
#include <lib/datetime.h>
#include <string_view>

namespace Kkm {
    KKM_CONST(DateTime::SleepUnit, c_sleepQuantum, DateTime::c_basicSleepQuantum); // Миллисекунды
    KKM_CONST(std::wstring_view, c_connParamsSeparator, L",");
    KKM_CONST(wchar_t, c_separatorChar, L'-');
    KKM_DEF(std::wstring_view, c_defDbDirectory, L"kkm");
    KKM_DEF(std::wstring_view, c_defBaudRate, L"115200");
    KKM_CONST(size_t, c_minLineLength, 24);
    KKM_CONST(size_t, c_maxLineLength, 192);
    KKM_DEF(size_t, c_defLineLength, 42);
    KKM_CONST(size_t, c_maxTextLength, 512);
    KKM_CONST(DateTime::SleepUnit, c_minDocumentClosingTimeout, DateTime::c_basicSleepQuantum); // Миллисекунды
    KKM_CONST(DateTime::SleepUnit, c_maxDocumentClosingTimeout, 10 * DateTime::c_basicSleep); // Миллисекунды
    KKM_DEF(DateTime::SleepUnit, c_defDocumentClosingTimeout, DateTime::c_basicSleep); // Миллисекунды
    KKM_DEF(std::wstring_view, c_defCliOperatorName, L"Оператор");
    KKM_DEF(std::wstring_view, c_defCliOperatorInn, );
    KKM_DEF(std::wstring_view, c_defCustomerAccountField, L"Лицевой счёт (идентификатор для оплаты):");
    KKM_CONST(double, c_minCashInOut, 0.01);
    KKM_CONST(double, c_minMaxCashInOut, 1);
    KKM_CONST(double, c_maxMaxCashInOut, 1e9);
    KKM_DEF(double, c_defMaxCashInOut, 5e5);
    KKM_CONST(double, c_minPrice, 0.01);
    KKM_CONST(double, c_minMaxPrice, 1);
    KKM_CONST(double, c_maxMaxPrice, 1e9);
    KKM_DEF(double, c_defMaxPrice, 3e5);
    KKM_CONST(double, c_minQuantity, 0.001);
    KKM_CONST(double, c_minMaxQuantity, 1);
    KKM_CONST(double, c_maxMaxQuantity, 1e9);
    KKM_DEF(double, c_defMaxQuantity, 1e3);
}
