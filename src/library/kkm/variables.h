// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "macro.h"
#include "types.h"
#include "defaults.h"
#include <string>
#include <filesystem>

namespace Kkm {
    KKM_VAR(std::filesystem::path, s_dbDirectory, c_defDbDirectory);
    KKM_VAR(std::wstring, s_defaultBaudRate, c_defBaudRate);
    KKM_VAR(size_t, s_defaultLineLength, c_defLineLength);
    KKM_VAR(TimeZone, s_timeZone, TimeZone::Device);
    KKM_VAR(bool, s_timeZoneConfigured, false);
    KKM_VAR(DateTime::SleepUnit, s_documentClosingTimeout, c_defDocumentClosingTimeout);
    KKM_VAR(std::wstring, s_cliOperatorName, c_defCliOperatorName);
    KKM_VAR(std::wstring, s_cliOperatorInn, c_defCliOperatorInn);
    KKM_VAR(std::wstring, s_customerAccountField, c_defCustomerAccountField);
    KKM_VAR(double, s_maxCashInOut, c_defMaxCashInOut);
    KKM_VAR(double, s_maxPrice, c_defMaxPrice);
    KKM_VAR(double, s_maxQuantity, c_defMaxQuantity);
}
