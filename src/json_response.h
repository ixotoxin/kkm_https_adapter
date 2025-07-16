// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "http.h"

namespace Http {
    struct JsonResponse : public ResponseData {
        Nln::Json m_data;

        JsonResponse()
        : ResponseData(), m_data(Nln::EmptyJsonObject) {
            assert(m_data.is_object());
        }

        [[maybe_unused]]
        explicit JsonResponse(const Nln::Json & data)
        : ResponseData(), m_data(data) {
            assert(m_data.is_object());
        }

        [[maybe_unused]]
        explicit JsonResponse(Nln::Json && data)
        : ResponseData(), m_data(std::forward<Nln::Json>(data)) {
            assert(m_data.is_object());
        }

        [[maybe_unused]]
        JsonResponse(bool success, const std::string & message)
        : ResponseData(), m_data({{ "!success", success }, { "!message", message }}) {
            assert(m_data.is_object());
        }

        [[maybe_unused]]
        JsonResponse(bool success, std::string && message)
        : ResponseData(), m_data({{ "!success", success }, { "!message", std::forward<std::string>(message) }}) {
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

        void render(Asio::StreamBuffer & buffer, Status status) override {
            assert(s_statusStrings.contains(status));
            assert(m_data.is_object());
            if (!m_data.contains("!success") || !m_data["!success"].is_boolean()) {
                m_data["!success"] = status < Status::BadRequest;
            }
            if (!m_data.contains("!message") || !m_data["!message"].is_string()) {
                m_data["!message"] = s_statusStrings.at(status);
            }
            std::string text { m_data.dump() };
            std::ostream output { &buffer };
            output
                << std::format(
                    Mbs::c_jsonResponseHeaderTemplate,
                    static_cast<int>(status),
                    s_statusStrings.at(status),
                    Mbs::c_jsonType,
                    text.size()
                )
                << text;
        }
    };
}
