// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "varop.h"
#include "defaults.h"
#include "variables.h"
#include "strings.h"
#include "core.h"
#include <lib/numeric.h>
#include <lib/text.h>
#include <lib/path.h>
#include <main/variables.h>

namespace Log {
    void setVars(const Nln::Json & json) {
        auto found = Json::handleKey(
            json, "log",
            [] (const Nln::Json & json, const std::wstring & path) -> bool {
                Json::handleKey(
                    json, "console",
                    [] (const Nln::Json & json, const std::wstring & path) -> bool {
                        Json::handleKey(
                            json, "level",
                            [] (const Nln::Json & json, const std::wstring & path) -> bool {
                                if (json.is_object()) {
                                    Json::handleKey(json, "foreground", Console::s_level, Wcs::c_levelCastMap, path);
                                } else {
                                    Json::handle(json, Console::s_level, Wcs::c_levelCastMap, path);
                                }
                                return true;
                            },
                            path
                        );
                        Json::handleKey(json, "outputTimestamp", Console::s_outputTimestamp, path);
                        Json::handleKey(json, "outputLevel", Console::s_outputLevel, path);
                        return true;
                    },
                    path
                );
                Json::handleKey(
                    json, "file",
                    [] (const Nln::Json & json, const std::wstring & path) -> bool {
                        Json::handleKey(
                            json, "level",
                            [] (const Nln::Json & json, const std::wstring & path) -> bool {
                                if (json.is_object()) {
                                    Json::handleKey(json, "foreground", File::s_fgLevel, Wcs::c_levelCastMap, path);
                                    Json::handleKey(json, "background", File::s_bgLevel, Wcs::c_levelCastMap, path);
                                } else {
                                    LevelUnderlying level { Log::c_levelNone };
                                    auto found = Json::handle(json, level, Wcs::c_levelCastMap, path);
                                    if (found) {
                                        File::s_fgLevel = level;
                                        File::s_bgLevel = level;
                                    }
                                }
                                return true;
                            },
                            path
                        );
                        Json::handleKey(
                            json, "directory", File::s_directory,
                            Path::touchDir(Path::absolute(Main::s_directory, Path::noEmpty())), path
                        );
                        return true;
                    },
                    path
                );
                Json::handleKey(
                    json, "eventLog",
                    [] (const Nln::Json & json, const std::wstring & path) -> bool {
                        Json::handleKey(
                            json, "level",
                            [] (const Nln::Json & json, const std::wstring & path) -> bool {
                                if (json.is_object()) {
                                    Json::handleKey(
                                        json, "foreground", EventLog::s_fgLevel,
                                        Wcs::c_levelCastMap, Numeric::min(c_levelInfo), path
                                    );
                                    Json::handleKey(
                                        json, "background", EventLog::s_bgLevel,
                                        Wcs::c_levelCastMap, Numeric::min(c_levelInfo), path
                                    );
                                } else {
                                    LevelUnderlying level { Log::c_levelNone };
                                    auto found
                                        = Json::handle(
                                            json, level, Wcs::c_levelCastMap,
                                            Numeric::min(c_levelInfo), path
                                        );
                                    if (found) {
                                        EventLog::s_fgLevel = level;
                                        EventLog::s_bgLevel = level;
                                    }
                                }
                                return true;
                            },
                            path
                        );
                        return true;
                    },
                    path
                );
                Json::handleKey(json, "appendLocation", s_appendLocation, path);
                return true;
            }
        );
        if (found) {
            Log::reconfig();
        }
    }

    std::wostream & vars(std::wostream & stream) {
        stream
            << L"CFG: log.console.level.foreground = " << levelLabel(Console::s_level) << L"\n"
            L"CFG: log.console.outputTimestamp = " << Text::Wcs::yesNo(Console::s_outputTimestamp) << L"\n"
            L"CFG: log.console.outputLevel = " << Text::Wcs::yesNo(Console::s_outputLevel) << L"\n"
            L"CFG: log.file.level.foreground = " << levelLabel(File::s_fgLevel) << L"\n"
            L"CFG: log.file.level.background = " << levelLabel(File::s_bgLevel) << L"\n"
            L"CFG: log.file.directory = \"" << File::s_directory.native() << L"\"\n"
            L"CFG: log.eventLog.level.foreground = " << levelLabel(EventLog::s_fgLevel) << L"\n"
            L"CFG: log.eventLog.level.background = " << levelLabel(EventLog::s_bgLevel) << L"\n"
            L"DEF: log.eventLog.source = \"" << c_eventSource << L"\"\n"
            L"CFG: log.appendLocation = " << Text::Wcs::yesNo(s_appendLocation) << L"\n";

        return stream;
    }
}
