// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "defaults.h"
#include "variables.h"
#include "strings.h"
#include <lib/except.h>
#include <lib/json.h>
#include <lib/path.h>
#include <main/variables.h>
#include <log/write.h>
#include <concepts>
#include <fstream>
#include <filesystem>
#include <string>
#include <format>

namespace Config {
    using Basic::Failure;

    void setBaseVars(wchar_t ** envp);

    template<typename T, std::same_as<void (*)(const Nln::Json &)> ... SETTERS>
    requires std::is_same_v<T, std::filesystem::path> || std::is_same_v<T, std::wstring>
    void readJson(const T & file, SETTERS ... setters) {
        if (!std::filesystem::is_regular_file(file)) {
            throw Failure(CONFIG_WFMT(Wcs::c_cantReadConfig, file.c_str())); // NOLINT(*-exception-baseclass)
        }
        try {
            std::filesystem::current_path(Config::s_directory);
            Nln::Json json(Nln::Json::parse(std::ifstream(file)));
            (setters(json), ...);
            std::filesystem::current_path(Main::s_directory);
            return; /** Не удаляй, смотри дальше. **/
        } catch (const Failure & e) {
            LOG_WARNING_NTS(e);
        } catch (const std::exception & e) {
            LOG_WARNING_NTS(e);
        } catch (...) {
            LOG_WARNING_NTS(Basic::Wcs::c_somethingWrong);
        }
        throw Failure(Wcs::c_invalidConfig); // NOLINT(*-exception-baseclass)
    }
}
