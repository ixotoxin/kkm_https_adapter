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
                Json::handleKey(json, "dbDirectory", Kkm::s_dbDirectory, Text::Wcs::trim());
                Json::handleKey(json, "defaultBaudRate", Kkm::s_defaultBaudRate, Kkm::Wcs::c_allowedBaudRate, path);
                Json::handleKey(
                    json, "defaultLineLength",
                    Kkm::s_defaultLineLength,
                    Numeric::between(Kkm::c_minLineLength, Kkm::c_maxLineLength),
                    path
                );
                Json::handleKey(
                    json, "timeZone",
                    Kkm::s_timeZone, Kkm::Mbs::c_timeZoneMap,
                    [] (auto value) { Kkm::s_timeZoneConfigured = true; return value; },
                    path
                );
                Json::handleKey(
                    json, "documentClosingTimeout",
                    Kkm::s_documentClosingTimeout,
                    Numeric::between(Kkm::c_minDocumentClosingTimeout, Kkm::c_maxDocumentClosingTimeout),
                    path
                );
                Json::handleKey(
                    json, "cliOperator",
                    [] (const Nln::Json & json, const std::wstring & path) -> bool {
                        Json::handleKey(
                            json, "name",
                            Kkm::s_cliOperatorName,
                            Text::Wcs::noEmpty(Text::Wcs::trim()),
                            path
                        );
                        Json::handleKey(json, "inn", Kkm::s_cliOperatorInn, Text::Wcs::trim(), path);
                        return true;
                    },
                    path
                );
                Json::handleKey(json, "customerAccountField", Kkm::s_customerAccountField, path);
                Json::handleKey(
                    json, "maxCashInOut",
                    Kkm::s_maxCashInOut,
                    Numeric::between(Kkm::c_minMaxCashInOut, Kkm::c_maxMaxCashInOut),
                    path
                );
                Json::handleKey(
                    json, "maxPrice",
                    Kkm::s_maxPrice,
                    Numeric::between(Kkm::c_minMaxPrice, Kkm::c_maxMaxPrice),
                    path
                );
                Json::handleKey(
                    json, "maxQuantity",
                    Kkm::s_maxQuantity,
                    Numeric::between(Kkm::c_minMaxQuantity, Kkm::c_maxMaxQuantity),
                    path
                );
                return true;
            }
        );
    }

    std::wostream & vars(std::wostream & stream) {
        stream <<
            L"[CFG] kkm.dbDirectory = \"" << Kkm::s_dbDirectory << L"\"\n"
            L"[CFG] kkm.defaultBaudRate = " << Kkm::s_defaultBaudRate << L"\n"
            L"[CFG] kkm.defaultLineLength = " << Kkm::s_defaultLineLength << L"\n"
            L"[CFG] kkm.timeZone = tz" << static_cast<int>(Kkm::s_timeZone) << L"\n"
            L"[CFG] kkm.documentClosingTimeout = " << Kkm::s_documentClosingTimeout << L"\n"
            L"[CFG] kkm.cliOperator.name = \"" << Kkm::s_cliOperatorName << L"\"\n"
            L"[CFG] kkm.cliOperator.inn = \"" << Kkm::s_cliOperatorInn << L"\"\n"
            L"[CFG] kkm.customerAccountField = \"" << Kkm::s_customerAccountField << L"\"\n"
            L"[CFG] kkm.maxCashInOut = " << Kkm::s_maxCashInOut << L"\n"
            L"[CFG] kkm.maxPrice = " << Kkm::s_maxPrice << L"\n"
            L"[CFG] kkm.maxQuantity = " << Kkm::s_maxQuantity << L"\n"
            L"[LRN] kkm.connParams = {\n";

        try {
            std::filesystem::path directory { Kkm::s_dbDirectory };
            bool nonFirst = false;
            for (auto const & entry: std::filesystem::directory_iterator { directory }) {
                if (entry.is_regular_file()) {
                    const std::filesystem::path & filePath { entry.path() };
                    std::wstring fileExt { filePath.extension().c_str() };
                    Text::lower(fileExt);
                    if (fileExt != L".json") {
                        continue;
                    }
                    Kkm::Device::KnownConnParams connParams { entry.path() };
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
