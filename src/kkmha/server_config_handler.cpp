// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "server_config_handler.h"
#include "http_json_response.h"
#include <lib/wconv.h>
#include <lib/text.h>
#include <kkm/variables.h>
#include <kkm/device.h>
#include <cassert>

namespace Server::Config {
    using Basic::Failure;

    bool Handler::asyncReady() const noexcept {
        return false;
    }

    void Handler::operator()(Http::Request & request) const noexcept try {
        assert(request.m_response.m_status == Http::Status::Ok);
        // if (request.m_response.m_status != Status::Ok) {
        //     return;
        // }

        if (request.m_method == Http::Method::Get && request.m_hint.size() == 3 && request.m_hint[2] == "general") {
            auto response = std::make_shared<Http::JsonResponse>();
            response->m_data["cliOperator"] = {
                { "name", Text::convert(Kkm::s_cliOperatorName) },
                { "inn", Text::convert(Kkm::s_cliOperatorInn) }
            };
            std::vector<std::string> serials {};
            for (auto const & entry: std::filesystem::directory_iterator { Kkm::s_dbDirectory }) {
                if (entry.is_regular_file()) {
                    if (Text::lowered(entry.path().extension().native()) != L".json") {
                        continue;
                    }
                    serials.emplace_back(Text::convert(entry.path().stem().native()));
                }
            }
            response->m_data["knownDevices"] = serials;
            request.m_response.m_data = std::move(response);
        } else {
            request.fail(Http::Status::MethodNotAllowed, Http::Mbs::c_methodNotAllowed);
        }

    } catch (const Failure & e) {
        request.fail(Http::Status::InternalServerError, Text::convert(e.what()), e.where());
    } catch (const std::exception & e) {
        request.fail(Http::Status::InternalServerError, e.what());
    } catch (...) {
        request.fail(Http::Status::InternalServerError, Basic::Mbs::c_somethingWrong);
    }
}
