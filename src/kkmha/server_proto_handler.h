// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "server_strings.h"
#include "http_request.h"
#include <log/write.h>
#include <cassert>

namespace Server {
    class ProtoHandler {
    public:
        ProtoHandler() = default;
        ProtoHandler(const ProtoHandler &) = default;
        ProtoHandler(ProtoHandler &&) = default;
        virtual ~ProtoHandler() = default;

        ProtoHandler & operator=(const ProtoHandler &) = default;
        ProtoHandler & operator=(ProtoHandler &&) = default;

        [[nodiscard]] virtual bool asyncReady() const noexcept = 0;
        virtual void operator()(Http::Request &) const noexcept = 0;

        static void fail(
            Http::Request & request,
            const Http::Status status,
            const std::string & message,
            const SrcLoc::Point & location = SrcLoc::Point::current()
        ) {
            assert(Meta::toUnderlying(status) >= 400);
            if (Log::s_appendLocation) {
                LOG_ERROR_TS(Mbs::c_prefixedTextWithSource, request.m_id, message, SrcLoc::toMbs(location));
            } else {
                LOG_ERROR_TS(Mbs::c_prefixedText, request.m_id, message);
            }
            request.m_response.m_status = status;
            request.m_response.m_data = message;
        }

        static void fail(
            Http::Request & request,
            const Http::Status status,
            std::string && message,
            const SrcLoc::Point & location = SrcLoc::Point::current()
        ) {
            assert(Meta::toUnderlying(status) >= 400);
            if (Log::s_appendLocation) {
                LOG_ERROR_TS(Mbs::c_prefixedTextWithSource, request.m_id, message, SrcLoc::toMbs(location));
            } else {
                LOG_ERROR_TS(Mbs::c_prefixedText, request.m_id, message);
            }
            request.m_response.m_status = status;
            request.m_response.m_data.emplace<std::string>(std::forward<std::string>(message));
        }

        static void fail(
            Http::Request & request,
            const Http::Status status,
            const std::string_view message,
            const SrcLoc::Point & location = SrcLoc::Point::current()
        ) {
            assert(Meta::toUnderlying(status) >= 400);
            if (Log::s_appendLocation) {
                LOG_ERROR_TS(Mbs::c_prefixedTextWithSource, request.m_id, message, SrcLoc::toMbs(location));
            } else {
                LOG_ERROR_TS(Mbs::c_prefixedText, request.m_id, message);
            }
            request.m_response.m_status = status;
            request.m_response.m_data.emplace<std::string>(message);
        }

        static void fail(
            Http::Request & request,
            const Http::Status status,
            const char * message,
            const SrcLoc::Point & location = SrcLoc::Point::current()
        ) {
            assert(Meta::toUnderlying(status) >= 400);
            if (Log::s_appendLocation) {
                LOG_ERROR_TS(Mbs::c_prefixedTextWithSource, request.m_id, message, SrcLoc::toMbs(location));
            } else {
                LOG_ERROR_TS(Mbs::c_prefixedText, request.m_id, message);
            }
            request.m_response.m_status = status;
            request.m_response.m_data.emplace<std::string>(message);
        }
    };
}
