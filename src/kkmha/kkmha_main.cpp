// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include <cmake/options.h>
#include <cmake/variables.h>
#include <lib/strings.h>
#include <lib/setcou16.h>
#include <log/write.h>
#include <log/varop.h>
#include <debug/memprof.h>
#include <kkm/varop.h>
#include <config/variables.h>
#include <config/core.h>
#include <config/varop.h>
#include <cstdlib>
#include <iostream>
#include "server_variables.h"
#include "server_core.h"
#include "server_varop.h"
#include "server_static_variables.h"
#include "server_static_varop.h"
#include "service_core.h"
#include "service_varop.h"

#if !BUILD_SEPARATED
#   include <kkmop_core.h>
#endif

void usage(std::wostream & stream, const std::filesystem::path & path) {
    stream
        << L"\n"
        L"Версия: " << BUILD_VERSION << L"\n"
        L"Использование: " << path.filename().c_str() << L" команда [аргумент ...]\n"
        L"Команды:\n"
        L"    help                Вывести справку\n"
        L"    show-config         Вывести конфигурацию\n"
        L"    install             Установить сервер как службу Windows\n"
        L"    uninstall           Удалить службу Windows\n"
        L"    start               Запустить службу Windows\n"
        L"    stop                Остановить службу Windows\n"
        L"    restart             Перезапустить службу Windows\n"
        L"    foreground          Запустить сервер как foreground-процесс\n"
#if BUILD_SEPARATED
        L"\n";
#else
        << Main::Wcs::c_kkmopUsage << L"\n";
#endif
}

int wmain(int argc, wchar_t ** argv, wchar_t ** envp) {
    START_MEMORY_PROFILING;
    FORCE_MEMORY_LEAK;

    try {
        if (argc > 1) {
            const std::wstring command { Text::lowered(argv[1]) };

            if (argc == 2 && command == L"help") {
                usage(std::wcout, argv[0]);
                return EXIT_SUCCESS;
            }

            Config::setBaseVars(envp);
            Config::readJson(Config::s_file, Log::setVars, Kkm::setVars, Server::setVars, Server::Static::setVars);

            if (Server::Static::s_enable) {
                Config::readJson(Server::Static::s_mimeMapFile, Server::Static::setMimeMap);
            }

            if (command == L"service") {
                Log::Console::s_level = Log::c_levelNone;
                std::atexit([] {
                    Log::File::close();
                    Log::EventLog::close();
                });
                Log::File::open();
                Log::EventLog::open();
                Service::Worker::run();
                return EXIT_SUCCESS;
            }

            Log::completeConsoleConfig();

            if (argc == 2) {
                if (command == L"show-config") {
                    std::wcout
                        << L'\n' << Main::vars << Log::vars << Kkm::vars
                        << Server::vars << Server::Static::vars << Service::vars << std::endl;
                    return EXIT_SUCCESS;
                }

                if (command == L"foreground") {
                    Server::run();
                    return EXIT_SUCCESS;
                }

                if (command == L"install") {
                    Service::Control::install(std::format(L"\"{}\" service", Main::s_file.c_str()));
                    ::Sleep(DateTime::c_defaultSleep);
                    return EXIT_SUCCESS;
                }

                if (command == L"uninstall") {
                    Service::Control::uninstall();
                    ::Sleep(DateTime::c_defaultSleep);
                    return EXIT_SUCCESS;
                }

                if (command == L"start") {
                    Service::Control::start();
                    ::Sleep(DateTime::c_defaultSleep);
                    return EXIT_SUCCESS;
                }

                if (command == L"stop") {
                    Service::Control::stop();
                    ::Sleep(DateTime::c_defaultSleep);
                    return EXIT_SUCCESS;
                }

                if (command == L"restart") {
                    Service::Control::stop();
                    ::Sleep(DateTime::c_defaultSleep);
                    Service::Control::start();
                    ::Sleep(DateTime::c_defaultSleep);
                    return EXIT_SUCCESS;
                }
            }

#if !BUILD_SEPARATED
            if (command == L"learn") {
                FORCE_MEMORY_LEAK;
                if (auto result = learnCmd(argc, argv); result) {
                    return *result;
                }
            } else if (argc == 3) {
                if (auto result = deviceCmd(command, argv[2]); result) {
                    return *result;
                }
            }
#endif
        }

        usage(std::wcerr, argv[0]);

    } catch (const Basic::Failure & e) {
        ntsLogError(e);
    } catch (const std::exception & e) {
        ntsLogError(e);
    } catch (...) {
        ntsLogError(Basic::Wcs::c_somethingWrong);
    }

    return EXIT_FAILURE;
}
