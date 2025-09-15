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
                        Json::handleKey(json, "level", Log::Console::s_level, Log::Wcs::c_levelCastMap, path);
                        Json::handleKey(json, "outputTimestamp", Log::Console::s_outputTimestamp, path);
                        Json::handleKey(json, "outputLevel", Log::Console::s_outputLevel, path);
                        return true;
                    },
                    path
                );
                Json::handleKey(
                    json, "file",
                    [] (const Nln::Json & json, const std::wstring & path) -> bool {
                        Json::handleKey(json, "level", Log::File::s_level, Log::Wcs::c_levelCastMap, path);
                        Json::handleKey(json, "directory", Log::File::s_directory, Text::Wcs::noEmpty(), path);
                        return true;
                    },
                    path
                );
                Json::handleKey(
                    json, "eventLog",
                    [] (const Nln::Json & json, const std::wstring & path) -> bool {
                        Json::handleKey(
                            json, "level",
                            Log::EventLog::s_level,
                            Log::Wcs::c_levelCastMap, Numeric::min(Log::c_levelInfo),
                            path
                        );
                        return true;
                    },
                    path
                );
                Json::handleKey(json, "consToConsOnly", Log::s_consToConsOnly, path);
                Json::handleKey(json, "appendLocation", Log::s_appendLocation, path);
                return true;
            }
        );
    }

    std::wostream & vars(std::wostream & stream) {
        stream
            << L"[CFG] log.console.level = " << Log::levelLabel(Log::Console::s_level) << L"\n"
            L"[CFG] log.console.outputTimestamp = " << Text::Wcs::yesNo(Log::Console::s_outputTimestamp) << L"\n"
            L"[CFG] log.console.outputLevel = " << Text::Wcs::yesNo(Log::Console::s_outputLevel) << L"\n"
            L"[CFG] log.file.level = " << Log::levelLabel(Log::File::s_level) << L"\n"
            L"[CFG] log.file.directory = \"" << Log::File::s_directory << L"\"\n"
            L"[CFG] log.eventLog.level = " << Log::levelLabel(Log::EventLog::s_level) << L"\n"
            L"[DEF] log.eventLog.source = \"" << Log::c_eventSource << L"\"\n"
            L"[CFG] log.consToConsOnly = " << Text::Wcs::yesNo(Log::s_consToConsOnly) << L"\n"
            L"[CFG] log.appendLocation = " << Text::Wcs::yesNo(Log::s_appendLocation) << L"\n";

        return stream;
    }
}
