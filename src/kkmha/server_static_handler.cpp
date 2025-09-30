// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "server_static_handler.h"
#include "server_static_variables.h"
#include "server_static_strings.h"
#include "http_solid_response.h"
#include "http_binary_response.h"
#include "server_cache_strings.h"
#include "server_cache_core.h"
#include <lib/except.h>
#include <lib/path.h>
#include <fstream>

namespace Server::Static {
    using namespace std::string_view_literals;

    using Basic::Failure;

    void redirectToIndex(Http::Request & request) {
        std::string newPath { request.m_path };
        if (newPath.find_last_of("/"sv) != newPath.size() - 1) {
            newPath.append("/"sv);
        }
        newPath.append(s_indexFile);
        request.m_response.m_status = Http::Status::MovedTemporarily;
        request.m_response.m_data
            = std::make_shared<Http::SolidResponse>(std::format(Mbs::c_redirectResponseTemplate, newPath));
    }

    bool Handler::asyncReady() const noexcept {
        return false;
    }

    void Handler::operator()(Http::Request & request) const noexcept try {
        assert(request.m_response.m_status == Http::Status::Ok);
        // if (request.m_response.m_status != Status::Ok) {
        //     return;
        // }

        if (request.m_method != Http::Method::Get) {
            return request.fail(Http::Status::MethodNotAllowed, Http::Mbs::c_methodNotAllowed);
        }
        if (request.m_path.size() < 3 || !Path::isGood(request.m_path) || !request.m_path.starts_with('/')) {
            return request.fail(Http::Status::BadRequest, Http::Mbs::c_badPath);
        }

        std::filesystem::path path { s_directory };

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
                return request.fail(Http::Status::NotFound, Http::Mbs::c_notFound);
            }
        }

        std::error_code error;

        {
            auto status = std::filesystem::status(path, error);
            if (error) {
                return request.fail(Http::Status::NotFound, error.message());
            }
            if (status.type() != std::filesystem::file_type::regular) {
                return request.fail(Http::Status::NotFound, Mbs::c_badFilesystemEntityType);
            }
        }

        Cache::maintain();
        Cache::Key cacheKey { "static::::" };
        cacheKey.append(Text::convert(path.c_str()));
        auto cacheEntry = Cache::load(cacheKey);

        if (cacheEntry) {
            auto fileTime = std::filesystem::last_write_time(path, error);
            if (error) {
                return request.fail(Http::Status::NotFound, error.message());
            }
            if (DateTime::cast<DateTime::Point>(fileTime) <= cacheEntry->m_cachedAt) {
                request.m_response.m_status = cacheEntry->m_status;
                request.m_response.m_data = cacheEntry->m_data;
                LOG_DEBUG_TS(Cache::Wcs::c_fromCache, request.m_id);
                return;
            }
        }

        size_t fileSize {};

        {
            fileSize = std::filesystem::file_size(path, error);
            if (error) {
                return request.fail(Http::Status::NotFound, error.message());
            }
            if (fileSize > c_fileSizeLimit) {
                return request.fail(Http::Status::NotFound, Mbs::c_fileTooLarge);
            }
        }

        auto response = std::make_shared<Http::BinaryResponse<Http::Shared>>();
        response->m_size = fileSize;

        {
            auto ext = Text::lowered(Text::convert(path.extension().c_str()));
            if (!ext.empty() && s_mimeMap.contains(ext)) {
                response->m_mimeType = s_mimeMap[ext].get<std::string>();
            } else if (s_enableUnknownType) {
                response->m_mimeType = c_defMimeType;
            } else {
                return request.fail(Http::Status::NotFound, Mbs::c_unknownMimeType);
            }
        }

        if (fileSize > 0) {
            response->m_data = std::make_shared_for_overwrite<char[]>(fileSize);
            std::ifstream file { path, std::ios::binary };
            if (file.good()) {
                file.read(response->m_data.get(), static_cast<std::streamsize>(fileSize));
            }
        }

        Cache::store(cacheKey, Cache::expiresAfter(c_fileCacheLifeTime), request.m_response.m_status, response);
        assert(request.m_response.m_status == Http::Status::Ok);
        if (request.m_response.m_status == Http::Status::Ok) {
            request.m_response.m_data = std::move(response);
        }

    } catch (const Failure & e) {
        request.fail(Http::Status::InternalServerError, Text::convert(e.what()), e.where());
    } catch (const std::exception & e) {
        request.fail(Http::Status::InternalServerError, e.what());
    } catch (...) {
        request.fail(Http::Status::InternalServerError, Basic::Mbs::c_somethingWrong);
    }
}
