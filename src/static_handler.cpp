// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "static_handler.h"
#include "library/utils.h"
#include <fstream>
#include "messages.h"
#include "solid_response.h"
#include "binary_response.h"
#include "http_cache.h"

namespace Http {
    namespace Wcs {
        using Basic::Wcs::c_invalidValue;
    }

    namespace Mbs {
        constexpr std::string_view c_redirect {
            "HTTP/1.1 302 Moved Temporarily\r\n"
            "Connection: close\r\n"
            "Location: {}\r\n"
            "Pragma: no-cache\r\n"
            "Cache-Control: no-cache, private\r\n"
            "Content-Length: 0\r\n"
            "\r\n"
        };
    }

    // using namespace std::string_literals;
    using namespace std::string_view_literals;
    using Basic::Failure;
    using Basic::DataError;

    std::string filterFileName(const std::string & fileName) {
        std::string result { /*Text::trimmed(*/fileName/*)*/ };
        if (
            result.find_first_of(
                "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f"
                "\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f"
                "<>:\"/\\|?*"sv
            ) != std::string_view::npos
            || result.empty()
            || result.starts_with("."sv)
            || result.ends_with("."sv)
            || result.starts_with(" "sv)
            || result.ends_with(" "sv)
        ) {
            throw DataError(Wcs::c_invalidValue); // NOLINT(*-exception-baseclass)
        }
        return result;
    }

    void redirectToIndex(Request & request) {
        std::string newPath { request.m_path };
        if (newPath.find_last_of("/"sv) != newPath.size() - 1) {
            newPath.append("/"sv);
        }
        newPath.append(s_indexFile);
        request.m_response.m_status = Status::MovedTemporarily;
        request.m_response.m_data = std::make_shared<SolidResponse>(std::format(Mbs::c_redirect, newPath));
    }

    bool StaticHandler::asyncReady() const noexcept {
        return false;
    }

    void StaticHandler::operator()(Request & request) const noexcept try {
        assert(request.m_response.m_status == Status::Ok);
        if (request.m_response.m_status != Status::Ok) {
            return;
        }
        if (request.m_method != Method::Get) {
            return request.fail(Status::MethodNotAllowed, Mbs::c_methodNotAllowed);
        }
        if (request.m_path.size() < 3 || !System::isGoodPath(request.m_path) || !request.m_path.starts_with('/')) {
            return request.fail(Status::BadRequest, Mbs::c_badPath);
        }

        std::filesystem::path path { s_staticDirectory };

        {
            std::wstring requestedPath { Text::convert(request.m_path) }, path2;
            auto pos = requestedPath.find_first_not_of(L'/');
            if (pos != std::string::npos) {
                pos = requestedPath.find_first_of(L'/', pos);
            }
            if (pos != std::string::npos) {
                pos = requestedPath.find_first_not_of(L'/', pos);
            }
            if (pos == std::string::npos) {
                return redirectToIndex(request);
            } else {
                path2.assign(requestedPath, pos);
            }
            path /= path2;
            if (std::filesystem::is_directory(path)) {
                return redirectToIndex(request);
            }
            if (!std::filesystem::is_regular_file(path)) {
                return request.fail(Status::NotFound, Mbs::c_notFound);
            }
        }

        std::error_code error;

        {
            auto status = std::filesystem::status(path, error);
            if (error) {
                return request.fail(Status::NotFound, error.message());
            }
            if (status.type() != std::filesystem::file_type::regular) {
                return request.fail(Status::NotFound, Mbs::c_badFilesystemEntityType);
            }
        }

        Cache::maintain();
        Cache::Key cacheKey { "static::::" };
        cacheKey.append(Text::convert(path.c_str()));
        auto cacheEntry = Cache::load(cacheKey);

        if (cacheEntry) {
            auto fileTime = std::filesystem::last_write_time(path, error);
            if (error) {
                return request.fail(Status::NotFound, error.message());
            }
            if (DateTime::cast<DateTime::Point>(fileTime) <= cacheEntry->m_cachedAt) {
                request.m_response.m_status = cacheEntry->m_status;
                request.m_response.m_data = cacheEntry->m_data;
                tsLogDebug(Wcs::c_fromCache, request.m_id);
                return;
            }
        }

        size_t fileSize {};

        {
            fileSize = std::filesystem::file_size(path, error);
            if (error) {
                return request.fail(Status::NotFound, error.message());
            }
            if (fileSize > c_fileSizeLimit) {
                return request.fail(Status::NotFound, Mbs::c_fileTooLarge);
            }
        }

        auto response = std::make_shared<BinaryResponse<Shared>>();
        response->m_size = fileSize;

        {
            auto ext = Text::lowered(Text::convert(path.extension().c_str()));
            if (!ext.empty() && s_mimeMap.contains(ext)) {
                response->m_mimeType = s_mimeMap[ext].get<std::string>();
            } else if (s_enableUnknownType) {
                response->m_mimeType = c_defMimeType;
            } else {
                return request.fail(Status::NotFound, Mbs::c_unknownMimeType);
            }
        }

        if (fileSize > 0) {
            response->m_data = std::make_shared<char[]>(fileSize);
            std::ifstream file { path, std::ios::binary };
            if (file.good()) {
                file.read(response->m_data.get(), static_cast<std::streamsize>(fileSize));
            }
        }

        Cache::store(cacheKey, Cache::expiresAfter(c_fileCacheLifeTime), request.m_response.m_status, response);
        assert(request.m_response.m_status == Status::Ok);
        if (request.m_response.m_status == Status::Ok) {
            request.m_response.m_data = response;
        }

    } catch (const Failure & e) {
        request.fail(Status::InternalServerError, Text::convert(e.what()), e.where());
    } catch (const std::exception & e) {
        request.fail(Status::InternalServerError, e.what());
    } catch (...) {
        request.fail(Status::InternalServerError, Basic::Mbs::c_somethingWrong);
    }
}
