// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "server_static_varop.h"
#include "server_static_variables.h"
#include "server_static_strings.h"

namespace Server::Static {
    using namespace std::string_literals;
    using namespace std::string_view_literals;

    using Basic::Failure;
    using Basic::DataError;

    std::string filterFileName(const std::string & fileName) {
        std::string result { /*Text::trimmed(*/fileName/*)*/ };
        if (
            result.find_first_of(
                "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f"
                "\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f"
                "<>:\"/\\|?*"sv
            ) != std::string_view::npos
            || result.empty()
            || result.starts_with("."sv)
            || result.ends_with("."sv)
            || result.starts_with(" "sv)
            || result.ends_with(" "sv)
        ) {
            throw DataError(Basic::Wcs::c_invalidValue); // NOLINT(*-exception-baseclass)
        }
        return result;
    }

    void setVars(const Nln::Json & json) {
        Json::handleKey(
            json, "server",
            [] (const Nln::Json & json, const std::wstring & path) -> bool {
                Json::handleKey(json, "enableStatic", s_enable, path);
                Json::handleKey(json, "staticDirectory", s_directory, path);
                Json::handleKey(json, "indexFile", s_indexFile, filterFileName, path);
                Json::handleKey(json, "mimeMap", s_mimeMapFile, Text::Wcs::noEmpty(), path);
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
            << L"[CFG] server.enableStatic = " << Text::Wcs::yesNo(s_enable) << L"\n"
            L"[CFG] server.staticDirectory = \"" << s_directory << L"\"\n"
            L"[CFG] server.indexFile = \"" << Text::convert(s_indexFile) << L"\"\n"
            L"[CFG] server.mimeMapFile = \"" << s_mimeMapFile << L"\"\n"
            L"[CFG] server.enableUnknownType = " << Text::Wcs::yesNo(s_enableUnknownType) << L"\n";

        return stream;
    }
}
