// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "server_kkmop_handler.h"
#include "server_kkmop_defauls.h"
#include "server_kkmop_strings.h"
#include "server_cache_strings.h"
#include "server_cache_core.h"
#include "http_json_response.h"
#include <lib/meta.h>
#include <debug/memprof.h>
#include <kkm/strings.h>
#include <kkm/device.h>
#include <kkm/callhelpers.h>
#include <cassert>
#include <utility>
#include <memory>
#include <unordered_map>

namespace Server::KkmOp {
    using namespace Kkm;

    static std::unordered_map<std::wstring, std::shared_ptr<KnownConnParams>> s_connParamsRegistry {};
    static std::mutex s_registryMutex {};

    struct Payload {
        using Id = decltype(Http::Request::m_id);

        const std::string m_serialNumber;
        const Nln::Json m_details;
        Nln::Json m_result;
        DateTime::Offset m_expiresAfter;
        Http::Status m_status { Http::Status::Ok };
        const Id m_requestId;

        Payload() = delete;

        Payload(
            std::string && serialNumber,
            Nln::Json && details,
            const Id requestId,
            const DateTime::Offset expiresAfter = 0s
        ) : m_serialNumber(std::forward<std::string>(serialNumber)),
            m_details(std::forward<Nln::Json>(details)), m_result(Nln::EmptyJsonObject),
            m_expiresAfter(expiresAfter), m_requestId(requestId) {
            assert(m_details.is_object());
            assert(m_result.is_object());
        }

        Payload(const Payload &) = delete;
        Payload(Payload &&) = delete;
        ~Payload() = default;

        Payload & operator=(const Payload &) = delete;
        Payload & operator=(Payload &&) = delete;

        [[maybe_unused]]
        inline void fail(
            const Http::Status status,
            const std::string_view message,
            const SrcLoc::Point & location = SrcLoc::Point::current()
        ) {
            assert(Meta::toUnderlying(status) >= 400);
            m_status = status;
            m_result[Json::Mbs::c_messageKey] = message;
            if (Log::s_appendLocation) {
                LOG_ERROR_TS(Server::Mbs::c_prefixedTextWithSource, m_requestId, message, SrcLoc::toMbs(location));
            } else {
                LOG_ERROR_TS(Server::Mbs::c_prefixedText, m_requestId, message);
            }
        }
    };

    [[maybe_unused]]
    inline std::shared_ptr<KnownConnParams> resolveConnParams(Payload & payload) {
        if (payload.m_serialNumber.empty()) {
            payload.fail(Http::Status::BadRequest, Server::Mbs::c_badRequest);
            return nullptr;
        }
        std::wstring wcSerialNumber { Text::convert(payload.m_serialNumber) };
        std::scoped_lock registryLock(s_registryMutex);
        if (s_connParamsRegistry.contains(wcSerialNumber)) {
            auto & params { s_connParamsRegistry.at(wcSerialNumber) };
            LOG_DEBUG_TS(Wcs::c_selectKkm, payload.m_requestId, wcSerialNumber, static_cast<std::wstring>(*params));
            return params;
        } else {
            auto [it, insert]
                = s_connParamsRegistry.try_emplace(wcSerialNumber, std::make_shared<KnownConnParams>(wcSerialNumber));
            if (insert) {
                LOG_DEBUG_TS(Wcs::c_selectKkm, payload.m_requestId, wcSerialNumber, static_cast<std::wstring>(*(it->second)));
                return it->second;
            }
        }
        payload.fail(
            Http::Status::NotFound,
            std::format(Mbs::c_notFound, payload.m_requestId, payload.m_serialNumber)
        );
        return nullptr;
    }

    template<class R>
    [[maybe_unused]]
    inline void callMethod(UndetailedMethod<R> method, Payload & payload) {
        if (auto connParams = resolveConnParams(payload); connParams) {
            callMethod(
                Device { *connParams, std::format(Wcs::c_requestPrefix, payload.m_requestId) },
                method, payload.m_result
            );
        }
    }

    template<class R, class D>
    [[maybe_unused]]
    inline void callMethod(DetailedMethod<R, D> method, Payload & payload) {
        if (auto connParams = resolveConnParams(payload); connParams) {
            callMethod(
                Device { *connParams, std::format(Wcs::c_requestPrefix, payload.m_requestId) },
                method, payload.m_details, payload.m_result
            );
        }
    }

    void learn(Payload & payload) {
        if (!payload.m_serialNumber.empty()) {
            return payload.fail(Http::Status::BadRequest, Server::Mbs::c_badRequest);
        }

        std::wstring connString;
        bool found = Json::handleKey(payload.m_details, "connParams", connString);
        if (!found) {
            return payload.fail(Http::Status::BadRequest, KKM_FMT(Kkm::Mbs::c_requiresProperty, "connParams"));
        }

        NewConnParams connParams { connString };
        Device kkm { connParams, std::format(Wcs::c_requestPrefix, payload.m_requestId) };
        std::wstring serialNumber { kkm.serialNumber() };
        LOG_DEBUG_TS(Wcs::c_getKkmInfo, payload.m_requestId, serialNumber);
        connParams.save(serialNumber);

        {
            std::scoped_lock registryLock(s_registryMutex);
            s_connParamsRegistry.insert_or_assign(
                serialNumber,
                std::make_shared<KnownConnParams>(connParams, serialNumber)
            );
        }

        StatusResult result;
        kkm.getStatus(result);
        kkm.printHello();
        payload.m_result << result;

        LOG_INFO_TS(Wcs::c_connParamsSaved, payload.m_requestId, serialNumber);
    }

    void resetRegistry(Payload & payload) {
        std::scoped_lock registryLock(s_registryMutex);
        s_connParamsRegistry.clear();
        if (!s_connParamsRegistry.empty()) {
            payload.fail(Http::Status::InternalServerError, Mbs::c_cantClearRegistry);
        }
        payload.m_expiresAfter = c_reportCacheLifeTime;
    }

    void baseStatus(Payload & payload) {
        FORCE_MEMORY_LEAK;
        callMethod(&Device::getStatus, payload);
        payload.m_expiresAfter = c_reportCacheLifeTime;
    }

    void status(Payload & payload) {
        FORCE_MEMORY_LEAK;
        if (payload.m_serialNumber.empty()) {
            return payload.fail(Http::Status::BadRequest, Server::Mbs::c_badRequest);
        }
        auto connParams = resolveConnParams(payload);
        if (connParams) {
            collectDataFromMethods(
                payload.m_result,
                Device { *connParams, std::format(Wcs::c_requestPrefix, payload.m_requestId) },
                &Device::getStatus,
                &Device::getShiftState,
                &Device::getReceiptState,
                &Device::getCashStat,
                &Device::getFndtOfdExchangeStatus,
                &Device::getFndtLastReceipt,
                &Device::getFndtLastDocument,
                &Device::getFndtErrors
            );
        }
        payload.m_expiresAfter = c_reportCacheLifeTime;
    }

    void fullStatus(Payload & payload) {
        FORCE_MEMORY_LEAK;
        if (payload.m_serialNumber.empty()) {
            return payload.fail(Http::Status::BadRequest, Server::Mbs::c_badRequest);
        }
        auto connParams = resolveConnParams(payload);
        if (connParams) {
            collectDataFromMethods(
                payload.m_result,
                Device { *connParams, std::format(Wcs::c_requestPrefix, payload.m_requestId) },
                &Device::getStatus,
                &Device::getShiftState,
                &Device::getReceiptState,
                &Device::getCashStat,
                &Device::getFndtOfdExchangeStatus,
                &Device::getFndtFnInfo,
                &Device::getFndtRegistrationInfo,
                &Device::getFndtLastRegistration,
                &Device::getFndtLastReceipt,
                &Device::getFndtLastDocument,
                &Device::getFndtErrors,
                &Device::getFfdVersion,
                &Device::getFwVersion
            );
        }
        payload.m_expiresAfter = c_reportCacheLifeTime;
    }

    void printDemo(Payload & payload) {
        callMethod(&Device::printDemo, payload);
        payload.m_expiresAfter = c_reportCacheLifeTime;
    }

    void printNonFiscalDocument(Payload & payload) {
        callMethod(&Device::printNonFiscalDocument, payload);
        payload.m_expiresAfter = c_reportCacheLifeTime;
    }

    void printInfo(Payload & payload) {
        callMethod(&Device::printInfo, payload);
        payload.m_expiresAfter = c_reportCacheLifeTime;
    }

    void printFnRegistrations(Payload & payload) {
        callMethod(&Device::printFnRegistrations, payload);
        payload.m_expiresAfter = c_reportCacheLifeTime;
    }

    void printOfdExchangeStatus(Payload & payload) {
        callMethod(&Device::printOfdExchangeStatus, payload);
        payload.m_expiresAfter = c_reportCacheLifeTime;
    }

    void printOfdTest(Payload & payload) {
        callMethod(&Device::printOfdTest, payload);
        payload.m_expiresAfter = c_reportCacheLifeTime;
    }

    void printCloseShiftReports(Payload & payload) {
        callMethod(&Device::printCloseShiftReports, payload);
        payload.m_expiresAfter = c_reportCacheLifeTime;
    }

    void printLastDocument(Payload & payload) {
        callMethod(&Device::printLastDocument, payload);
        payload.m_expiresAfter = c_reportCacheLifeTime;
    }

    void cashStat(Payload & payload) {
        callMethod(&Device::getCashStat, payload);
        payload.m_expiresAfter = c_reportCacheLifeTime;
    }

    void cashIn(Payload & payload) {
        callMethod(&Device::registerCashIn, payload);
    }

    void cashOut(Payload & payload) {
        callMethod(&Device::registerCashOut, payload);
    }

    void sell(Payload & payload) {
        callMethod(&Device::registerSell, payload);
    }

    void sellReturn(Payload & payload) {
        callMethod(&Device::registerSellReturn, payload);
    }

    void closeShift(Payload & payload) {
        callMethod(&Device::closeShift, payload);
        payload.m_expiresAfter = c_reportCacheLifeTime;
    }

    void reportX(Payload & payload) {
        callMethod(&Device::reportX, payload);
        payload.m_expiresAfter = c_reportCacheLifeTime;
    }

    void resetState(Payload & payload) {
        callMethod(&Device::resetState, payload);
        payload.m_expiresAfter = c_reportCacheLifeTime;
    }

    static const std::unordered_map<std::string, void (*)(Payload &)> s_handlers {
        { "get/kkm/base-status", baseStatus },
        { "get/kkm/status", status },
        { "get/kkm/full-status", fullStatus },
        { "post/kkm/learn", learn },
        { "post/kkm/reset-registry", resetRegistry },
        { "post/kkm/print-demo", printDemo },
        { "post/kkm/print-non-fiscal-doc", printNonFiscalDocument },
        { "post/kkm/print-info", printInfo },
        { "post/kkm/print-fn-registrations", printFnRegistrations },
        { "post/kkm/print-ofd-exchange-status", printOfdExchangeStatus },
        { "post/kkm/print-ofd-test", printOfdTest },
        { "post/kkm/print-close-shift-reports", printCloseShiftReports },
        { "post/kkm/print-last-document", printLastDocument },
        { "get/kkm/cash-stat", cashStat },
        { "post/kkm/cash-in", cashIn },
        { "post/kkm/cash-out", cashOut },
        { "post/kkm/sell", sell },
        { "post/kkm/sell-return", sellReturn },
        { "post/kkm/report-x", reportX },
        { "post/kkm/report-z", closeShift },
        { "post/kkm/close-shift", closeShift },
        { "post/kkm/reset-state", resetState }
    };

    bool Handler::asyncReady() const noexcept {
        return true;
    }

    void Handler::operator()(Http::Request & request) const noexcept try {
        assert(request.m_response.m_status == Http::Status::Ok);
        // if (request.m_response.m_status != Status::Ok) {
        //     return;
        // }

        std::string idempotencyKey {};

        {
            auto it = request.m_header.find("x-idempotency-key");
            if (it != request.m_header.end()) {
                idempotencyKey.assign(it->second);
            }
        }

        if (request.m_method == Http::Method::Post) {
            if (idempotencyKey.empty()) {
                return fail(request, Http::Status::BadRequest, Server::Mbs::c_invalidXIdempotencyKey);
            }
            auto it = request.m_header.find("content-type");
            if (it != request.m_header.end()) {
                bool typeOk { false };
                bool charsetOk { true };
                std::vector<std::string> chunks;
                Text::splitTo(chunks, it->second, " ;");
                for (auto & chunk: chunks) {
                    Text::trim(chunk);
                    if (chunk == "application/json") {
                        typeOk = true;
                    } else {
                        std::string subHeader, subValue;
                        Text::splitVariable(chunk, subHeader, subValue, true, true);
                        if (subHeader == "charset" && subValue != "utf-8" && subValue != "utf8") {
                            charsetOk = false;
                        }
                    }
                }
                if (!typeOk || !charsetOk) {
                    return fail(request, Http::Status::BadRequest, Server::Mbs::c_invalidContentType);
                }
            }
        } else if (request.m_method != Http::Method::Get) {
            return fail(request, Http::Status::MethodNotAllowed, Server::Mbs::c_methodNotAllowed);
        }

        Cache::maintain();
        Cache::Key cacheKey;

        if (!idempotencyKey.empty()) {
            cacheKey.assign("kkm::::");
            cacheKey.append(request.m_remote.to_string());
            cacheKey.append("::::");
            cacheKey.append(idempotencyKey);
        }

        if (!cacheKey.empty()) {
            auto cacheEntry = Cache::load(cacheKey);
            if (cacheEntry) {
                request.m_response.m_status = cacheEntry->m_status;
                request.m_response.m_data = cacheEntry->m_data;
                LOG_DEBUG_TS(Cache::Wcs::c_fromCache, request.m_id);
                return;
            }
        }

        std::string handlerKey, serialNumber;

        if (request.m_hint.size() == 4) {
            Text::concatTo(handlerKey, request.m_hint[0], "/", request.m_hint[1], "/", request.m_hint[3]);
            serialNumber.assign(request.m_hint[2]);
        } else if (request.m_hint.size() == 3) {
            Text::joinTo(handlerKey, request.m_hint, "/");
        } else {
            return fail(request, Http::Status::NotFound, Server::Mbs::c_notFound);
        }

        auto handler = s_handlers.find(handlerKey);
        if (handler == s_handlers.end()) {
            return fail(request, Http::Status::NotFound, Server::Mbs::c_notFound);
        }

        Nln::Json details(Nln::EmptyJsonObject);
        assert(details.is_object());
        if (request.m_method == Http::Method::Post && !request.m_body.empty()) {
            details = Nln::Json::parse(request.m_body);
            if (!details.is_object()) {
                return fail(request, Http::Status::BadRequest, Server::Mbs::c_badRequest);
            }
        }

        Payload payload {
            std::move(serialNumber), std::move(details), request.m_id,
            request.m_method == Http::Method::Get ? c_reportCacheLifeTime : c_receiptCacheLifeTime
        };
        (handler->second)(payload);
        assert(payload.m_result.is_object());

        auto response = std::make_shared<Http::JsonResponse>(std::move(payload.m_result));
        if (!cacheKey.empty()) {
            Cache::store(cacheKey, Cache::expiresAfter(payload.m_expiresAfter), payload.m_status, response);
        }

        assert(request.m_response.m_status == Http::Status::Ok);
        if (request.m_response.m_status == Http::Status::Ok) {
            request.m_response.m_status = payload.m_status;
            request.m_response.m_data = std::move(response);
        }

    } catch (const Basic::Failure & e) {
        fail(request, Http::Status::InternalServerError, Text::convert(e.what()), e.where());
    } catch (const std::exception & e) {
        fail(request, Http::Status::InternalServerError, e.what());
    } catch (...) {
        fail(request, Http::Status::InternalServerError, Basic::Mbs::c_somethingWrong);
    }
}
