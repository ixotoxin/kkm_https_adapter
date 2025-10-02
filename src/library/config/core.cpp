// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "core.h"
#include "variables.h"
#include <lib/winapi.h>
#include <lib/errexp.h>
#include <lib/path.h>
#include <log/defaults.h>

namespace Config {
    void setBaseVars(wchar_t ** envp) {
        {
            wchar_t binaryFile[MAX_PATH];
            if (::GetModuleFileNameW(nullptr, binaryFile, ARRAYSIZE(binaryFile))) {
                Main::s_file.assign(binaryFile);
                Main::s_directory = Main::s_file.parent_path();
                std::filesystem::current_path(Main::s_directory);
            } else {
                throw Failure(System::explainError(L"GetModuleFileName(...)")); // NOLINT(*-exception-baseclass)
            }
        }

        {
            std::filesystem::path confDirectory { Config::c_directory };
            std::filesystem::path logsDirectory { Log::c_directory };
            for (; *envp; ++envp) {
                std::wstring variable, value;
                Text::splitVariable(*envp, variable, value, true);
                if (variable == Config::c_directoryEnv) {
                    confDirectory.assign(std::move(value));
                } else if (variable == Log::c_directoryEnv) {
                    logsDirectory.assign(std::move(value));
                }
            }
            Config::s_directory.assign(std::filesystem::absolute(confDirectory));
            Config::s_file.assign(s_directory);
            Config::s_file /= c_file;
            Log::File::s_directory.assign(std::filesystem::absolute(logsDirectory));
        }
    }
}
