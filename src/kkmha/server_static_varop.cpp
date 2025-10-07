// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "server_static_varop.h"
#include "server_static_variables.h"
#include "server_static_strings.h"
#include <lib/path.h>

namespace Server::Static {
    using namespace std::string_literals;

    using Basic::Failure;

    void setVars(const Nln::Json & json) {
        Json::handleKey(
            json, "server",
            [] (const Nln::Json & json, const std::wstring & path) -> bool {
                Json::handleKey(json, "enableStatic", s_enable, path);
                Json::handleKey(
                    json, "staticDirectory", s_directory,
                    Path::testDir(s_enable, Path::absolute(Path::noEmpty())), path
                );
                Json::handleKey(json, "indexFile", s_indexFile, Path::Mbs::goodFileName(), path);
                Json::handleKey(
                    json, "mimeMap", s_mimeMapFile,
                    Path::testFile(s_enable, Path::absolute(Path::noEmpty())), path
                );
                Json::handleKey(json, "enableUnknownType", s_enableUnknownType, path);
                return true;
            }
        );
    }

    void setMimeMap(const Nln::Json & json) {
        if (!json.is_object()) {
            throw Failure(Wcs::c_incorrectStructure); // NOLINT(*-exception-baseclass)
        }
        for (auto & [key, value] : json.items()) {
            if (!value.is_string()) {
                throw Failure(Wcs::c_incorrectStructure); // NOLINT(*-exception-baseclass)
            }
            auto ext = Text::lowered(key);
            std::replace_if(ext.begin(), ext.end(), [] (char c) { return c == 0xa || c == 0xd; }, ' ');
            Text::trim(ext);
            auto type = Text::lowered(value.get<std::string>());
            std::replace_if(type.begin(), type.end(), [] (char c) { return c == 0xa || c == 0xd; }, ' ');
            Text::trim(type);
            if (ext.empty() || type.empty()) {
                throw Failure(Wcs::c_incorrectStructure); // NOLINT(*-exception-baseclass)
            }
            if (ext[0] != '.') {
                ext = "."s + ext; // NOLINT(*-inefficient-string-concatenation)
            }
            s_mimeMap[ext] = std::move(type);
        }
    }

    std::wostream & vars(std::wostream & stream) {
        stream
            << L"CFG: server.enableStatic = " << Text::Wcs::yesNo(s_enable) << L"\n"
            L"CFG: server.staticDirectory = \"" << s_directory.native() << L"\"\n"
            L"CFG: server.indexFile = \"" << Text::convert(s_indexFile) << L"\"\n"
            L"CFG: server.mimeMapFile = \"" << s_mimeMapFile.native() << L"\"\n"
            L"CFG: server.enableUnknownType = " << Text::Wcs::yesNo(s_enableUnknownType) << L"\n";

        return stream;
    }
}
