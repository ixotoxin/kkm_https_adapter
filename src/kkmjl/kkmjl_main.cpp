// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include <cmake/variables.h>
#include <lib/setcou16.h>
#include <log/varop.h>
#include <debug/memprof.h>
#include <kkm/varop.h>
#include <config/variables.h>
#include <config/core.h>
#include <cstdlib>
#include <iostream>
#include "kkmjl_core.h"

int wmain(int argc, wchar_t ** argv, wchar_t ** envp) {
    try {
        if (argc == 3) {
            Config::setBaseVars(envp);
            Config::readJson(Config::s_file, Log::setVars, Kkm::setVars);
            return KkmJsonLoader::exec(argv[1], argv[2]);
        }
        std::wcerr
            << L"{\n"
#if WITH_ASAN || WITH_CRTDBG
            L"    \"" MEMORY_PROFILING_FLAG_KEY "\": \"" MEMORY_PROFILING_FLAG_VALUE "\",\n"
#endif
            L"    \"" << Json::Wcs::c_successKey << L"\": false,\n"
            L"    \"" << Json::Wcs::c_messageKey << L"\": \"Неверное использование\",\n"
            L"    \"!version\": \"" << BUILD_VERSION << L"\",\n"
            L"    \"!usage\": \"" << argv[0] << L" {сн} {вф}\"\n"
            L"}\n";
    } catch (const Basic::Failure & e) {
        KkmJsonLoader::printError(e.explain(Log::s_appendLocation));
    } catch (const std::exception & e) {
        KkmJsonLoader::printError(Text::convert(e.what()));
    } catch (...) {
        KkmJsonLoader::printError(Basic::Wcs::c_somethingWrong);
    }

    return EXIT_FAILURE;
}
