// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "server_variables.h"
#include <lib/numeric.h>
#include <lib/text.h>
#include <lib/path.h>
#include <string>

namespace Server {
    void setVars(const Nln::Json & json) {
        Json::handleKey(
            json, "server",
            [] (const Nln::Json & json, const std::wstring & path) -> bool {
                Json::handleKey(json, "ipv4Only", Server::s_ipv4Only, path);
                Json::handleKey(
                    json, "port", Server::s_port,
                    Numeric::between(Server::c_minPort, Server::c_maxPort), path
                );
                Json::handleKey(
                    json, "requestTimeout", Server::s_requestTimeout,
                    Numeric::between(Server::c_minRequestTimeout, Server::c_maxRequestTimeout), path
                );
                Json::handleKey(
                    json, "concurrencyLimit", Server::s_concurrencyLimit,
                    Numeric::between(Server::c_minConcurrencyLimit, Server::c_maxConcurrencyLimit), path
                );
                Json::handleKey(json, "enableLegacyTls", Server::s_enableLegacyTls, path);
                Json::handleKey(json, "securityLevel", Server::s_securityLevel, Numeric::between(0, 5), path);
                Json::handleKey(
                    json, "certificateChainFile", Server::s_certificateChainFile,
                    Path::existsFile(Path::absolute(Path::noEmpty())), path
                );
                Json::handleKey(
                    json, "privateKeyFile", Server::s_privateKeyFile,
                    Path::existsFile(Path::absolute(Path::noEmpty())), path
                );
                Json::handleKey(json, "privateKeyPassword", Server::s_privateKeyPassword, path);
                Json::handleKey(json, "secret", Server::s_secret, path);
                Json::handleKey(json, "loopbackWithoutSecret", Server::s_loopbackWithoutSecret, path);
                return true;
            }
        );
    }

    std::wostream & vars(std::wostream & stream) {
        std::wstring securityLevel;

        if (s_enableLegacyTls) {
            securityLevel.assign(L"0");
        } else if (s_securityLevel >= 0) {
            securityLevel.assign(std::to_wstring(s_securityLevel));
        } else {
            securityLevel.assign(L"<openssl-default>");
        }

        stream
            << L"CFG: server.ipv4Only = " << Text::Wcs::yesNo(s_ipv4Only) << L"\n"
            L"CFG: server.port = " << s_port << L"\n"
            L"CFG: server.requestTimeout = " << s_requestTimeout << L"\n"
            L"CFG: server.concurrencyLimit = " << s_concurrencyLimit << L"\n"
            L"CFG: server.enableLegacyTls = " << Text::Wcs::yesNo(s_enableLegacyTls) << L"\n"
            L"CFG: server.securityLevel = " << securityLevel << L"\n"
            L"CFG: server.certificateChainFile = \"" << s_certificateChainFile.native() << L"\"\n"
            L"CFG: server.privateKeyFile = \"" << s_privateKeyFile.native() << L"\"\n"
            L"CFG: server.privateKeyPassword = \"" << Text::convert(s_privateKeyPassword) << L"\"\n"
            L"CFG: server.secret = \"" << Text::convert(s_secret) << L"\"\n"
            L"CFG: server.loopbackWithoutSecret = " << Text::Wcs::yesNo(s_loopbackWithoutSecret) << L"\n";

        return stream;
    }
}
