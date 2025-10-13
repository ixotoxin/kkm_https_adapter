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

        if (request.m_method == Http::Method::Get) {
            request.m_response.m_data = s_pongResponse;
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
