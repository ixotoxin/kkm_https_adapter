// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "server_default_handler.h"
#include "server_default_strings.h"
#include "server_static_variables.h"
#include "server_favicon.h"
#include "http_solid_response.h"
#include "http_binary_response.h"
#include <cassert>

namespace Server::Default {
    using Basic::Failure;

    bool Handler::asyncReady() const noexcept {
        return false;
    }

    void Handler::operator()(Http::Request & request) const noexcept try {
        assert(request.m_response.m_status == Http::Status::Ok);

        if (request.m_method == Http::Method::Get) {
            if (Static::s_enable && request.m_hint.size() < 2) {
                request.m_response.m_status = Http::Status::MovedTemporarily;
                request.m_response.m_data = std::make_shared<Http::SolidResponse>(Mbs::c_redirectToStaticResponse);
                LOG_DEBUG_TS(Wcs::c_redirectToStatic, request.m_id);
            } else if (request.m_hint.size() == 2 && request.m_hint[1] == "favicon.ico") {
                request.m_response.m_data
                    = std::make_shared<Http::BinaryResponse<Http::Regular>>(
                        reinterpret_cast<char *>(&c_favIconData[0]),
                        c_favIconLength,
                        c_favIconMime
                    );
            } else {
                fail(request, Http::Status::NotFound, Server::Mbs::c_notFound);
            }
        } else if (request.m_method == Http::Method::Post) {
            fail(request, Http::Status::NotFound, Server::Mbs::c_notFound);
        } else {
            fail(request, Http::Status::MethodNotAllowed, Server::Mbs::c_methodNotAllowed);
        }

    } catch (const Failure & e) {
        fail(request, Http::Status::InternalServerError, Text::convert(e.what()), e.where());
    } catch (const std::exception & e) {
        fail(request, Http::Status::InternalServerError, e.what());
    } catch (...) {
        fail(request, Http::Status::InternalServerError, Basic::Mbs::c_somethingWrong);
    }
}
