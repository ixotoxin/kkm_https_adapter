// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include <cmake/options.h>
#include <cmake/variables.h>
#include <lib/strings.h>
#include <lib/setcou16.h>
#include <main/variables.h>
#include <main/varop.h>
#include <log/write.h>
#include <log/varop.h>
#include <debug/memprof.h>
#include <kkm/varop.h>
#include <config/variables.h>
#include <config/core.h>
#include <config/varop.h>
#include <cstdlib>
#include <iostream>
#include "server_core.h"
#include "server_varop.h"
#include "server_static_variables.h"
#include "server_static_varop.h"
#include "service_core.h"
#include "service_varop.h"

#if !BUILD_SEPARATED
#   include <kkmop_strings.h>
#   include <kkmop_core.h>
#   include <kkmjl_strings.h>
#   include <kkmjl_core.h>
#endif

void usage(std::wostream & stream, const std::filesystem::path & path) {
    stream
        << L"\n"
        L"Версия: " << BUILD_VERSION << L"\n"
        L"Использование: " << path.filename().native()
#if BUILD_SEPARATED
        << L" команда\n"
#else
        << L" команда [аргумент ...]\n"
#endif
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
        << KkmOperator::Wcs::c_usage1
        << KkmJsonLoader::Wcs::c_usage1
        << L"где\n"
        << KkmOperator::Wcs::c_usage2
        << KkmJsonLoader::Wcs::c_usage2
        << L"\n";
#endif
}

int wmain(int argc, wchar_t ** argv, wchar_t ** envp) {
    START_MEMORY_PROFILING;
    FORCE_MEMORY_LEAK;

    try {
        if (argc > 1) {
            const std::wstring command { Text::lowered(argv[1]) };
            bool isService { command == L"service" };

            if (isService) {
                Log::asBackgroundProcess();
            } else if (argc == 2 && command == L"help") {
                usage(std::wcout, argv[0]);
                return EXIT_SUCCESS;
            }

            Config::setBaseVars(envp);
            Config::readJson(Config::s_file, Log::setVars, Kkm::setVars, Server::setVars, Server::Static::setVars);

            if (Server::Static::s_enable) {
                Config::readJson(Server::Static::s_mimeMapFile, Server::Static::setMimeMap);
            }

            if (isService) {
                Service::Worker::run();
                return EXIT_SUCCESS;
            }

            if (argc == 2) {
                if (command == L"show-config") {
                    std::wcout
                        << L'\n' << Main::vars << Config::vars << Log::vars << Kkm::vars
                        << Server::vars << Server::Static::vars << Service::vars << std::endl;
                    return EXIT_SUCCESS;
                }

                if (command == L"foreground") {
                    Server::run();
                    return EXIT_SUCCESS;
                }

                if (command == L"install") {
                    Service::Control::install(std::format(L"\"{}\" service", Main::s_file.native()));
                    ::Sleep(DateTime::c_basicSleep);
                    return EXIT_SUCCESS;
                }

                if (command == L"uninstall") {
                    Service::Control::uninstall();
                    ::Sleep(DateTime::c_basicSleep);
                    return EXIT_SUCCESS;
                }

                if (command == L"start") {
                    Service::Control::start();
                    ::Sleep(DateTime::c_basicSleep);
                    return EXIT_SUCCESS;
                }

                if (command == L"stop") {
                    Service::Control::stop();
                    ::Sleep(DateTime::c_basicSleep);
                    return EXIT_SUCCESS;
                }

                if (command == L"restart") {
                    Service::Control::stop();
                    ::Sleep(DateTime::c_basicSleep);
                    Service::Control::start();
                    ::Sleep(DateTime::c_basicSleep);
                    return EXIT_SUCCESS;
                }
            }

#if !BUILD_SEPARATED
            if (argc > 2 && command == L"learn") {
                FORCE_MEMORY_LEAK;
                return KkmOperator::learn(argc - 2, &argv[2]);
            } else if (argc == 3) {
                if (auto result = KkmOperator::exec(command, argv[2]); result) {
                    return *result;
                }
            } else if (argc == 4 && command == L"exec") {
                return KkmJsonLoader::safeExec(argv[2], argv[3]);
            }
#endif
        }

        usage(std::wcerr, argv[0]);

    } catch (const Basic::Failure & e) {
        LOG_ERROR_NTS(e);
    } catch (const std::exception & e) {
        LOG_ERROR_NTS(e);
    } catch (...) {
        LOG_ERROR_NTS(Basic::Wcs::c_somethingWrong);
    }

    return EXIT_FAILURE;
}
