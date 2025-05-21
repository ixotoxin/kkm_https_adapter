// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "http.h"

namespace Http {
    namespace Mbs {
        constexpr std::string_view c_staticResponseHeaderTemplate {
            "HTTP/1.1 {} {}\r\n"
            "Connection: close\r\n"
            "Content-Type: {}\r\n"
            "Content-Length: {}\r\n"
            "\r\n"
        };
    }

    struct BinaryResponse : public ResponseData {
        std::string m_mimeType {};
        std::shared_ptr<char[]> m_data { nullptr };
        size_t m_size { 0 };

        BinaryResponse() = default;
        BinaryResponse(const BinaryResponse &) = delete;
        BinaryResponse(BinaryResponse &&) = delete;
        ~BinaryResponse() override = default;

        BinaryResponse & operator=(const BinaryResponse &) = delete;
        BinaryResponse & operator=(BinaryResponse &&) = delete;

        void render(Asio::StreamBuffer & buffer, Status status) override {
            std::ostream output { &buffer };
            if (m_data && m_size) {
                output
                    << std::format(
                        Mbs::c_staticResponseHeaderTemplate,
                        static_cast<int>(status),
                        s_statusStrings.at(status),
                        m_mimeType,
                        m_size
                    );
                output.write(m_data.get(), m_size);
            } else {
                output
                    << std::format(
                        Mbs::c_staticResponseHeaderTemplate,
                        static_cast<int>(status),
                        s_statusStrings.at(status),
                        m_mimeType,
                        0
                    );
            }
        }
    };
}
