// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "varop.h"
#include "defaults.h"
#include "variables.h"
#include "strings.h"
#include "core.h"
#include <lib/numeric.h>
#include <lib/text.h>

namespace Log {
    void setVars(const Nln::Json & json) {
        Json::handleKey(
            json, "log",
            [] (const Nln::Json & json, const std::wstring & path) -> bool {
                Json::handleKey(
                    json, "console",
                    [] (const Nln::Json & json, const std::wstring & path) -> bool {
                        Json::handleKey(json, "level", Console::s_level, Wcs::c_levelCastMap, path);
                        Json::handleKey(json, "outputTimestamp", Console::s_outputTimestamp, path);
                        Json::handleKey(json, "outputLevel", Console::s_outputLevel, path);
                        return true;
                    },
                    path
                );
                Json::handleKey(
                    json, "file",
                    [] (const Nln::Json & json, const std::wstring & path) -> bool {
                        Json::handleKey(json, "level", File::s_level, Wcs::c_levelCastMap, path);
                        Json::handleKey(json, "directory", File::s_directory, Text::Wcs::noEmpty(), path);
                        return true;
                    },
                    path
                );
                Json::handleKey(
                    json, "eventLog",
                    [] (const Nln::Json & json, const std::wstring & path) -> bool {
                        Json::handleKey(
                            json, "level",
                            EventLog::s_level,
                            Wcs::c_levelCastMap, Numeric::min(c_levelInfo),
                            path
                        );
                        return true;
                    },
                    path
                );
                Json::handleKey(json, "consToConsOnly", s_consToConsOnly, path);
                Json::handleKey(json, "appendLocation", s_appendLocation, path);
                return true;
            }
        );
    }

    std::wostream & vars(std::wostream & stream) {
        stream
            << L"[CFG] log.console.level = " << levelLabel(Console::s_level) << L"\n"
            L"[CFG] log.console.outputTimestamp = " << Text::Wcs::yesNo(Console::s_outputTimestamp) << L"\n"
            L"[CFG] log.console.outputLevel = " << Text::Wcs::yesNo(Console::s_outputLevel) << L"\n"
            L"[CFG] log.file.level = " << levelLabel(File::s_level) << L"\n"
            L"[CFG] log.file.directory = \"" << File::s_directory << L"\"\n"
            L"[CFG] log.eventLog.level = " << levelLabel(EventLog::s_level) << L"\n"
            L"[DEF] log.eventLog.source = \"" << c_eventSource << L"\"\n"
            L"[CFG] log.consToConsOnly = " << Text::Wcs::yesNo(s_consToConsOnly) << L"\n"
            L"[CFG] log.appendLocation = " << Text::Wcs::yesNo(s_appendLocation) << L"\n";

        return stream;
    }
}
