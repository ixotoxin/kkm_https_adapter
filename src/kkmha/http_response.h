// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "http_types.h"
#include "http_strings.h"
#include "http_proto_response.h"
#include <lib/meta.h>
#include <lib/json.h>
#include <cassert>
#include <memory>
#include <variant>
#include <ostream>
#include <format>

namespace Http {
    struct Response {
        std::variant<std::nullptr_t, std::string, std::shared_ptr<ProtoResponse>> m_data { nullptr };
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
                assert(Mbs::c_statusStrings.contains(m_status));
                Nln::Json json({
                    { Json::Mbs::c_successKey, m_status < Status::BadRequest },
                    { Json::Mbs::c_messageKey, m_data.index() == 1 ? std::get<1>(m_data) : Mbs::c_statusStrings.at(m_status) }
                }, false, Nln::EmptyJsonObject);
                std::string text { json.dump() };
                std::ostream output { &buffer };
                output
                    << std::format(
                        Mbs::c_responseHeaderTemplate,
                        Meta::toUnderlying(m_status),
                        Mbs::c_statusStrings.at(m_status),
                        Mbs::c_jsonMimeType,
                        text.size()
                    )
                    << text;
            }
        }
    };
}
