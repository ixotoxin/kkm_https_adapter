// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "ping_handler.h"
#include "messages.h"

namespace Http {
    using Basic::Failure;

    bool PingHandler::asyncReady() const noexcept {
        return false;
    }

    void PingHandler::operator()(Request & request) const noexcept try {
        assert(request.m_response.m_status == Status::Ok);
        // if (request.m_response.m_status != Status::Ok) {
        //     return;
        // }

        if (request.m_method == Method::Get) {
            request.m_response.m_data = "Pong";
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
