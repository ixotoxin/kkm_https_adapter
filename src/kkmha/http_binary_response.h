// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "http_types.h"
#include "http_strings.h"
#include "http_proto_response.h"
#include <lib/meta.h>
#include <type_traits>
#include <cassert>
#include <memory>
#include <ostream>
#include <format>

namespace Http {
    using Unique = std::unique_ptr<char[]>;
    using Shared = std::shared_ptr<char[]>;
    using Weak = std::weak_ptr<char[]>;
    using Regular = std::add_pointer_t<char>;

    template <typename T>
    concept CustomPointer
        = std::is_same_v<T, Unique> || std::is_same_v<T, Shared>
          || std::is_same_v<T, Weak> || std::is_same_v<T, Regular>;

    template<CustomPointer> struct isSmartPointer : std::false_type {};
    template<> struct isSmartPointer<Unique> : std::true_type {};
    template<> struct isSmartPointer<Shared> : std::true_type {};
    template<> struct isSmartPointer<Weak> : std::true_type {};

    template<CustomPointer T>
    inline constexpr bool isSmart = isSmartPointer<T>::value;

    template<CustomPointer T>
    struct BinaryResponse final : ProtoResponse {
        std::string m_mimeType {};
        T m_data { nullptr };
        size_t m_size { 0 };

        BinaryResponse() = default;
        BinaryResponse(const BinaryResponse &) = delete;
        BinaryResponse(BinaryResponse &&) = delete;

        BinaryResponse(T data, const size_t size, const std::string_view mimeType)
        : m_mimeType { mimeType }, m_data { data }, m_size { size } {}

        ~BinaryResponse() override = default;

        BinaryResponse & operator=(const BinaryResponse &) = delete;
        BinaryResponse & operator=(BinaryResponse &&) = delete;

        explicit operator bool() override {
            return m_data && m_size;
        }

        void render(Asio::StreamBuffer & buffer, const Status status) override {
            assert(Mbs::c_statusStrings.contains(status));
            std::ostream output { &buffer };
            if (m_data && m_size) {
                output
                    << std::format(
                        Mbs::c_staticResponseHeaderTemplate,
                        Meta::toUnderlying(status),
                        Mbs::c_statusStrings.at(status),
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
                        Meta::toUnderlying(status),
                        Mbs::c_statusStrings.at(status),
                        m_mimeType,
                        0
                    );
            }
        }
    };
}
