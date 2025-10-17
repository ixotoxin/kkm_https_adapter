// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "kkmjl_strings.h"
#include <lib/except.h>
#include <lib/text.h>
#include <log/write.h>
#include <kkm/strings.h>
#include <kkm/device.h>
#include <kkm/impex.h>
#include <kkm/callhelpers.h>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <fstream>

namespace KkmJsonLoader {
    using namespace Kkm;

    [[nodiscard]]
    inline int exec(wchar_t * serialNumber, const wchar_t * fileName) {
        Log::asBackgroundProcess();
        const std::filesystem::path path { fileName };
        std::ifstream file { path };
        if (!file.is_open() || !file.good()) {
            throw Basic::Failure(LIB_WFMT(Basic::Wcs::c_couldntReadFile, path.native())); // NOLINT(*-exception-baseclass)
        }

        const Nln::Json details(Nln::Json::parse(file));
        if (!details.is_object()) {
            throw Basic::Failure(Json::Wcs::c_jsonObjectWasExpected); // NOLINT(*-exception-baseclass)
        }

        std::wstring serial { serialNumber };
        if (serial == L"-") {
            serial.clear();
        } else if (serial == L"+") {
            if (details.contains(Mbs::c_id)) {
                serial.assign(Text::convert(details.at(Mbs::c_id).get<std::string>()));
            } else {
                serial.clear();
            }
        }

        std::wstring query {};
        if (details.contains(Mbs::c_query)) {
            query.assign(Text::convert(Text::lowered(details.at(Mbs::c_query).get<std::string>())));
        } else {
            throw Basic::Failure(KKM_FMT(Kkm::Mbs::c_requiresProperty, Mbs::c_query)); // NOLINT(*-exception-baseclass)
        }

        Nln::Json result(Nln::EmptyJsonObject);
        if (query == L"learn") {
            std::wstring connString;
            const bool found { Json::handleKey(details, "connParams", connString) };
            if (!found) {
                throw Basic::Failure(KKM_FMT(Kkm::Mbs::c_requiresProperty, "connParams")); // NOLINT(*-exception-baseclass)
            }
            NewConnParams connParams { connString };
            Device kkm { connParams };
            connParams.save(kkm.serialNumber());
            StatusResult statusResult {};
            kkm.getStatus(statusResult);
            kkm.printHello();
            result << statusResult;
        } else if (query == L"base-status") {
            callMethod(Device { KnownConnParams { serial } }, &Device::getStatus, result);
        } else if (query == L"status") {
            collectDataFromMethods(
                result,
                Device { KnownConnParams { serial } },
                &Device::getStatus,
                &Device::getShiftState,
                &Device::getReceiptState,
                &Device::getCashStat,
                &Device::getFndtOfdExchangeStatus,
                &Device::getFndtLastReceipt,
                &Device::getFndtLastDocument,
                &Device::getFndtErrors
            );
        } else if (query == L"full-status") {
            collectDataFromMethods(
                result,
                Device { KnownConnParams { serial } },
                &Device::getStatus,
                &Device::getShiftState,
                &Device::getReceiptState,
                &Device::getCashStat,
                &Device::getFndtOfdExchangeStatus,
                &Device::getFndtFnInfo,
                &Device::getFndtRegistrationInfo,
                &Device::getFndtLastRegistration,
                &Device::getFndtLastReceipt,
                &Device::getFndtLastDocument,
                &Device::getFndtErrors,
                &Device::getFfdVersion,
                &Device::getFwVersion
            );
        } else if (query == L"print-demo") {
            callMethod(Device { KnownConnParams { serial } }, &Device::printDemo, result);
        } else if (query == L"print-non-fiscal-doc") {
            callMethod(Device { KnownConnParams { serial } }, &Device::printNonFiscalDocument, details, result);
        } else if (query == L"print-info") {
            callMethod(Device { KnownConnParams { serial } }, &Device::printInfo, result);
        } else if (query == L"print-fn-registrations") {
            callMethod(Device { KnownConnParams { serial } }, &Device::printFnRegistrations, result);
        } else if (query == L"print-ofd-exchange-status") {
            callMethod(Device { KnownConnParams { serial } }, &Device::printOfdExchangeStatus, result);
        } else if (query == L"print-ofd-test") {
            callMethod(Device { KnownConnParams { serial } }, &Device::printOfdTest, result);
        } else if (query == L"print-close-shift-reports") {
            callMethod(Device { KnownConnParams { serial } }, &Device::printCloseShiftReports, result);
        } else if (query == L"print-last-document") {
            callMethod(Device { KnownConnParams { serial } }, &Device::printLastDocument, result);
        } else if (query == L"cash-stat") {
            callMethod(Device { KnownConnParams { serial } }, &Device::getCashStat, result);
        } else if (query == L"cash-in") {
            callMethod(Device { KnownConnParams { serial } }, &Device::registerCashIn, details, result);
        } else if (query == L"cash-out") {
            callMethod(Device { KnownConnParams { serial } }, &Device::registerCashOut, details, result);
        } else if (query == L"sell") {
            callMethod(Device { KnownConnParams { serial } }, &Device::registerSell, details, result);
        } else if (query == L"sell-return") {
            callMethod(Device { KnownConnParams { serial } }, &Device::registerSellReturn, details, result);
        } else if (query == L"report-z" || query == L"close-shift") {
            callMethod(Device { KnownConnParams { serial } }, &Device::closeShift, details, result);
        } else if (query == L"report-x") {
            callMethod(Device { KnownConnParams { serial } }, &Device::reportX, details, result);
        } else if (query == L"reset-state") {
            callMethod(Device { KnownConnParams { serial } }, &Device::resetState, details, result);
        } else {
            throw Basic::Failure(KKM_FMT(Kkm::Mbs::c_requiresProperty, Mbs::c_query)); // NOLINT(*-exception-baseclass)
        }

        std::wcout << result;
        return EXIT_SUCCESS;
    }

    inline void printError(const std::wstring_view error) {
        std::wcerr
            << L"{\n"
            L"    \"" << Text::convert(Json::Mbs::c_successKey) << L"\": false,\n"
            L"    \"" << Text::convert(Json::Mbs::c_messageKey) << L"\": \"" << error << L"\"\n"
            L"}\n";
    }

    [[nodiscard, maybe_unused]]
    inline int safeExec(wchar_t * serialNumber, const wchar_t * fileName) noexcept try {
        return exec(serialNumber, fileName);
    } catch (const Basic::Failure & e) {
        printError(e.explain(Log::s_appendLocation));
        return EXIT_FAILURE;
    } catch (const std::exception & e) {
        printError(Text::convert(e.what()));
        return EXIT_FAILURE;
    } catch (...) {
        printError(Basic::Wcs::c_somethingWrong);
        return EXIT_FAILURE;
    }
}
