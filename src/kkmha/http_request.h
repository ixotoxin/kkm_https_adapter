// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "http_types.h"
#include "http_response.h"
#include "asio.h"
#include <lib/meta.h>
#include <lib/datetime.h>
#include <cassert>
#include <utility>
#include <atomic>
#include <vector>

namespace Http {
    class Request {
        using SequenceType = int64_t;

        constexpr static SequenceType c_idMask { 0xfff };
        inline static std::atomic<SequenceType> s_sequence { 1 + (DateTime::windows() & c_idMask) };

    public:
        using IdType = uint16_t;

        Header m_header {};
        Response m_response {};
        std::string m_verb {};
        std::string m_path {};
        std::string m_body {};
        Asio::IpAddress m_remote;
        std::vector<std::string> m_hint {};

        Method m_method { Method::NotImplemented };
        const IdType m_id;

        Request() = delete;

        inline explicit Request(Asio::IpAddress && remote)
        : m_remote { std::forward<Asio::IpAddress>(remote) },
          m_id { static_cast<IdType>(s_sequence.fetch_add(1 + (DateTime::windows() & c_idMask))) } {};

        Request(const Request &) = delete;
        Request(Request &&) = delete;
        ~Request() = default;

        Request & operator=(const Request &) = delete;
        Request & operator=(Request &&) = delete;

        [[nodiscard]]
        inline bool emptyResponse() const {
            return m_response.m_data.index() == 0
                || (
                    m_response.m_data.index() == 1
                    && std::get<1>(m_response.m_data).empty()
                   )
                || (
                    m_response.m_data.index() == 2
                    && !std::get<2>(m_response.m_data)
                    /*&& !*std::get<2>(m_response.m_data)*/
                   );
        }
    };
}
