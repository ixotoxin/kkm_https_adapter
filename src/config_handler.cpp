// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "config_handler.h"
#include "kkm.h"
#include "json_response.h"

namespace Http {
    using Basic::Failure;

    bool ConfigHandler::asyncReady() const noexcept {
        return false;
    }

    void ConfigHandler::operator()(Request & request) const noexcept try {
        assert(request.m_response.m_status == Status::Ok);
        // if (request.m_response.m_status != Status::Ok) {
        //     return;
        // }

        if (request.m_method == Method::Get && request.m_hint.size() == 3 && request.m_hint[2] == "general") {
            auto response = std::make_shared<JsonResponse>();
            response->m_data["cliOperator"] = {
                { "name", Text::convert(Kkm::s_cliOperatorName) },
                { "inn", Text::convert(Kkm::s_cliOperatorInn) }
            };
            std::vector<std::string> serials {};
            for (auto const & entry: std::filesystem::directory_iterator { Kkm::s_dbDirectory }) {
                if (entry.is_regular_file()) {
                    if (Text::lowered(entry.path().extension().c_str()) != L".json") {
                        continue;
                    }
                    serials.emplace_back(Text::convert(entry.path().stem().c_str()));
                }
            }
            response->m_data["knownDevices"] = serials;
            request.m_response.m_data = response;
        } else {
            request.fail(Status::MethodNotAllowed, Mbs::c_methodNotAllowed);
        }

    } catch (const Failure & e) {
        request.fail(Status::InternalServerError, Text::convert(e.what()), e.where());
    } catch (const std::exception & e) {
        request.fail(Status::InternalServerError, e.what());
    } catch (...) {
        request.fail(Status::InternalServerError, Basic::Mbs::c_somethingWrong);
    }
}
