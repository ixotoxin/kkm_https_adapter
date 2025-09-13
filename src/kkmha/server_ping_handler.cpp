// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "server_ping_handler.h"
#include <cassert>

namespace Server::Ping {
    using Basic::Failure;

    bool Handler::asyncReady() const noexcept {
        return false;
    }

    void Handler::operator()(Http::Request & request) const noexcept try {
        assert(request.m_response.m_status == Http::Status::Ok);
        // if (request.m_response.m_status != Http::Status::Ok) {
        //     return;
        // }

        if (request.m_method == Http::Method::Get) {
            request.m_response.m_data = "Pong";
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
