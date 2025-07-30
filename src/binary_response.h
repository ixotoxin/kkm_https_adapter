// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "http_res_data.h"

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

    using Shared = std::shared_ptr<char[]>;
    using Weak = std::weak_ptr<char[]>;
    using Regular = std::add_pointer_t<char>;

    template <typename T>
    concept CustomPointer = std::is_same_v<T, Regular> || std::is_same_v<T, Shared> || std::is_same_v<T, Weak>;

    template<CustomPointer> struct isSmartPointer : std::false_type {};
    template<> struct isSmartPointer<Shared> : std::true_type {};
    template<> struct isSmartPointer<Weak> : std::true_type {};

    template<CustomPointer T>
    inline constexpr bool isSmart = isSmartPointer<T>::value;

    template<CustomPointer T>
    struct BinaryResponse : public ResponseData {
        std::string m_mimeType {};
        T m_data { nullptr };
        size_t m_size { 0 };

        BinaryResponse() = default;
        BinaryResponse(const BinaryResponse &) = delete;
        BinaryResponse(BinaryResponse &&) = delete;

        BinaryResponse(T data, size_t size, std::string_view mimeType) // NOLINT(*-unnecessary-value-param)
        : m_mimeType { mimeType }, m_data { data }, m_size { size } {}

        ~BinaryResponse() override = default;

        BinaryResponse & operator=(const BinaryResponse &) = delete;
        BinaryResponse & operator=(BinaryResponse &&) = delete;

        explicit operator bool() override {
            return m_data && m_size;
        }

        void render(Asio::StreamBuffer & buffer, Status status) override {
            assert(s_statusStrings.contains(status));
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
                if constexpr (isSmart<T>) {
                    output.write(m_data.get(), static_cast<std::streamsize>(m_size));
                } else {
                    output.write(m_data, static_cast<std::streamsize>(m_size));
                }
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
