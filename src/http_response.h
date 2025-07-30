// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "http_res_data.h"
#include <variant>

namespace Http {
    namespace Mbs {
        constexpr std::string_view c_responseHeaderTemplate {
            "HTTP/1.1 {} {}\r\n"
            "Connection: close\r\n"
            "Pragma: no-cache\r\n"
            "Cache-Control: no-cache, private\r\n"
            "Content-Type: {}\r\n"
            "Content-Length: {}\r\n"
            "\r\n"
        };
    }

    struct Response {
        std::variant<std::nullptr_t, std::string, std::shared_ptr<ResponseData>> m_data { nullptr };
        Status m_status { Status::Ok };

        Response() = default;
        Response(const Response &) = delete;
        Response(Response &&) = delete;
        ~Response() = default;

        Response & operator=(const Response &) = delete;
        Response & operator=(Response &&) = delete;

        void render(Asio::StreamBuffer & buffer) {
            if (m_data.index() == 2) {
                std::get<2>(m_data)->render(buffer, m_status);
            } else {
                assert(s_statusStrings.contains(m_status));
                Nln::Json json({
                    { Mbs::c_successKey, m_status < Status::BadRequest },
                    { Mbs::c_messageKey, m_data.index() == 1 ? std::get<1>(m_data) : s_statusStrings.at(m_status) }
                }, false, Nln::EmptyJsonObject);
                std::string text { json.dump() };
                std::ostream output { &buffer };
                output
                    << std::format(
                        Mbs::c_responseHeaderTemplate,
                        static_cast<int>(m_status),
                        s_statusStrings.at(m_status),
                        Mbs::c_jsonMimeType,
                        text.size()
                    )
                    << text;
            }
        }
    };
}
