// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "config.h"
#include "library/utils.h"
#include "library/json.h"
#include <fstream>
#include "log.h"
#include "kkm.h"
#include "static_handler.h"
#include "server.h"

namespace Config {
    using namespace std::string_literals;
    using Basic::Failure;

    namespace Wcs {
        using Basic::Wcs::c_somethingWrong;
    }

    namespace Mbs {
        using Basic::Mbs::c_somethingWrong;
    }

    void read(wchar_t ** envp) {
        {
            wchar_t binaryFile[MAX_PATH];
            if (::GetModuleFileNameW(nullptr, binaryFile, ARRAYSIZE(binaryFile))) {
                s_binaryFile.assign(binaryFile);
                s_workDirectory = s_binaryFile.parent_path();
                /*if (!::SetCurrentDirectoryW(s_workDirectory.c_str())) {
                    throw Failure(System::explainError(L"SetCurrentDirectoryW(...)")); // NOLINT(*-exception-baseclass)
                }*/
                std::error_code error;
                std::filesystem::current_path(s_workDirectory, error);
                if (error) {
                    throw Failure(error); // NOLINT(*-exception-baseclass)
                }
            } else {
                throw Failure(System::explainError(L"GetModuleFileName(...)")); // NOLINT(*-exception-baseclass)
            }
        }
        {
            std::filesystem::path confDirectory { c_configDirectory };
            std::filesystem::path logsDirectory { Log::c_directory };
            for (; *envp; ++envp) {
                std::wstring variable, value;
                Text::splitVariable(*envp, variable, value, true);
                if (variable == c_configDirectoryEnv) {
                    confDirectory.assign(std::move(value));
                }/* else if (variable == c_logsDirectoryEnv) {
                    logsDirectory.assign(std::move(value));
                }*/
            }
            std::error_code error;
            s_configDirectory.assign(std::filesystem::absolute(confDirectory, error));
            if (error) {
                throw Failure(error); // NOLINT(*-exception-baseclass)
            }
            s_configFile.assign(s_configDirectory);
            s_configFile /= c_configFile;
            Log::File::s_directory.assign(std::filesystem::absolute(logsDirectory, error));
            if (error) {
                throw Failure(error); // NOLINT(*-exception-baseclass)
            }
        }
        {
            if (!std::filesystem::is_regular_file(s_configFile)) {
                throw Failure(std::format(Wcs::c_cantReadConfig, s_configFile.c_str())); // NOLINT(*-exception-baseclass)
            }
            try {
                Nln::Json json(Nln::Json::parse(std::ifstream(s_configFile)));
                Json::handleKey(
                    json, "log",
                    [] (const Nln::Json & json, const std::wstring & path) -> bool {
                        Json::handleKey(
                            json, "console",
                            [] (const Nln::Json & json, const std::wstring & path) -> bool {
                                Json::handleKey(json, "level", Log::Console::s_level, Log::s_levelCastMap, path);
                                Json::handleKey(json, "outputTimestamp", Log::Console::s_outputTimestamp, path);
                                Json::handleKey(json, "outputLevel", Log::Console::s_outputLevel, path);
                                return true;
                            },
                            path
                        );
                        Json::handleKey(
                            json, "file",
                            [] (const Nln::Json & json, const std::wstring & path) -> bool {
                                Json::handleKey(json, "level", Log::File::s_level, Log::s_levelCastMap, path);
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
                                    Log::s_levelCastMap, Numeric::min(Log::c_levelInfo),
                                    path
                                );
                                return true;
                            },
                            path
                        );
                        Json::handleKey(json, "appendLocation", Log::s_appendLocation, path);
                        return true;
                    }
                );
                Json::handleKey(
                    json ,"server",
                    [] (const Nln::Json & json, const std::wstring & path) -> bool {
                        Json::handleKey(json, "ipv4Only", Server::s_ipv4Only, path);
                        Json::handleKey(
                            json, "port",
                            Server::s_port,
                            Numeric::between(Server::c_minPort, Server::c_maxPort),
                            path
                        );
                        Json::handleKey(
                            json, "concurrencyLimit",
                            Server::s_concurrencyLimit,
                            Numeric::between(Server::c_minConcurrencyLimit, Server::c_maxConcurrencyLimit),
                            path
                        );
                        Json::handleKey(json, "enableLegacyTls", Server::s_enableLegacyTls, path);
                        Json::handleKey(json, "securityLevel", Server::s_securityLevel, Numeric::between(0, 5), path);
                        Json::handleKey(
                            json, "certificateChainFile",
                            Server::s_certificateChainFile,
                            Text::Mbs::noEmpty(Text::Mbs::trim()),
                            path
                        );
                        Json::handleKey(
                            json, "privateKeyFile",
                            Server::s_privateKeyFile,
                            Text::Mbs::noEmpty(Text::Mbs::trim()),
                            path
                        );
                        Json::handleKey(json, "privateKeyPassword", Server::s_privateKeyPassword, path);
                        Json::handleKey(json, "secret", Http::s_secret, path);
                        Json::handleKey(json, "loopbackWithoutSecret", Http::s_loopbackWithoutSecret, path);
                        Json::handleKey(json, "enableStatic", Http::s_enableStatic, path);
                        if (Http::s_enableStatic) {
                            Json::handleKey(json, "staticDirectory", Http::s_staticDirectory, path);
                            Json::handleKey(json, "mimeMap", Http::s_mimeMapFile, Text::Wcs::noEmpty(), path);
                            Json::handleKey(json, "enableUnknownType", Http::s_enableUnknownType, path);
                        }
                        return true;
                    }
                );
                Json::handleKey(
                    json, "kkm",
                    [] (const Nln::Json & json, const std::wstring & path) -> bool {
                        Json::handleKey(json, "directory", Kkm::s_directory, Text::Wcs::trim());
                        Json::handleKey(json, "defaultBaudRate", Kkm::s_defaultBaudRate, Kkm::s_allowedBaudRate, path);
                        Json::handleKey(
                            json, "timeZone",
                            Kkm::s_timeZone, Kkm::s_timeZoneMap,
                            [] (auto value) { Kkm::s_timeZoneConfigured = true; return value; },
                            path
                        );
                        Json::handleKey(
                            json, "defaultLineLength",
                            Kkm::s_defaultLineLength,
                            Numeric::between(Kkm::c_minLineLength, Kkm::c_maxLineLength),
                            path
                        );
                        Json::handleKey(
                            json, "documentClosingTimeout",
                            Kkm::s_documentClosingTimeout,
                            Numeric::between(Kkm::c_minDocumentClosingTimeout, Kkm::c_maxDocumentClosingTimeout),
                            path
                        );
                        Json::handleKey(
                            json, "cliOperator",
                            [] (const Nln::Json & json, const std::wstring & path) -> bool {
                                Json::handleKey(json, "name", Kkm::s_cliOperatorName, Text::Wcs::trim(), path);
                                Json::handleKey(json, "inn", Kkm::s_cliOperatorInn, Text::Wcs::trim(), path);
                                return true;
                            },
                            path
                        );
                        Json::handleKey(json, "customerAccountField", Kkm::s_customerAccountField, path);
                        Json::handleKey(
                            json, "maxQuantity",
                            Kkm::s_maxQuantity,
                            Numeric::between(Kkm::c_minMaxQuantity, Kkm::c_maxMaxQuantity),
                            path
                        );
                        Json::handleKey(
                            json, "maxPrice",
                            Kkm::s_maxPrice,
                            Numeric::between(Kkm::c_minMaxPrice, Kkm::c_maxMaxPrice),
                            path
                        );
                        return true;
                    }
                );
                if (Http::s_enableStatic) {
                    if (!std::filesystem::is_regular_file(Http::s_mimeMapFile)) {
                        throw Failure(std::format(Wcs::c_cantReadConfig, Http::s_mimeMapFile)); // NOLINT(*-exception-baseclass)
                    }
                    json.clear();
                    std::ifstream input { Http::s_mimeMapFile };
                    input >> json;
                    if (json.empty() || !json.is_object()) {
                        throw Failure(std::format(Wcs::c_cantParseConfig, Http::s_mimeMapFile)); // NOLINT(*-exception-baseclass)
                    }
                    for (auto & [key, value] : json.items()) {
                        if (!value.is_string()) {
                            throw Failure(std::format(Wcs::c_cantParseConfig, Http::s_mimeMapFile)); // NOLINT(*-exception-baseclass)
                        }
                        auto ext = Text::lowered(key);
                        std::replace_if(ext.begin(), ext.end(), [] (char c) { return c == 0xa || c == 0xd; }, ' ');
                        Text::trim(ext);
                        auto type = Text::lowered(value.get<std::string>());
                        std::replace_if(type.begin(), type.end(), [] (char c) { return c == 0xa || c == 0xd; }, ' ');
                        Text::trim(type);
                        if (ext.empty() || type.empty()) {
                            throw Failure(std::format(Wcs::c_cantParseConfig, Http::s_mimeMapFile)); // NOLINT(*-exception-baseclass)
                        }
                        if (ext[0] != '.') {
                            ext = "."s + ext; // NOLINT(*-inefficient-string-concatenation)
                        }
                        Http::s_mimeMap[ext] = std::move(type);
                    }
                }
                return; /** Не удаляй, смотри дальше. **/
            } catch (const Failure & e) {
                ntsLogWarning(e);
            } catch (const std::exception & e) {
                ntsLogWarning(e);
            } catch (...) {
                ntsLogWarning(Wcs::c_somethingWrong);
            }
            throw Failure(Wcs::c_invalidConfig); // NOLINT(*-exception-baseclass)
        }
    }
}
