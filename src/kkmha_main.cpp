// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "library/basic.h"
#include <fcntl.h>
#include <io.h>
#include "defaults.h"
#include "messages.h"
#include "log.h"
#include "kkm.h"
#include "http.h"
#include "static_handler.h"
#include "server.h"
#include "service.h"
#include "config.h"

void usage(std::wostream & stream, const std::filesystem::path & path) {
    stream <<
        L"\n"
        L"Использование: " << path.filename().c_str() << L" команда [аргумент ...]\n"
        L"Команды:\n"
        L"    help                   Вывести справку\n"
        L"    show-config            Вывести конфигурацию\n"
        L"    install                Установить сервер как службу Windows\n"
        L"    uninstall              Удалить службу Windows\n"
        L"    start                  Запустить службу\n"
        L"    stop                   Остановить службу\n"
        L"    restart                Перезапустить службу\n"
        L"    foreground             Запустить сервер как foreground-процесс\n"
        L"    learn _пп_ [_пп_ ...]  Добавить ККМ\n"
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
    // ::SetConsoleOutputCP(CP_UTF8);
    // ::SetConsoleCP(CP_UTF8);
    // ::setlocale(LC_ALL, ".UTF8");
    ::_setmode(::_fileno(stdout), _O_U16TEXT);
    ::_setmode(::_fileno(stderr), _O_U16TEXT);

    ENABLE_MEMORY_PROFILING;
    MEMORY_LEAK;

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
                std::wcout << L"\n"
                    L"[RTM] main.binaryFile = \"" << Config::s_binaryFile.c_str() << L"\"\n"
                    L"[RTM] main.workDirectory = \"" << Config::s_workDirectory.c_str() << L"\"\n"
                    L"[RTM] main.configDirectory = \"" << Config::s_configDirectory.c_str() << L"\"\n"
                    L"[RTM] main.configFile = \"" << Config::s_configFile.c_str() << L"\"\n"
                    L"[CFG] log.console.level = \"" << Log::label(Log::Console::s_level) << L"\"\n"
                    L"[CFG] log.console.outputTimestamp = " << Text::Wcs::yesNo(Log::Console::s_outputTimestamp) << L"\n"
                    L"[CFG] log.console.outputLevel = " << Text::Wcs::yesNo(Log::Console::s_outputLevel) << L"\n"
                    L"[CFG] log.file.level = \"" << Log::label(Log::File::s_level) << L"\"\n"
                    L"[CFG] log.file.directory = \"" << Log::File::s_directory << L"\"\n"
                    L"[CFG] log.eventLog.level = \"" << Log::label(Log::EventLog::s_level) << L"\"\n"
                    L"[DEF] log.eventLog.source = \"" << Log::c_eventSource << L"\"\n"
                    L"[CFG] log.appendLocation = " << Text::Wcs::yesNo(Log::s_appendLocation) << L"\n"
                    L"[DEF] service.systemName = \"" << Service::c_systemName << L"\"\n"
                    L"[DEF] service.displayName = \"" << Service::c_displayName << L"\"\n"
                    L"[CFG] server.ipv4Only = " << Text::Wcs::yesNo(Server::s_ipv4Only) << L"\n"
                    L"[CFG] server.port = " << Server::s_port << L"\n"
                    L"[CFG] server.enableLegacyTls = " << Text::Wcs::yesNo(Server::s_enableLegacyTls) << L"\n"
                    L"[CFG] server.securityLevel = " << Server::s_securityLevel << L"\n"
                    L"[CFG] server.certificateChainFile = \"" << Text::convert(Server::s_certificateChainFile) << L"\"\n"
                    L"[CFG] server.privateKeyFile = \"" << Text::convert(Server::s_privateKeyFile) << L"\"\n"
                    L"[CFG] server.privateKeyPassword = \"" << Text::convert(Server::s_privateKeyPassword) << L"\"\n"
                    L"[CFG] server.secret = \"" << Text::convert(Http::s_secret) << L"\"\n"
                    L"[CFG] server.loopbackWithoutSecret = " << Text::Wcs::yesNo(Http::s_loopbackWithoutSecret) << L"\n"
                    L"[CFG] server.enableStatic = " << Text::Wcs::yesNo(Http::s_enableStatic) << L"\n"
                    L"[CFG] server.staticDirectory = \"" << Http::s_staticDirectory << L"\"\n"
                    L"[CFG] server.mimeMapFile = \"" << Http::s_mimeMapFile << L"\"\n"
                    L"[CFG] server.enableUnknownType = " << Text::Wcs::yesNo(Http::s_enableUnknownType) << L"\n"
                    L"[CFG] kkm.directory = " << Kkm::s_directory << L"\n"
                    L"[CFG] kkm.defaultBaudRate = " << Kkm::s_defaultBaudRate << L"\n"
                    L"[CFG] kkm.defaultLineLength = " << Kkm::s_defaultLineLength << L"\n"
                    L"[CFG] kkm.cliOperator.name = \"" << Kkm::s_cliOperatorName << L"\"\n"
                    L"[CFG] kkm.cliOperator.inn = \"" << Kkm::s_cliOperatorInn << L"\"\n"
                    L"[CFG] kkm.customerAccountField = \"" << Kkm::s_customerAccountField << L"\"\n"
                    L"[CFG] kkm.documentClosedChecking.attempts = \"" << Kkm::s_docClosedCheckingAttempts << L"\"\n"
                    L"[CFG] kkm.documentClosedChecking.waiting = \"" << Kkm::s_docClosedCheckingWaiting << L"\"\n"
                    L"[CFG] kkm.connParams = { ";

                try {
                    std::filesystem::path directory { Kkm::s_directory };
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

                std::wcout << L" }" << std::endl;
                return EXIT_SUCCESS;
            }

            if (command == L"install") {
                Service::install(std::format(L"\"{}\" service", Config::s_binaryFile.c_str()));
                return EXIT_SUCCESS;
            }

            if (command == L"uninstall") {
                Service::uninstall();
                return EXIT_SUCCESS;
            }

            if (command == L"start") {
                Service::start();
                ::Sleep(500);
                return EXIT_SUCCESS;
            }

            if (command == L"stop") {
                Service::stop();
                ::Sleep(500);
                return EXIT_SUCCESS;
            }

            if (command == L"restart") {
                Service::stop();
                ::Sleep(500);
                Service::start();
                ::Sleep(500);
                return EXIT_SUCCESS;
            }

            if (command == L"foreground") {
                Server::run();
                return EXIT_SUCCESS;
            }

            if (command == L"service") {
                std::atexit([] { Log::File::close(); Log::EventLog::close(); });
                Log::Console::s_level = Log::c_levelNone;
                Log::File::open();
                Log::EventLog::open();
                Service::run();
                // Log::EventLog::close();
                // Log::File::close();
                return EXIT_SUCCESS;
            }

            if (command == L"learn" && argc > 2) {
                for (int i = 2, n = 1; i < argc; ++i, ++n) {
                    try {
                        std::wstring serialNumber;
                        {
                            Kkm::Device::NewConnParams connParams { std::wstring { argv[i] }};
                            Kkm::Device kkm { connParams, std::format(Basic::Wcs::c_commandPrefix, n) };
                            serialNumber.assign(kkm.serialNumber());
                            ntsLogDebug(Basic::Wcs::c_getKkmInfo, n, serialNumber);
                            kkm.printHello();
                            connParams.save(serialNumber);
                        }
                        ntsLogInfo(Basic::Wcs::c_connParamsSaved, n, serialNumber);

                    // TODO: Исправить перехват исключений
                    } catch (const Basic::Failure & e) {
                        ntsLogWarning(std::format(Basic::Wcs::c_prefixedText, n, e.explain()));
                    } catch (const std::exception & e) {
                        ntsLogWarning(std::format(Basic::Wcs::c_prefixedText, n, Text::convert(e.what())));
                    } catch (...) {
                        ntsLogWarning(std::format(Basic::Wcs::c_prefixedText, n, Kkm::Wcs::c_invalidConnParams));
                    }
                }
                return EXIT_SUCCESS;
            }

            if (command == L"status" && argc == 3) {
                Log::Console::s_level = Log::c_levelNone;
                jsonOut = true;
                Nln::Json json(Nln::EmptyJsonObject);
                {
                    Kkm::Device::KnownConnParams connParams { argv[2] };
                    Kkm::Device kkm { connParams };
                    Kkm::Device::Call::StatusResult result {};
                    kkm.getStatus(result);
                    result.exportTo(json);
                }
                std::wcout << Text::convert(json.dump(4));
                return EXIT_SUCCESS;
            }

            if (command == L"full-status" && argc == 3) {
                Log::Console::s_level = Log::c_levelNone;
                jsonOut = true;
                Nln::Json json(Nln::EmptyJsonObject);
                {
                    Kkm::Device::KnownConnParams connParams { argv[2] };
                    Kkm::Device kkm { connParams };
                    {
                        Kkm::Device::Call::StatusResult result;
                        kkm.getStatus(result);
                        result.exportTo(json);
                    }
                    {
                        Kkm::Device::Call::ShiftStateResult result;
                        kkm.getShiftState(result);
                        result.exportTo(json);
                    }
                    {
                        Kkm::Device::Call::ReceiptStateResult result;
                        kkm.getReceiptState(result);
                        result.exportTo(json);
                    }
                    {
                        Kkm::Device::Call::CashStatResult result;
                        kkm.getCashStat(result);
                        result.exportTo(json);
                    }
                    {
                        Kkm::Device::Call::FndtOfdExchangeStatusResult result;
                        kkm.getFndtOfdExchangeStatus(result);
                        result.exportTo(json);
                    }
                    {
                        Kkm::Device::Call::FndtFnInfoResult result;
                        kkm.getFndtFnInfo(result);
                        result.exportTo(json);
                    }
                    {
                        Kkm::Device::Call::FndtLastRegistrationResult result;
                        kkm.getFndtLastRegistration(result);
                        result.exportTo(json);
                    }
                    {
                        Kkm::Device::Call::FndtLastReceiptResult result;
                        kkm.getFndtLastReceipt(result);
                        result.exportTo(json);
                    }
                    {
                        Kkm::Device::Call::FndtLastDocumentResult result;
                        kkm.getFndtLastDocument(result);
                        result.exportTo(json);
                    }
                    {
                        Kkm::Device::Call::FndtErrorsResult result;
                        kkm.getFndtErrors(result);
                        result.exportTo(json);
                    }
                    {
                        Kkm::Device::Call::VersionResult result;
                        kkm.getVersion(result);
                        result.exportTo(json);
                    }
                }
                std::wcout << Text::convert(json.dump(4));
                return EXIT_SUCCESS;
            }

            if (command == L"cash-stat" && argc == 3) {
                Log::Console::s_level = Log::c_levelNone;
                jsonOut = true;
                Nln::Json json(Nln::EmptyJsonObject);
                {
                    Kkm::Device::KnownConnParams connParams { argv[2] };
                    Kkm::Device kkm { connParams };
                    Kkm::Device::Call::CashStatResult result {};
                    kkm.getCashStat(result);
                    result.exportTo(json);
                }
                std::wcout << Text::convert(json.dump(4));
                return EXIT_SUCCESS;
            }

            if (command == L"demo-print" && argc == 3) {
                Log::Console::s_level = Log::c_levelNone;
                jsonOut = true;
                Nln::Json json(Nln::EmptyJsonObject);
                {
                    Kkm::Device::KnownConnParams connParams { argv[2] };
                    Kkm::Device kkm { connParams };
                    Kkm::Device::Call::Result result {};
                    kkm.printDemo(result);
                    result.exportTo(json);
                }
                std::wcout << Text::convert(json.dump(4));
                return EXIT_SUCCESS;
            }

            if (command == L"info" && argc == 3) {
                Log::Console::s_level = Log::c_levelNone;
                jsonOut = true;
                Nln::Json json(Nln::EmptyJsonObject);
                {
                    Kkm::Device::KnownConnParams connParams { argv[2] };
                    Kkm::Device kkm { connParams };
                    Kkm::Device::Call::Result result {};
                    kkm.printInfo(result);
                    result.exportTo(json);
                }
                std::wcout << Text::convert(json.dump(4));
                return EXIT_SUCCESS;
            }

            if (command == L"fn-regs" && argc == 3) {
                Log::Console::s_level = Log::c_levelNone;
                jsonOut = true;
                Nln::Json json(Nln::EmptyJsonObject);
                {
                    Kkm::Device::KnownConnParams connParams { argv[2] };
                    Kkm::Device kkm { connParams };
                    Kkm::Device::Call::Result result {};
                    kkm.printFnRegistrations(result);
                    result.exportTo(json);
                }
                std::wcout << Text::convert(json.dump(4));
                return EXIT_SUCCESS;
            }

            if (command == L"ofd-status" && argc == 3) {
                Log::Console::s_level = Log::c_levelNone;
                jsonOut = true;
                Nln::Json json(Nln::EmptyJsonObject);
                {
                    Kkm::Device::KnownConnParams connParams { argv[2] };
                    Kkm::Device kkm { connParams };
                    Kkm::Device::Call::Result result {};
                    kkm.printOfdExchangeStatus(result);
                    result.exportTo(json);
                }
                std::wcout << Text::convert(json.dump(4));
                return EXIT_SUCCESS;
            }

            if (command == L"ofd-test" && argc == 3) {
                Log::Console::s_level = Log::c_levelNone;
                jsonOut = true;
                Nln::Json json(Nln::EmptyJsonObject);
                {
                    Kkm::Device::KnownConnParams connParams { argv[2] };
                    Kkm::Device kkm { connParams };
                    Kkm::Device::Call::Result result {};
                    kkm.printOfdTest(result);
                    result.exportTo(json);
                }
                std::wcout << Text::convert(json.dump(4));
                return EXIT_SUCCESS;
            }

            if (command == L"shift-reports" && argc == 3) {
                Log::Console::s_level = Log::c_levelNone;
                jsonOut = true;
                Nln::Json json(Nln::EmptyJsonObject);
                {
                    Kkm::Device::KnownConnParams connParams { argv[2] };
                    Kkm::Device kkm { connParams };
                    Kkm::Device::Call::Result result {};
                    kkm.printCloseShiftReports(result);
                    result.exportTo(json);
                }
                std::wcout << Text::convert(json.dump(4));
                return EXIT_SUCCESS;
            }

            if (command == L"last-document" && argc == 3) {
                Log::Console::s_level = Log::c_levelNone;
                jsonOut = true;
                Nln::Json json(Nln::EmptyJsonObject);
                {
                    Kkm::Device::KnownConnParams connParams { argv[2] };
                    Kkm::Device kkm { connParams };
                    Kkm::Device::Call::Result result {};
                    kkm.printLastDocument(result);
                    result.exportTo(json);
                }
                std::wcout << Text::convert(json.dump(4));
                return EXIT_SUCCESS;
            }

            if (command == L"report-x" && argc == 3) {
                Log::Console::s_level = Log::c_levelNone;
                jsonOut = true;
                Nln::Json json(Nln::EmptyJsonObject);
                {
                    Kkm::Device::KnownConnParams connParams { argv[2] };
                    Kkm::Device kkm { connParams };
                    Kkm::Device::Call::CloseDetails details {
                        Kkm::s_cliOperatorName,
                        Kkm::s_cliOperatorInn,
                        true,
                        true
                    };
                    Kkm::Device::Call::Result result {};
                    kkm.reportX(details, result);
                    result.exportTo(json);
                }
                std::wcout << Text::convert(json.dump(4));
                return EXIT_SUCCESS;
            }

            if (command == L"close-shift" && argc == 3) {
                Log::Console::s_level = Log::c_levelNone;
                jsonOut = true;
                Nln::Json json(Nln::EmptyJsonObject);
                {
                    Kkm::Device::KnownConnParams connParams { argv[2] };
                    Kkm::Device kkm { connParams };
                    Kkm::Device::Call::CloseDetails details {
                        Kkm::s_cliOperatorName,
                        Kkm::s_cliOperatorInn,
                        true,
                        true
                    };
                    Kkm::Device::Call::Result result {};
                    kkm.closeShift(details, result);
                    result.exportTo(json);
                }
                std::wcout << Text::convert(json.dump(4));
                return EXIT_SUCCESS;
            }

            if (command == L"reset-state" && argc == 3) {
                Log::Console::s_level = Log::c_levelNone;
                jsonOut = true;
                Nln::Json json(Nln::EmptyJsonObject);
                {
                    Kkm::Device::KnownConnParams connParams { argv[2] };
                    Kkm::Device kkm { connParams };
                    Kkm::Device::Call::CloseDetails details {
                        Kkm::s_cliOperatorName,
                        Kkm::s_cliOperatorInn,
                        true,
                        true
                    };
                    Kkm::Device::Call::Result result {};
                    kkm.resetState(details, result);
                    result.exportTo(json);
                }
                std::wcout << Text::convert(json.dump(4));
                return EXIT_SUCCESS;
            }
        }

        usage(std::wcerr, argv[0]);


    // TODO: Исправить перехват исключений
    } catch (const Basic::Failure & e) {
        if (jsonOut) {
            Nln::Json json {{"!success", false }, { "!message", Text::convert(e.explain()) }};
            std::wcout << Text::convert(json.dump(4));
        } else {
            ntsLogError(e);
        }
    } catch (const std::exception & e) {
        if (jsonOut) {
            Nln::Json json {{"!success", false }, { "!message", e.what() }};
            std::wcout << Text::convert(json.dump(4));
        } else {
            ntsLogError(e);
        }
    } catch (...) {
        if (jsonOut) {
            Nln::Json json {{"!success", false }, { "!message", Basic::Mbs::c_somethingWrong }};
            std::wcout << Text::convert(json.dump(4));
        } else {
            ntsLogError(Basic::Wcs::c_somethingWrong);
        }
    }

    return EXIT_FAILURE;
}
