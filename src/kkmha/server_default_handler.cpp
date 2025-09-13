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
        // if (request.m_response.m_status != Status::Ok) {
        //     return;
        // }

        if (request.m_method == Http::Method::Get) {
            if (Static::s_enable && request.m_hint.size() < 2) {
                request.m_response.m_status = Http::Status::MovedTemporarily;
                request.m_response.m_data = std::make_shared<Http::SolidResponse>(Mbs::c_redirectToStaticResponse);
                tsLogDebug(Http::Wcs::c_redirectToStatic, request.m_id);
            } else if (request.m_hint.size() == 2 && request.m_hint[1] == "favicon.ico") {
                request.m_response.m_data
                    = std::make_shared<Http::BinaryResponse<Http::Regular>>(
                        reinterpret_cast<char *>(&c_favIconData[0]),
                        c_favIconLength,
                        c_favIconMime
                    );
            } else {
                request.fail(Http::Status::NotFound, Http::Mbs::c_notFound);
            }
        } else if (request.m_method == Http::Method::Post) {
            request.fail(Http::Status::NotFound, Http::Mbs::c_notFound);
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
