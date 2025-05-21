// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "library/datetime.h"
#include "library/json.h"
#include <variant>
#include <vector>
#include <unordered_map>
#include "messages.h"
#include "log.h"
#include "asio.h"

namespace Http {
    namespace Wcs {
        using Basic::Wcs::c_somethingWrong;
    }

    namespace Mbs {
        using Basic::Mbs::c_somethingWrong;

        constexpr std::string_view c_jsonResponseHeaderTemplate {
            "HTTP/1.1 {} {}\r\n"
            "Connection: close\r\n"
            "Pragma: no-cache\r\n"
            "Cache-Control: no-cache, private\r\n"
            "Content-Type: {}\r\n"
            "Content-Length: {}\r\n"
            "\r\n"
        };

        constexpr std::string_view c_jsonType { "application/json" };
    }

    enum class Method { NotImplemented, Get, Post };

    enum class Status {
        Ok = 200,
        MovedTemporarily = 302,
        BadRequest = 400,
        Unauthorized,
        Forbidden = 403,
        NotFound,
        MethodNotAllowed,
        ImATeapot = 418,
        InternalServerError = 500,
        NotImplemented,
        ServiceUnavailable = 503,
        UnknownError = 520,
    };

    inline const std::unordered_map<Status, std::string> s_statusStrings {
        { Status::Ok, "OK" },
        { Status::MovedTemporarily, "Moved Temporarily" },
        { Status::BadRequest, "Bad Request" },
        { Status::Unauthorized, "Unauthorized" },
        { Status::Forbidden, "Forbidden" },
        { Status::NotFound, "Not Found" },
        { Status::MethodNotAllowed, "Method Not Allowed" },
        { Status::ImATeapot, "I’m a teapot" },
        { Status::InternalServerError, "Internal Server Error" },
        { Status::NotImplemented, "Not Implemented" },
        { Status::ServiceUnavailable, "Service Unavailable" },
        { Status::UnknownError, "Unknown Error" },
    };

    struct ResponseData {
        ResponseData() = default;
        ResponseData(const ResponseData &) = default;
        ResponseData(ResponseData &&) = default;
        virtual ~ResponseData() = default;

        ResponseData & operator=(const ResponseData &) = default;
        ResponseData & operator=(ResponseData &&) = default;

        virtual void render(Asio::StreamBuffer &, Status) = 0;
    };

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
                Nln::Json json({
                    { "!success", m_status < Status::BadRequest },
                    { "!message", m_data.index() == 1 ? std::get<1>(m_data) : s_statusStrings.at(m_status) }
                }, false, Nln::EmptyJsonObject);
                std::string text { json.dump() };
                std::ostream output { &buffer };
                output
                    << std::format(
                        Mbs::c_jsonResponseHeaderTemplate,
                        static_cast<int>(m_status),
                        s_statusStrings.at(m_status),
                        Mbs::c_jsonType,
                        text.size()
                    )
                    << text;
            }
        }
    };

    using RequestHeader = std::unordered_map<std::string, std::string>;

    struct Request {
        RequestHeader m_header {};
        Response m_response {};
        std::string m_verb {};
        std::string m_path {};
        std::string m_body {};
        Asio::IpAddress m_remote {};
        std::vector<std::string> m_hint {};
        Method m_method { Method::NotImplemented };
        const uint16_t m_id;

        Request() = delete;

        inline explicit Request(Asio::IpAddress remote)
        : m_remote(std::move(remote)),
          m_id(static_cast<decltype(m_id)>(s_sequence.fetch_add(DateTime::windows() & 0x0fff))) {};

        Request(const Request &) = delete;
        Request(Request &&) = delete;
        ~Request() = default;

        Request & operator=(const Request &) = delete;
        Request & operator=(Request &&) = delete;

        [[nodiscard]]
        inline bool emptyResponse() const {
            return m_response.m_data.index() == 0
                || (m_response.m_data.index() == 1 && std::get<1>(m_response.m_data).empty())
                || (m_response.m_data.index() == 2 && !std::get<2>(m_response.m_data));
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

    private:
        inline static std::atomic<int64_t> s_sequence { DateTime::windows() & 0xffff };
    };

    class RequestHandler {
    public:
        RequestHandler() = default;
        RequestHandler(const RequestHandler &) = default;
        RequestHandler(RequestHandler &&) = default;
        virtual ~RequestHandler() = default;

        RequestHandler & operator=(const RequestHandler &) = default;
        RequestHandler & operator=(RequestHandler &&) = default;

        [[nodiscard]] virtual bool asyncReady() const noexcept = 0;
        [[nodiscard]] virtual bool isCaching() const noexcept = 0;
        virtual void operator()(Request &) const noexcept = 0;
    };

    inline std::string s_secret { c_defSecret };
    inline bool s_loopbackWithoutSecret { c_loopbackWithoutSecret };

    inline void authenticate(Request & request) {
        if (
            request.m_response.m_status < Http::Status::BadRequest
            && (!s_loopbackWithoutSecret || !Asio::isLoopback(request.m_remote))
        ) {
            auto it = request.m_header.find("x-secret");
            if (it == request.m_header.end() || it->second.empty() || it->second != s_secret) {
                request.m_response.m_status = Http::Status::Forbidden;
                request.m_response.m_data.emplace<1>(Mbs::c_forbidden);
                tsLogError(Wcs::c_forbidden, request.m_id);
            }
        }
    }
}
