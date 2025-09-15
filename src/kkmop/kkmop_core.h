// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "kkmop_strings.h"
#include <log/write.h>
#include <kkm/variables.h>
#include <kkm/device.h>
#include <cstdlib>
#include <iostream>
#include <optional>

inline std::optional<int> learnCmd(int argc, wchar_t ** argv) {
    if (argc < 3) {
        return std::nullopt;
    }

    for (int i = 2, n = 1; i < argc; ++i, ++n) {
        try {
            std::wstring serialNumber;

            {
                Kkm::Device::NewConnParams connParams { std::wstring { argv[i] } };
                Kkm::Device kkm { connParams, std::format(Main::Wcs::c_commandPrefix, n) };
                serialNumber.assign(kkm.serialNumber());
                ntsLogDebug(Main::Wcs::c_getKkmInfo, n, serialNumber);
                kkm.printHello();
                connParams.save(serialNumber);
            }

            ntsLogInfo(Main::Wcs::c_connParamsSaved, n, serialNumber);
        } catch (const Kkm::Failure & e) {
            ntsLogWarning(std::format(Main::Wcs::c_prefixedText, n, e.explain(Log::s_appendLocation)));
        }
    }

    return EXIT_SUCCESS;
}

inline std::optional<int> deviceCmd(const std::wstring & command, wchar_t * serial) try {
    Log::Console::s_level = Log::c_levelNone;

    if (command == L"base-status") {
        Nln::Json json(Nln::EmptyJsonObject);
        Kkm::Device kkm { Kkm::Device::KnownConnParams { serial }};
        Kkm::callUndetailedMethod(kkm, &Kkm::Device::getStatus, json);
        std::wcout << json;
        return EXIT_SUCCESS;
    }

    if (command == L"status") {
        Nln::Json json(Nln::EmptyJsonObject);
        Kkm::Device kkm { Kkm::Device::KnownConnParams { serial }};
        Kkm::collectDataFromMethods(
            json, kkm,
            &Kkm::Device::getStatus,
            &Kkm::Device::getShiftState,
            &Kkm::Device::getReceiptState,
            &Kkm::Device::getCashStat,
            &Kkm::Device::getFndtOfdExchangeStatus,
            &Kkm::Device::getFndtLastReceipt,
            &Kkm::Device::getFndtLastDocument,
            &Kkm::Device::getFndtErrors
        );
        std::wcout << json;
        return EXIT_SUCCESS;
    }

    if (command == L"full-status") {
        Nln::Json json(Nln::EmptyJsonObject);
        Kkm::Device kkm { Kkm::Device::KnownConnParams { serial } };
        Kkm::collectDataFromMethods(
            json, kkm,
            &Kkm::Device::getStatus,
            &Kkm::Device::getShiftState,
            &Kkm::Device::getReceiptState,
            &Kkm::Device::getCashStat,
            &Kkm::Device::getFndtOfdExchangeStatus,
            &Kkm::Device::getFndtFnInfo,
            &Kkm::Device::getFndtRegistrationInfo,
            &Kkm::Device::getFndtLastRegistration,
            &Kkm::Device::getFndtLastReceipt,
            &Kkm::Device::getFndtLastDocument,
            &Kkm::Device::getFndtErrors,
            &Kkm::Device::getFfdVersion,
            &Kkm::Device::getFwVersion
        );
        std::wcout << json;
        return EXIT_SUCCESS;
    }

    if (command == L"cash-stat") {
        Nln::Json json(Nln::EmptyJsonObject);
        Kkm::Device kkm { Kkm::Device::KnownConnParams { serial } };
        Kkm::callUndetailedMethod(kkm, &Kkm::Device::getCashStat, json);
        std::wcout << json;
        return EXIT_SUCCESS;
    }

    if (command == L"demo-print") {
        Nln::Json json(Nln::EmptyJsonObject);
        Kkm::Device kkm { Kkm::Device::KnownConnParams { serial } };
        Kkm::callUndetailedMethod(kkm, &Kkm::Device::printDemo, json);
        std::wcout << json;
        return EXIT_SUCCESS;
    }

    if (command == L"info") {
        Nln::Json json(Nln::EmptyJsonObject);
        Kkm::Device kkm { Kkm::Device::KnownConnParams { serial } };
        Kkm::callUndetailedMethod(kkm, &Kkm::Device::printInfo, json);
        std::wcout << json;
        return EXIT_SUCCESS;
    }

    if (command == L"fn-regs") {
        Nln::Json json(Nln::EmptyJsonObject);
        Kkm::Device kkm { Kkm::Device::KnownConnParams { serial } };
        Kkm::callUndetailedMethod(kkm, &Kkm::Device::printFnRegistrations, json);
        std::wcout << json;
        return EXIT_SUCCESS;
    }

    if (command == L"ofd-status") {
        Nln::Json json(Nln::EmptyJsonObject);
        Kkm::Device kkm { Kkm::Device::KnownConnParams { serial } };
        Kkm::callUndetailedMethod(kkm, &Kkm::Device::printOfdExchangeStatus, json);
        std::wcout << json;
        return EXIT_SUCCESS;
    }

    if (command == L"ofd-test") {
        Nln::Json json(Nln::EmptyJsonObject);
        Kkm::Device kkm { Kkm::Device::KnownConnParams { serial } };
        Kkm::callUndetailedMethod(kkm, &Kkm::Device::printOfdTest, json);
        std::wcout << json;
        return EXIT_SUCCESS;
    }

    if (command == L"shift-reports") {
        Nln::Json json(Nln::EmptyJsonObject);
        Kkm::Device kkm { Kkm::Device::KnownConnParams { serial } };
        Kkm::callUndetailedMethod(kkm, &Kkm::Device::printCloseShiftReports, json);
        std::wcout << json;
        return EXIT_SUCCESS;
    }

    if (command == L"last-document") {
        Nln::Json json(Nln::EmptyJsonObject);
        Kkm::Device kkm { Kkm::Device::KnownConnParams { serial } };
        Kkm::callUndetailedMethod(kkm, &Kkm::Device::printLastDocument, json);
        std::wcout << json;
        return EXIT_SUCCESS;
    }

    if (command == L"report-x") {
        Nln::Json json(Nln::EmptyJsonObject);
        Kkm::Device kkm { Kkm::Device::KnownConnParams { serial } };
        Kkm::callDetailedMethod(
            kkm, &Kkm::Device::reportX,
            { Kkm::s_cliOperatorName, Kkm::s_cliOperatorInn, false, false },
            json
        );
        std::wcout << json;
        return EXIT_SUCCESS;
    }

    if (command == L"close-shift") {
        Nln::Json json(Nln::EmptyJsonObject);
        Kkm::Device kkm { Kkm::Device::KnownConnParams { serial } };
        Kkm::callDetailedMethod(
            kkm, &Kkm::Device::closeShift,
            { Kkm::s_cliOperatorName, Kkm::s_cliOperatorInn, true, false },
            json
        );
        std::wcout << json;
        return EXIT_SUCCESS;
    }

    if (command == L"reset-state") {
        Nln::Json json(Nln::EmptyJsonObject);
        Kkm::Device kkm { Kkm::Device::KnownConnParams { serial } };
        Kkm::callDetailedMethod(
            kkm, &Kkm::Device::resetState,
            { Kkm::s_cliOperatorName, Kkm::s_cliOperatorInn, true, true },
            json
        );
        std::wcout << json;
        return EXIT_SUCCESS;
    }

    return std::nullopt;

} catch (const Basic::Failure & e) {
    std::wcout
        << Nln::Json::object({
            { Json::Mbs::c_successKey, false },
            { Json::Mbs::c_messageKey, Text::convert(e.explain(Log::s_appendLocation)) }
        });
    return EXIT_FAILURE;
} catch (const std::exception & e) {
    std::wcout
        << Nln::Json::object({
            { Json::Mbs::c_successKey, false },
            { Json::Mbs::c_messageKey, e.what() }
        });
    return EXIT_FAILURE;
} catch (...) {
    std::wcout
        << Nln::Json::object({
            { Json::Mbs::c_successKey, false },
            { Json::Mbs::c_messageKey, Basic::Mbs::c_somethingWrong }
        });
    return EXIT_FAILURE;
}
