// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include <cmake_options.h>
#include <build_options.h>
#include <lib/setcou16.h>
#include <lib/memprof.h>
#include "defaults.h"
#include "messages.h"
#include "log.h"
#include "kkm.h"
#include "http.h"
#include "static_handler.h"
#include "server.h"
#include "service.h"
#include "config.h"

namespace Mbs {
    using Json::Mbs::c_successKey;
    using Json::Mbs::c_messageKey;
}

std::wstring logLevelLabel(int level) {
    assert(level >= Log::c_levelDebug && level <= Log::c_levelNone);
    if (level == Log::c_levelNone) {
        return L"none";
    }
    if (Log::Wcs::c_levelLabels.contains(static_cast<Log::Level>(level))) {
        return Text::lowered(Log::Wcs::c_levelLabels.at(static_cast<Log::Level>(level)));
    }
    return Basic::Wcs::c_fallbackErrorMessage;
}

void usage(std::wostream & stream, const std::filesystem::path & path) {
    stream <<
        L"\n"
        L"Версия: " << BUILD_VERSION << L"\n"
        L"Использование: " << path.filename().c_str() << L" команда [аргумент ...]\n"
        L"Команды:\n"
        L"    help                   Вывести справку\n"
        L"    show-config            Вывести конфигурацию\n"
        L"    install                Установить сервер как службу Windows\n"
        L"    uninstall              Удалить службу Windows\n"
        L"    start                  Запустить службу Windows\n"
        L"    stop                   Остановить службу Windows\n"
        L"    restart                Перезапустить службу Windows\n"
        L"    foreground             Запустить сервер как foreground-процесс\n"
        L"    learn _пп_ [_пп_ ...]  Добавить ККМ\n"
        L"    base-status _сн_       Вывести базовый статус ККМ (п1)\n"
        L"    status _сн_            Вывести статус ККМ (п1)\n"
        L"    full-status _сн_       Вывести полный статус ККМ (п1)\n"
        L"    cash-stat _сн_         Вывести информации о наличных (п1)\n"
        L"    demo-print _сн_        Выполнить демо-печать (п2)\n"
        L"    info _сн_              Напечатать информацию о ККМ (п2)\n"
        L"    fn-regs _сн_           Напечатать итоги регистрации / перерегистрации (п2)\n"
        L"    ofd-status _сн_        Напечатать отчет о состоянии расчетов (п2)\n"
        L"    ofd-test _сн_          Тестировать подключение ККМ к ОФД (п2)\n"
        L"    shift-reports _сн_     Напечатать нераспечатанные отчеты о закрытии смены (п2)\n"
        L"    last-document _сн_     Напечатать копию последнего документа (п2)\n"
        L"    report-x _сн_          Печать Х-отчёта ККМ (п2)\n"
        L"    close-shift _сн_       Закрыть смену ККМ (п2)\n"
        L"    reset-state _сн_       Возврат ККМ к исходному состоянию (п2)\n"
        L"где _пп_ - параметры подключения (можно указать несколько)\n"
        L"    _сн_ - серийный номер ККМ\n"
        L"    (п1) - вывод в консоль в JSON-формате\n"
        L"    (п2) - печать документа + вывод в консоль в JSON-формате\n"
        L"\n";
}

int wmain(int argc, wchar_t ** argv, wchar_t ** envp) {
    START_MEMORY_PROFILING;
    FORCE_MEMORY_LEAK;

    bool jsonOut { false };

    try {
        if (argc > 1) {
            std::wstring command { Text::lowered(argv[1]) };

            if (command == L"help") {
                usage(std::wcout, argv[0]);
                return EXIT_SUCCESS;
            }

            Config::read(envp);

            if (command == L"show-config") {
                std::wstring securityLevel;
                if (Server::s_enableLegacyTls) {
                    securityLevel.assign(L"0");
                } else if (Server::s_securityLevel >= 0) {
                    securityLevel.assign(std::to_wstring(Server::s_securityLevel));
                } else {
                    securityLevel.assign(L"<openssl-default>");
                }
                std::wcout
                    << L"\n"
                    L"[BLD] main.version = \"" << BUILD_VERSION << L"\"\n"
                    L"[BLD] main.buildTimestamp = \"" << BUILD_TIMESTAMP << L"\"\n"
                    L"[BLD] main.buildType = \"" << BUILD_TYPE << L"\"\n"
                    L"[RTM] main.binaryFile = \"" << Config::s_binaryFile.c_str() << L"\"\n"
                    L"[RTM] main.workDirectory = \"" << Config::s_workDirectory.c_str() << L"\"\n"
                    L"[RTM] main.configDirectory = \"" << Config::s_configDirectory.c_str() << L"\"\n"
                    L"[RTM] main.configFile = \"" << Config::s_configFile.c_str() << L"\"\n"
                    L"[CFG] log.console.level = " << logLevelLabel(Log::Console::s_level) << L"\n"
                    L"[CFG] log.console.outputTimestamp = " << Text::Wcs::yesNo(Log::Console::s_outputTimestamp) << L"\n"
                    L"[CFG] log.console.outputLevel = " << Text::Wcs::yesNo(Log::Console::s_outputLevel) << L"\n"
                    L"[CFG] log.file.level = " << logLevelLabel(Log::File::s_level) << L"\n"
                    L"[CFG] log.file.directory = \"" << Log::File::s_directory << L"\"\n"
                    L"[CFG] log.eventLog.level = " << logLevelLabel(Log::EventLog::s_level) << L"\n"
                    L"[DEF] log.eventLog.source = \"" << Log::c_eventSource << L"\"\n"
                    L"[CFG] log.appendLocation = " << Text::Wcs::yesNo(Log::s_appendLocation) << L"\n"
                    L"[DEF] service.systemName = \"" << Service::c_systemName << L"\"\n"
                    L"[DEF] service.displayName = \"" << Service::c_displayName << L"\"\n"
                    L"[CFG] server.ipv4Only = " << Text::Wcs::yesNo(Server::s_ipv4Only) << L"\n"
                    L"[CFG] server.port = " << Server::s_port << L"\n"
                    L"[CFG] server.concurrencyLimit = " << Server::s_concurrencyLimit << L"\n"
                    L"[CFG] server.enableLegacyTls = " << Text::Wcs::yesNo(Server::s_enableLegacyTls) << L"\n"
                    L"[CFG] server.securityLevel = " << securityLevel << L"\n"
                    L"[CFG] server.certificateChainFile = \"" << Text::convert(Server::s_certificateChainFile) << L"\"\n"
                    L"[CFG] server.privateKeyFile = \"" << Text::convert(Server::s_privateKeyFile) << L"\"\n"
                    L"[CFG] server.privateKeyPassword = \"" << Text::convert(Server::s_privateKeyPassword) << L"\"\n"
                    L"[CFG] server.secret = \"" << Text::convert(Http::s_secret) << L"\"\n"
                    L"[CFG] server.loopbackWithoutSecret = " << Text::Wcs::yesNo(Http::s_loopbackWithoutSecret) << L"\n"
                    L"[CFG] server.enableStatic = " << Text::Wcs::yesNo(Http::s_enableStatic) << L"\n"
                    L"[CFG] server.staticDirectory = \"" << Http::s_staticDirectory << L"\"\n"
                    L"[CFG] server.mimeMapFile = \"" << Http::s_mimeMapFile << L"\"\n"
                    L"[CFG] server.enableUnknownType = " << Text::Wcs::yesNo(Http::s_enableUnknownType) << L"\n"
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
                    L"[LRN] kkm.connParams = { ";

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
                                std::wcout << L", ";
                            } else {
                                nonFirst = true;
                            }
                            std::wcout
                                << L'"' << connParams.serialNumber()
                                << L"\": \"" << static_cast<std::wstring>(connParams) << L'"';
                        }
                    }
                } catch (...) {}

                std::wcout << L" }\n" << std::endl;
                return EXIT_SUCCESS;
            }

            if (command == L"install") {
                Service::Control::install(std::format(L"\"{}\" service", Config::s_binaryFile.c_str()));
                ::Sleep(Basic::c_sleep);
                return EXIT_SUCCESS;
            }

            if (command == L"uninstall") {
                Service::Control::uninstall();
                ::Sleep(Basic::c_sleep);
                return EXIT_SUCCESS;
            }

            if (command == L"start") {
                Service::Control::start();
                ::Sleep(Basic::c_sleep);
                return EXIT_SUCCESS;
            }

            if (command == L"stop") {
                Service::Control::stop();
                ::Sleep(Basic::c_sleep);
                return EXIT_SUCCESS;
            }

            if (command == L"restart") {
                Service::Control::stop();
                ::Sleep(Basic::c_sleep);
                Service::Control::start();
                ::Sleep(Basic::c_sleep);
                return EXIT_SUCCESS;
            }

            if (command == L"foreground") {
                Server::run();
                return EXIT_SUCCESS;
            }

            if (command == L"service") {
                std::atexit([] {
                    Log::File::close();
                    Log::EventLog::close();
                });
                Log::Console::s_level = Log::c_levelNone;
                Log::File::open();
                Log::EventLog::open();
                Service::Worker::run();
                return EXIT_SUCCESS;
            }

            if (command == L"learn" && argc > 2) {
                for (int i = 2, n = 1; i < argc; ++i, ++n) {
                    try {
                        std::wstring serialNumber;

                        {
                            Kkm::Device::NewConnParams connParams { std::wstring { argv[i] } };
                            Kkm::Device kkm { connParams, std::format(Basic::Wcs::c_commandPrefix, n) };
                            serialNumber.assign(kkm.serialNumber());
                            ntsLogDebug(Basic::Wcs::c_getKkmInfo, n, serialNumber);
                            kkm.printHello();
                            connParams.save(serialNumber);
                        }

                        ntsLogInfo(Basic::Wcs::c_connParamsSaved, n, serialNumber);
                    } catch (const Kkm::Failure & e) {
                        ntsLogWarning(std::format(Basic::Wcs::c_prefixedText, n, e.explain()));
                    }
                }
                return EXIT_SUCCESS;
            }

            jsonOut = true;
            Log::Console::s_level = Log::c_levelNone;

            if (command == L"base-status" && argc == 3) {
                Nln::Json json(Nln::EmptyJsonObject);
                Kkm::Device kkm { Kkm::Device::KnownConnParams { argv[2] }};
                Kkm::callUndetailedMethod(kkm, &Kkm::Device::getStatus, json);
                std::wcout << Text::convert(json.dump(4));
                return EXIT_SUCCESS;
            }

            if (command == L"status" && argc == 3) {
                Nln::Json json(Nln::EmptyJsonObject);
                Kkm::Device kkm { Kkm::Device::KnownConnParams { argv[2] }};
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
                std::wcout << Text::convert(json.dump(4));
                return EXIT_SUCCESS;
            }

            if (command == L"full-status" && argc == 3) {
                Nln::Json json(Nln::EmptyJsonObject);
                Kkm::Device kkm { Kkm::Device::KnownConnParams { argv[2] } };
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
                std::wcout << Text::convert(json.dump(4));
                return EXIT_SUCCESS;
            }

            if (command == L"cash-stat" && argc == 3) {
                Nln::Json json(Nln::EmptyJsonObject);
                Kkm::Device kkm { Kkm::Device::KnownConnParams { argv[2] } };
                Kkm::callUndetailedMethod(kkm, &Kkm::Device::getCashStat, json);
                std::wcout << Text::convert(json.dump(4));
                return EXIT_SUCCESS;
            }

            if (command == L"demo-print" && argc == 3) {
                Nln::Json json(Nln::EmptyJsonObject);
                Kkm::Device kkm { Kkm::Device::KnownConnParams { argv[2] } };
                Kkm::callUndetailedMethod(kkm, &Kkm::Device::printDemo, json);
                std::wcout << Text::convert(json.dump(4));
                return EXIT_SUCCESS;
            }

            if (command == L"info" && argc == 3) {
                Nln::Json json(Nln::EmptyJsonObject);
                Kkm::Device kkm { Kkm::Device::KnownConnParams { argv[2] } };
                Kkm::callUndetailedMethod(kkm, &Kkm::Device::printInfo, json);
                std::wcout << Text::convert(json.dump(4));
                return EXIT_SUCCESS;
            }

            if (command == L"fn-regs" && argc == 3) {
                Nln::Json json(Nln::EmptyJsonObject);
                Kkm::Device kkm { Kkm::Device::KnownConnParams { argv[2] } };
                Kkm::callUndetailedMethod(kkm, &Kkm::Device::printFnRegistrations, json);
                std::wcout << Text::convert(json.dump(4));
                return EXIT_SUCCESS;
            }

            if (command == L"ofd-status" && argc == 3) {
                Nln::Json json(Nln::EmptyJsonObject);
                Kkm::Device kkm { Kkm::Device::KnownConnParams { argv[2] } };
                Kkm::callUndetailedMethod(kkm, &Kkm::Device::printOfdExchangeStatus, json);
                std::wcout << Text::convert(json.dump(4));
                return EXIT_SUCCESS;
            }

            if (command == L"ofd-test" && argc == 3) {
                Nln::Json json(Nln::EmptyJsonObject);
                Kkm::Device kkm { Kkm::Device::KnownConnParams { argv[2] } };
                Kkm::callUndetailedMethod(kkm, &Kkm::Device::printOfdTest, json);
                std::wcout << Text::convert(json.dump(4));
                return EXIT_SUCCESS;
            }

            if (command == L"shift-reports" && argc == 3) {
                Nln::Json json(Nln::EmptyJsonObject);
                Kkm::Device kkm { Kkm::Device::KnownConnParams { argv[2] } };
                Kkm::callUndetailedMethod(kkm, &Kkm::Device::printCloseShiftReports, json);
                std::wcout << Text::convert(json.dump(4));
                return EXIT_SUCCESS;
            }

            if (command == L"last-document" && argc == 3) {
                Nln::Json json(Nln::EmptyJsonObject);
                Kkm::Device kkm { Kkm::Device::KnownConnParams { argv[2] } };
                Kkm::callUndetailedMethod(kkm, &Kkm::Device::printLastDocument, json);
                std::wcout << Text::convert(json.dump(4));
                return EXIT_SUCCESS;
            }

            if (command == L"report-x" && argc == 3) {
                Nln::Json json(Nln::EmptyJsonObject);
                Kkm::Device kkm { Kkm::Device::KnownConnParams { argv[2] } };
                Kkm::callDetailedMethod(
                    kkm, &Kkm::Device::reportX,
                    { Kkm::s_cliOperatorName, Kkm::s_cliOperatorInn, false, false },
                    json
                );
                std::wcout << Text::convert(json.dump(4));
                return EXIT_SUCCESS;
            }

            if (command == L"close-shift" && argc == 3) {
                Nln::Json json(Nln::EmptyJsonObject);
                Kkm::Device kkm { Kkm::Device::KnownConnParams { argv[2] } };
                Kkm::callDetailedMethod(
                    kkm, &Kkm::Device::closeShift,
                    { Kkm::s_cliOperatorName, Kkm::s_cliOperatorInn, true, false },
                    json
                );
                std::wcout << Text::convert(json.dump(4));
                return EXIT_SUCCESS;
            }

            if (command == L"reset-state" && argc == 3) {
                Nln::Json json(Nln::EmptyJsonObject);
                Kkm::Device kkm { Kkm::Device::KnownConnParams { argv[2] } };
                Kkm::callDetailedMethod(
                    kkm, &Kkm::Device::resetState,
                    { Kkm::s_cliOperatorName, Kkm::s_cliOperatorInn, true, true },
                    json
                );
                std::wcout << Text::convert(json.dump(4));
                return EXIT_SUCCESS;
            }
        }

        usage(std::wcerr, argv[0]);

    } catch (const Basic::Failure & e) {
        if (jsonOut) {
            std::wcout
                << Text::convert(
                    Nln::Json::object(
                        { { Mbs::c_successKey, false }, { Mbs::c_messageKey, Text::convert(e.explain()) } }
                    ).dump(4)
                );
        } else {
            ntsLogError(e);
        }
    } catch (const std::exception & e) {
        if (jsonOut) {
            std::wcout
                << Text::convert(
                    Nln::Json::object(
                        { { Mbs::c_successKey, false }, { Mbs::c_messageKey, e.what() } }
                    ).dump(4)
                );
        } else {
            ntsLogError(e);
        }
    } catch (...) {
        if (jsonOut) {
            std::wcout
                << Text::convert(
                    Nln::Json::object(
                        { { Mbs::c_successKey, false }, { Mbs::c_messageKey, Basic::Mbs::c_somethingWrong } }
                    ).dump(4)
                );
        } else {
            ntsLogError(Basic::Wcs::c_somethingWrong);
        }
    }

    return EXIT_FAILURE;
}
