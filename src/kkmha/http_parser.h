// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "http_types.h"
#include "http_strings.h"
#include "http_request.h"
#include <cassert>
#include <istream>
#include <string>

namespace Http {
    class Parser {
        using Reader = void (Parser::*)(std::istream &);

        void parseMethod(std::istream &);
        void parseHeader(std::istream &);
        void parseBody(std::istream &);
        void dummyReader(std::istream &);

        Request & m_request;
        Reader m_reader { &Parser::parseMethod };
        size_t m_expectedSize { 0 };
        int m_step { 0 };

    public:
        Parser() = delete;
        Parser(const Parser &) = delete;
        Parser(Parser &&) = delete;
        explicit Parser(Request & request) : m_request(request) {}
        ~Parser() = default;

        Parser & operator=(const Parser &) = delete;
        Parser & operator=(Parser &&) = delete;

        void operator()(Asio::StreamBuffer &);

        [[nodiscard]]
        inline size_t expecting() const {
            if (
                m_request.m_response.m_status == Status::Ok
                && m_step >= 2
                && m_request.m_body.size() < m_expectedSize
            ) {
                return m_expectedSize - m_request.m_body.size();
            }
            return 0;
        }

        inline void complete() {
            assert(Mbs::c_statusStrings.contains(m_request.m_response.m_status));
            if (m_request.m_response.m_status != Status::Ok && m_request.emptyResponse()) {
                m_request.m_response.m_data = Mbs::c_statusStrings.at(m_request.m_response.m_status);
            }
        }
    };
}
