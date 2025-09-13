// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "core.h"
#include "variables.h"
#include <lib/winapi.h>
#include <lib/utils.h>
#include <log/defaults.h>

namespace Config {
    void setBaseVars(wchar_t ** envp) {
        {
            wchar_t binaryFile[MAX_PATH];
            if (::GetModuleFileNameW(nullptr, binaryFile, ARRAYSIZE(binaryFile))) {
                Main::s_file.assign(binaryFile);
                Main::s_directory = Main::s_file.parent_path();
                /*if (!::SetCurrentDirectoryW(Main::s_directory.c_str())) {
                    throw Failure(System::explainError(L"SetCurrentDirectoryW(...)")); // NOLINT(*-exception-baseclass)
                }*/
                std::error_code error;
                std::filesystem::current_path(Main::s_directory, error);
                if (error) {
                    throw Failure(error); // NOLINT(*-exception-baseclass)
                }
            } else {
                throw Failure(System::explainError(L"GetModuleFileName(...)")); // NOLINT(*-exception-baseclass)
            }
        }

        {
            std::filesystem::path confDirectory { c_directory };
            std::filesystem::path logsDirectory { Log::c_directory };
            for (; *envp; ++envp) {
                std::wstring variable, value;
                Text::splitVariable(*envp, variable, value, true);
                if (variable == c_directoryEnv) {
                    confDirectory.assign(std::move(value));
                } else if (variable == Log::c_directoryEnv) {
                    logsDirectory.assign(std::move(value));
                }
            }
            std::error_code error;
            s_directory.assign(std::filesystem::absolute(confDirectory, error));
            if (error) {
                throw Failure(error); // NOLINT(*-exception-baseclass)
            }
            s_file.assign(s_directory);
            s_file /= c_file;
            Log::File::s_directory.assign(std::filesystem::absolute(logsDirectory, error));
            if (error) {
                throw Failure(error); // NOLINT(*-exception-baseclass)
            }
        }
    }
}
