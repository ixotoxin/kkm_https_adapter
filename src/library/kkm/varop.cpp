// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "varop.h"
#include "defaults.h"
#include "variables.h"
#include "strings.h"
#include "device.h"
#include <lib/numeric.h>
#include <lib/text.h>

namespace Kkm {
    void setVars(const Nln::Json & json) {
        Json::handleKey(
            json, "kkm",
            [] (const Nln::Json & json, const std::wstring & path) -> bool {
                Json::handleKey(json, "dbDirectory", s_dbDirectory, Text::Wcs::trim());
                Json::handleKey(json, "defaultBaudRate", s_defaultBaudRate, Wcs::c_allowedBaudRate, path);
                Json::handleKey(
                    json, "defaultLineLength",
                    s_defaultLineLength,
                    Numeric::between(c_minLineLength, c_maxLineLength),
                    path
                );
                Json::handleKey(
                    json, "timeZone",
                    s_timeZone, Mbs::c_timeZoneMap,
                    [] (auto value) { s_timeZoneConfigured = true; return value; },
                    path
                );
                Json::handleKey(
                    json, "documentClosingTimeout",
                    s_documentClosingTimeout,
                    DateTime::between(c_minDocumentClosingTimeout, c_maxDocumentClosingTimeout),
                    path
                );
                Json::handleKey(
                    json, "cliOperator",
                    [] (const Nln::Json & json, const std::wstring & path) -> bool {
                        Json::handleKey(json, "name", s_cliOperatorName, Text::Wcs::noEmpty(Text::Wcs::trim()), path);
                        Json::handleKey(json, "inn", s_cliOperatorInn, Text::Wcs::trim(), path);
                        return true;
                    },
                    path
                );
                Json::handleKey(json, "customerAccountField", s_customerAccountField, path);
                Json::handleKey(
                    json, "maxCashInOut",
                    s_maxCashInOut,
                    Numeric::between(c_minMaxCashInOut, c_maxMaxCashInOut),
                    path
                );
                Json::handleKey(
                    json, "maxPrice",
                    s_maxPrice,
                    Numeric::between(c_minMaxPrice, c_maxMaxPrice),
                    path
                );
                Json::handleKey(
                    json, "maxQuantity",
                    s_maxQuantity,
                    Numeric::between(c_minMaxQuantity, c_maxMaxQuantity),
                    path
                );
                return true;
            }
        );
    }

    std::wostream & vars(std::wostream & stream) {
        stream
            << L"[CFG] kkm.dbDirectory = \"" << s_dbDirectory << L"\"\n"
            L"[CFG] kkm.defaultBaudRate = " << s_defaultBaudRate << L"\n"
            L"[CFG] kkm.defaultLineLength = " << s_defaultLineLength << L"\n"
            L"[CFG] kkm.timeZone = tz" << Meta::toUnderlying(s_timeZone) << L"\n"
            L"[CFG] kkm.documentClosingTimeout = " << s_documentClosingTimeout << L"\n"
            L"[CFG] kkm.cliOperator.name = \"" << s_cliOperatorName << L"\"\n"
            L"[CFG] kkm.cliOperator.inn = \"" << s_cliOperatorInn << L"\"\n"
            L"[CFG] kkm.customerAccountField = \"" << s_customerAccountField << L"\"\n"
            L"[CFG] kkm.maxCashInOut = " << s_maxCashInOut << L"\n"
            L"[CFG] kkm.maxPrice = " << s_maxPrice << L"\n"
            L"[CFG] kkm.maxQuantity = " << s_maxQuantity << L"\n"
            L"[LRN] kkm.connParams = {\n";

        try {
            std::filesystem::path directory { s_dbDirectory };
            bool nonFirst = false;
            for (auto const & entry: std::filesystem::directory_iterator { directory }) {
                if (entry.is_regular_file()) {
                    const std::filesystem::path & filePath { entry.path() };
                    std::wstring fileExt { filePath.extension().c_str() };
                    Text::lower(fileExt);
                    if (fileExt != L".json") {
                        continue;
                    }
                    Device::KnownConnParams connParams { entry.path() };
                    if (nonFirst) {
                        stream << L",\n";
                    } else {
                        nonFirst = true;
                    }
                    stream
                        << L"            \"" << connParams.serialNumber()
                        << L"\": \"" << static_cast<std::wstring>(connParams) << L'"';
                }
            }
        } catch (...) {}

        stream << L"\n      }\n";

        return stream;
    }
}
