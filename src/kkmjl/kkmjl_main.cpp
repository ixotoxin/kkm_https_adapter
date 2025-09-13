// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include <cmake/options.h>
#include <cmake/variables.h>
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

void usage(std::wostream & stream, const std::filesystem::path & path) {
    stream <<
        L"\n!! Не реализовано !!\n\n"
        L"Версия: " << BUILD_VERSION << L"\n"
        L"Использование: " << path.filename().c_str() << L" команда [аргумент ...]\n"
        L"Команды:\n"
        L"    help                Вывести справку\n"
        L"    show-config         Вывести конфигурацию\n"
        L"    ...\n"
        L"\n";
}

int wmain(int argc, wchar_t ** argv, wchar_t ** envp) {
    START_MEMORY_PROFILING;
    FORCE_MEMORY_LEAK;

    try {
        if (argc > 1) {
            std::wstring command { Text::lowered(argv[1]) };

            if (argc == 2 && command == L"help") {
                usage(std::wcout, argv[0]);
                return EXIT_SUCCESS;
            }

            Config::setBaseVars(envp);
            Config::readJson(Config::s_file, Log::setVars, Kkm::setVars);
            Log::completeConsoleConfig();

            if (argc == 2 && command == L"show-config") {
                std::wcout << L'\n' << Main::vars << Log::vars << Kkm::vars << std::endl;
                return EXIT_SUCCESS;
            }

            ntsLogError(L"Утилита исполнения команды описанной json-файлом будет реализована позже");

            return EXIT_FAILURE;
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
