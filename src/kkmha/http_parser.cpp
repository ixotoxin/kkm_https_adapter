// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "http_parser.h"
#include "http_defaults.h"
#include <lib/text.h>
#include <log/write.h>

namespace Http {
    using Basic::Failure;

    void Parser::parseMethod(std::istream & stream) {
        std::string line;
        std::getline(stream, line);

        auto pos1 = line.find_first_of(' ');
        if (pos1 == std::string::npos) {
            m_request.m_response.m_status = Status::BadRequest;
            m_reader = &Parser::dummyParser;
            return;
        }
        m_request.m_verb.assign(line.data(), line.data() + pos1);

        ++pos1;
        auto pos2 = line.find_first_of(" ?#\r\n", pos1);
        if (pos2 == std::string::npos || pos1 == pos2) {
            m_request.m_response.m_status = Status::BadRequest;
            m_reader = &Parser::dummyParser;
            return;
        }
        m_request.m_path.assign(line.data() + pos1, line.data() + pos2);

        Text::splitTo(m_request.m_hint, Text::lowered<std::string>({ line.c_str(), pos2 }), " /\\");
        if (m_request.m_hint.empty()) {
            m_request.m_response.m_status = Status::BadRequest;
            m_reader = &Parser::dummyParser;
            return;
        }

        if (m_request.m_hint[0] == "post") {
            m_request.m_method = Method::Post;
        } else if (m_request.m_hint[0] == "get") {
            m_request.m_method = Method::Get;
        } else {
            m_request.m_response.m_status = Status::NotImplemented;
            m_reader = &Parser::dummyParser;
            return;
        }

        m_reader = &Parser::parseHeader;
        ++m_step;
    }

    void Parser::parseHeader(std::istream & stream) {
        std::string line;
        std::getline(stream, line);
        Text::trim(line);

        if (line.empty()) {
            if (m_request.m_method == Method::Post && m_expectedSize) {
                ++m_step;
                m_reader = &Parser::parseBody;
            } else {
                m_reader = &Parser::dummyParser;
            }
            return;
        }

        std::string field, value;
        Text::splitVariable(line, field, value);
        m_request.m_header[field] = value;
        if (field == "content-length") {
            m_expectedSize = Text::cast<size_t>(value);
            if (m_expectedSize > c_requestBodySizeLimit) {
                LOG_ERROR_TS(Wcs::c_bodySizeLimitExceeded, m_request.m_id);
                m_expectedSize = 0;
                m_request.m_response.m_status = Status::BadRequest;
                m_request.m_response.m_data.emplace<1>(Mbs::c_bodySizeLimitExceeded);
                m_reader = &Parser::dummyParser;
            } else {
                m_request.m_body.reserve(m_expectedSize + 1);
            }
        }
    }

    void Parser::parseBody(std::istream & stream) {
        char buffer[c_parserBufferSize];
        while (stream.read(buffer, sizeof(buffer))) {
            m_request.m_body.append(buffer, sizeof(buffer));
        }
        m_request.m_body.append(buffer, stream.gcount());
    }

    void Parser::dummyParser(std::istream & stream) { // NOLINT(*-convert-member-functions-to-static)
        // ISSUE: А надо ли очищать стрим? Разобраться.
        // TODO: Оптимизировать очистку стрима.
        char buffer[c_parserBufferSize];
        while (stream.good()) {
            stream.read(buffer, sizeof(buffer));
        }
    }

    void Parser::operator()(Asio::StreamBuffer & buffer) {
        try {
            std::istream input(&buffer);
            std::noskipws(input);
            while (input.good()) {
                (this->*m_reader)(input);
            }
            return; /** Не удаляй, смотри дальше. **/
        } catch (const Failure & e) {
            LOG_ERROR_TS(e);
        } catch (const std::exception & e) {
            LOG_ERROR_TS(e);
        } catch (...) {
            LOG_ERROR_TS(Basic::Wcs::c_somethingWrong);
        }
        if (m_request.m_response.m_status == Status::Ok) {
            m_request.m_response.m_status = Status::BadRequest;
        }
        m_reader = &Parser::dummyParser;
    }
}
