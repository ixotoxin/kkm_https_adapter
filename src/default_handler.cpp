// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "default_handler.h"
#include "solid_response.h"
#include "binary_response.h"
#include "favicon.h"
#include "static_handler.h"

namespace Http {
    namespace Mbs {
        constexpr std::string_view c_redirectToStatic {
            "HTTP/1.1 302 Moved Temporarily\r\n"
            "Connection: close\r\n"
            "Location: /static/index.html\r\n"
            "Pragma: no-cache\r\n"
            "Cache-Control: no-cache, private\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: 128\r\n"
            "\r\n"
            "<script lang=\"javascript\">document.location.href=\"/static/index.html\";</script>"
            "Click this <a href=\"/static/index.html\">link</a>."
        };
    }

    using Basic::Failure;

    bool DefaultHandler::asyncReady() const noexcept {
        return false;
    }

    void DefaultHandler::operator()(Request & request) const noexcept try {
        assert(request.m_response.m_status == Status::Ok);
        if (request.m_response.m_status != Status::Ok) {
            return;
        }

        if (request.m_method == Method::Get) {
            if (s_enableStatic && request.m_hint.size() < 2) {
                request.m_response.m_status = Status::MovedTemporarily;
                request.m_response.m_data = std::make_shared<SolidResponse>(Mbs::c_redirectToStatic);
                tsLogDebug(Wcs::c_redirectToStatic, request.m_id);
            } else if (request.m_hint.size() == 2 && request.m_hint[1] == "favicon.ico") {
                request.m_response.m_data
                    = std::make_shared<BinaryResponse<Regular>>(
                        reinterpret_cast<char *>(&c_favIconData[0]),
                        c_favIconLength,
                        c_favIconMime
                    );
            } else {
                request.fail(Status::NotFound, Mbs::c_notFound);
            }
        } else if (request.m_method == Method::Post) {
            request.fail(Status::NotFound, Mbs::c_notFound);
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
