// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "library/datetime.h"
#include "http_response.h"
#include <vector>
#include "messages.h"
#include "log.h"

namespace Http {
    class Request {
        using SequenceType = int64_t;
        using IdType = uint16_t;
        constexpr static SequenceType c_idMask = 0xfff;

    public:
        RequestHeader m_header {};
        Response m_response {};
        std::string m_verb {};
        std::string m_path {};
        std::string m_body {};
        Asio::IpAddress m_remote;
        std::vector<std::string> m_hint {};

    private:
        inline static std::atomic<SequenceType> s_sequence { 1 + (DateTime::windows() & c_idMask) };

    public:
        Method m_method { Method::NotImplemented };
        const IdType m_id;

        Request() = delete;

        inline explicit Request(Asio::IpAddress && remote)
        : m_remote(std::forward<Asio::IpAddress>(remote)),
          m_id(static_cast<IdType>(s_sequence.fetch_add(1 + (DateTime::windows() & c_idMask)))) {};

        Request(const Request &) = delete;
        Request(Request &&) = delete;
        ~Request() = default;

        Request & operator=(const Request &) = delete;
        Request & operator=(Request &&) = delete;

        inline void authenticate() {
            if (
                m_response.m_status < Http::Status::BadRequest
                && (!s_loopbackWithoutSecret || !Asio::isLoopback(m_remote))
            ) {
                auto it = m_header.find("x-secret");
                if (it == m_header.end() || it->second.empty() || it->second != s_secret) {
                    m_response.m_status = Http::Status::Forbidden;
                    m_response.m_data.emplace<1>(Mbs::c_forbidden);
                    tsLogError(Wcs::c_forbidden, m_id);
                }
            }
        }

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

        [[maybe_unused]]
        inline void fail(
            const Status status,
            const std::string_view message,
            const std::source_location & location = std::source_location::current()
        ) {
            assert(static_cast<int>(status) >= 400);
            m_response.m_status = status;
            std::string message2 { message };
            m_response.m_data = message2;
            if (Log::s_appendLocation) {
                message2 += location;
            }
            tsLogError(Mbs::c_prefixedText, m_id, message2);
        }
    };
}
