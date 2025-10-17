// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "http_types.h"
#include "http_strings.h"
#include "http_proto_response.h"
#include <lib/meta.h>
#include <lib/json.h>
#include <cassert>
#include <utility>
#include <ostream>
#include <format>

namespace Http {
    struct JsonResponse final : ProtoResponse {
        Nln::Json m_data;

        JsonResponse()
        : ProtoResponse(), m_data(Nln::EmptyJsonObject) {
            assert(m_data.is_object());
        }

        [[maybe_unused]]
        explicit JsonResponse(const Nln::Json & data)
        : ProtoResponse(), m_data(data) {
            assert(m_data.is_object());
        }

        [[maybe_unused]]
        explicit JsonResponse(Nln::Json && data)
        : ProtoResponse(), m_data(std::forward<Nln::Json>(data)) {
            assert(m_data.is_object());
        }

        [[maybe_unused]]
        JsonResponse(bool success, const std::string & message)
        : ProtoResponse(),
          m_data({ { Json::Mbs::c_successKey, success }, { Json::Mbs::c_messageKey, message } }) {
            assert(m_data.is_object());
        }

        [[maybe_unused]]
        JsonResponse(bool success, std::string && message)
        : ProtoResponse(),
          m_data({ { Json::Mbs::c_successKey, success }, { Json::Mbs::c_messageKey, std::forward<std::string>(message) } }) {
            assert(m_data.is_object());
        }

        JsonResponse(const JsonResponse &) = delete;
        JsonResponse(JsonResponse &&) = delete;
        ~JsonResponse() override = default;

        JsonResponse & operator=(const JsonResponse &) = delete;
        JsonResponse & operator=(JsonResponse &&) = delete;

        explicit operator bool() override {
            return !m_data.empty();
        }

        void render(Asio::StreamBuffer & buffer, const Status status) override {
            assert(Mbs::c_statusStrings.contains(status));
            assert(m_data.is_object());
            if (!m_data.contains(Json::Mbs::c_successKey) || !m_data[Json::Mbs::c_successKey].is_boolean()) {
                m_data[Json::Mbs::c_successKey] = status < Status::BadRequest;
            }
            if (!m_data.contains(Json::Mbs::c_messageKey) || !m_data[Json::Mbs::c_messageKey].is_string()) {
                m_data[Json::Mbs::c_messageKey] = Mbs::c_statusStrings.at(status);
            }
            const std::string text { m_data.dump() };
            std::ostream output { &buffer };
            output
                << std::format(
                    Mbs::c_responseHeaderTemplate,
                    Meta::toUnderlying(status),
                    Mbs::c_statusStrings.at(status),
                    Mbs::c_jsonMimeType,
                    text.size()
                )
                << text;
        }
    };
}
