// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "kkm_handler.h"
#include "messages.h"
#include "kkm.h"
#include "json_response.h"
#include "http_cache.h"

namespace Kkm {
    namespace Wcs {
        using namespace Http::Wcs;
    }

    namespace Mbs {
        using namespace Http::Mbs;
    }

    namespace Cache = Http::Cache;

    using TimeOffsetValue = DateTime::OffsetValue;
    using Http::Method;
    using Http::Status;
    using NewConnParams = Device::NewConnParams;
    using KnownConnParams = Device::KnownConnParams;
    using Call = Device::Call;

    static std::unordered_map<std::wstring, std::shared_ptr<KnownConnParams>> s_connParamsRegistry {};
    static std::mutex s_registryMutex {};

    struct Payload {
        using Id = decltype(Http::Request::m_id);

        const std::string m_serialNumber;
        const Nln::Json m_details;
        Nln::Json m_result;
        TimeOffsetValue m_expireIn;
        Status m_status { Status::Ok };
        const Id m_requestId;

        Payload() = delete;

        Payload(
            std::string && serialNumber,
            Nln::Json && details,
            const Id requestId,
            const TimeOffsetValue expireIn = 0
        ) : m_serialNumber(std::forward<std::string>(serialNumber)),
            m_details(std::forward<Nln::Json>(details)), m_result(Nln::EmptyJsonObject),
            m_expireIn(expireIn), m_requestId(requestId) {
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
            const Status status,
            const std::string_view message,
            const std::source_location & location = std::source_location::current()
        ) {
            assert(static_cast<int>(status) >= 400);
            m_status = status;
            std::string message2 { message };
            m_result["!message"] = message2;
            if (Log::s_appendLocation) {
                message2 += location;
            }
            tsLogWarning(Mbs::c_prefixedText, m_requestId, message2);
        }
    };

    [[maybe_unused]]
    inline std::shared_ptr<KnownConnParams> resolveConnParams(Payload & payload) {
        if (payload.m_serialNumber.empty()) {
            payload.fail(Status::BadRequest, Mbs::c_badRequest);
            return nullptr;
        }
        auto wcSerialNumber { Text::convert(payload.m_serialNumber) };
        std::scoped_lock registryLock(s_registryMutex);
        if (s_connParamsRegistry.contains(wcSerialNumber)) {
            auto & params { s_connParamsRegistry.at(wcSerialNumber) };
            tsLogDebug(Wcs::c_selectKkm, payload.m_requestId, wcSerialNumber, static_cast<std::wstring>(*params));
            return params;
        } else {
            auto [it, insert]
                = s_connParamsRegistry.try_emplace(
                    wcSerialNumber,
                    std::make_shared<KnownConnParams>(wcSerialNumber.c_str())
                );
            if (insert) {
                tsLogDebug(Wcs::c_selectKkm, payload.m_requestId, wcSerialNumber, static_cast<std::wstring>(*(it->second)));
                return it->second;
            }
        }
        payload.fail(
            Status::NotFound,
            std::format(Mbs::c_notFound, payload.m_requestId, payload.m_serialNumber)
        );
        return nullptr;
    }

    template<std::derived_from<Call::Result> ResultType>
    using InfoMethod = void (Device::*)(ResultType &);

    template<class ResultType>
    [[maybe_unused]]
    inline void methodCall(
        InfoMethod<ResultType> method,
        Payload & payload
    ) {
        ResultType result;
        auto connParams { resolveConnParams(payload) };
        if (connParams) {
            Device kkm { *connParams, std::format(Wcs::c_requestPrefix, payload.m_requestId) };
            (kkm.*method)(result);
            result.exportTo(payload.m_result);
        }
    }

    template<std::derived_from<Call::Result> ResultType, std::derived_from<Call::Details> DetailsType>
    using ActionMethod = void (Device::*)(const DetailsType &, ResultType &);

    template<class ResultType, class DetailsType, typename ... AdditionalDetailsArgs>
    [[maybe_unused]]
    inline void methodCall(
        ActionMethod<ResultType, DetailsType> method,
        Payload & payload,
        AdditionalDetailsArgs ... args
    ) {
        if (payload.m_serialNumber.empty()) {
            return payload.fail(Status::BadRequest, Mbs::c_badRequest);
        }
        DetailsType details(payload.m_details, args...);
        Call::Result result;
        auto connParams { resolveConnParams(payload) };
        if (connParams) {
            Device kkm { *connParams, std::format(Wcs::c_requestPrefix, payload.m_requestId) };
            (kkm.*method)(details, result);
            result.exportTo(payload.m_result);
        }
    }

    template<std::derived_from<Call::Details> DetailsType, typename ... AdditionalDetailsArgs>
    [[maybe_unused]]
    inline void methodCall(
        ActionMethod<Call::Result, DetailsType> method,
        Payload & payload,
        AdditionalDetailsArgs ... args
    ) {
        methodCall<Call::Result, DetailsType, AdditionalDetailsArgs...>(method, payload, args...);
    }

    void learn(Payload & payload) {
        if (!payload.m_serialNumber.empty()) {
            return payload.fail(Status::BadRequest, Mbs::c_badRequest);
        }

        std::wstring connString;
        if (!Json::handleKey(payload.m_details, "connParams", connString)) {
            return payload.fail(Status::BadRequest, std::format(Mbs::c_requiresProperty, "connParams"));
        }

        std::wstring serialNumber;

        {
            NewConnParams connParams { connString };
            Device kkm { connParams, std::format(Wcs::c_requestPrefix, payload.m_requestId) };
            serialNumber.assign(kkm.serialNumber());
            tsLogDebug(Wcs::c_getKkmInfo, payload.m_requestId, serialNumber);

            {
                std::scoped_lock registryLock(s_registryMutex);
                s_connParamsRegistry.insert_or_assign(
                    serialNumber,
                    std::make_shared<KnownConnParams>(connParams, serialNumber.c_str())
                );
            }

            Call::StatusResult result;
            kkm.getStatus(result);
            kkm.printHello();
            connParams.save(serialNumber);
            result.exportTo(payload.m_result);
        }

        tsLogInfo(Wcs::c_connParamsSaved, payload.m_requestId, serialNumber);
    }

    void resetRegistry(Payload & payload) {
        std::scoped_lock registryLock(s_registryMutex);
        s_connParamsRegistry.clear();
        if (!s_connParamsRegistry.empty()) {
            payload.fail(Status::InternalServerError, Mbs::c_cantClearRegistry);
        }
        payload.m_expireIn = Http::c_reportCacheLifeTime;
    }

    void status(Payload & payload) {
        MEMORY_LEAK;
        methodCall<Call::StatusResult>(&Device::getStatus, payload);
        payload.m_expireIn = Http::c_reportCacheLifeTime;
    }

    void fullStatus(Payload & payload) {
        if (payload.m_serialNumber.empty()) {
            return payload.fail(Status::BadRequest, Mbs::c_badRequest);
        }

        auto connParams { resolveConnParams(payload) };
        if (connParams) {
            Device kkm { *connParams, std::format(Wcs::c_requestPrefix, payload.m_requestId) };

            {
                Call::StatusResult result;
                kkm.getStatus(result);
                result.exportTo(payload.m_result);
            }
            {
                Call::ShiftStateResult result;
                kkm.getShiftState(result);
                result.exportTo(payload.m_result);
            }
            {
                Call::ReceiptStateResult result;
                kkm.getReceiptState(result);
                result.exportTo(payload.m_result);
            }
            {
                Call::CashStatResult result;
                kkm.getCashStat(result);
                result.exportTo(payload.m_result);
            }
            {
                Call::FndtOfdExchangeStatusResult result;
                kkm.getFndtOfdExchangeStatus(result);
                result.exportTo(payload.m_result);
            }
            {
                Call::FndtFnInfoResult result;
                kkm.getFndtFnInfo(result);
                result.exportTo(payload.m_result);
            }
            {
                Call::FndtLastRegistrationResult result;
                kkm.getFndtLastRegistration(result);
                result.exportTo(payload.m_result);
            }
            {
                Call::FndtLastReceiptResult result;
                kkm.getFndtLastReceipt(result);
                result.exportTo(payload.m_result);
            }
            {
                Call::FndtLastDocumentResult result;
                kkm.getFndtLastDocument(result);
                result.exportTo(payload.m_result);
            }
            {
                Call::FndtErrorsResult result;
                kkm.getFndtErrors(result);
                result.exportTo(payload.m_result);
            }
            {
                Call::VersionResult result;
                kkm.getVersion(result);
                result.exportTo(payload.m_result);
            }
        }
        payload.m_expireIn = Http::c_reportCacheLifeTime;
    }

    void printDemo(Payload & payload) {
        methodCall<Call::Result>(&Device::printDemo, payload);
        payload.m_expireIn = Http::c_reportCacheLifeTime;
    }

    void printNonFiscalDocument(Payload & payload) {
        methodCall<Call::PrintDetails>(&Device::printNonFiscalDocument, payload);
        payload.m_expireIn = Http::c_reportCacheLifeTime;
    }

    void printInfo(Payload & payload) {
        methodCall<Call::Result>(&Device::printInfo, payload);
        payload.m_expireIn = Http::c_reportCacheLifeTime;
    }

    void printFnRegistrations(Payload & payload) {
        methodCall<Call::Result>(&Device::printFnRegistrations, payload);
        payload.m_expireIn = Http::c_reportCacheLifeTime;
    }

    void printOfdExchangeStatus(Payload & payload) {
        methodCall<Call::Result>(&Device::printOfdExchangeStatus, payload);
        payload.m_expireIn = Http::c_reportCacheLifeTime;
    }

    void printOfdTest(Payload & payload) {
        methodCall<Call::Result>(&Device::printOfdTest, payload);
        payload.m_expireIn = Http::c_reportCacheLifeTime;
    }

    void printCloseShiftReports(Payload & payload) {
        methodCall<Call::Result>(&Device::printCloseShiftReports, payload);
        payload.m_expireIn = Http::c_reportCacheLifeTime;
    }

    void printLastDocument(Payload & payload) {
        methodCall<Call::Result>(&Device::printLastDocument, payload);
        payload.m_expireIn = Http::c_reportCacheLifeTime;
    }

    void cashStat(Payload & payload) {
        methodCall<Call::CashStatResult>(&Device::getCashStat, payload);
        payload.m_expireIn = Http::c_reportCacheLifeTime;
    }

    void cashIn(Payload & payload) {
        methodCall<Call::CashDetails>(&Device::registerCashIn, payload);
    }

    void cashOut(Payload & payload) {
        methodCall<Call::CashDetails>(&Device::registerCashOut, payload);
    }

    void sell(Payload & payload) {
        methodCall<Call::ReceiptDetails>(&Device::registerSell, payload);
    }

    void sellReturn(Payload & payload) {
        methodCall<Call::ReceiptDetails>(&Device::registerSellReturn, payload);
    }

    void closeShift(Payload & payload) {
        methodCall<Call::CloseDetails>(&Device::closeShift, payload);
        payload.m_expireIn = Http::c_reportCacheLifeTime;
    }

    void reportX(Payload & payload) {
        methodCall<Call::CloseDetails>(&Device::reportX, payload);
        payload.m_expireIn = Http::c_reportCacheLifeTime;
    }

    void resetState(Payload & payload) {
        methodCall<Call::CloseDetails>(&Device::resetState, payload);
        payload.m_expireIn = Http::c_reportCacheLifeTime;
    }

    static const std::unordered_map<std::string, void (*)(Payload &)> s_handlers {
        { "get/kkm/status", status },
        { "get/kkm/full-status", fullStatus },
        // { "get/kkm/status/full", fullStatus },
        { "post/kkm/learn", learn },
        { "post/kkm/reset-registry", resetRegistry },
        // { "post/kkm/registry/reset", resetRegistry },
        { "post/kkm/print-demo", printDemo },
        { "post/kkm/print-non-fiscal-doc", printNonFiscalDocument },
        { "post/kkm/print-info", printInfo },
        { "post/kkm/print-fn-registrations", printFnRegistrations },
        { "post/kkm/print-ofd-exchange-status", printOfdExchangeStatus },
        { "post/kkm/print-ofd-test", printOfdTest },
        { "post/kkm/print-close-shift-reports", printCloseShiftReports },
        { "post/kkm/print-last-document", printLastDocument },
        { "get/kkm/cash-stat", cashStat },
        // { "get/kkm/cash/stat", cashStat },
        { "post/kkm/cash-in", cashIn },
        // { "post/kkm/cash/in", cashIn },
        { "post/kkm/cash-out", cashOut },
        // { "post/kkm/cash/out", cashOut },
        { "post/kkm/sell", sell },
        { "post/kkm/sell-return", sellReturn },
        { "post/kkm/report-x", reportX },
        { "post/kkm/report-z", closeShift },
        { "post/kkm/close-shift", closeShift },
        { "post/kkm/reset-state", resetState }
    };

    bool HttpHandler::asyncReady() const noexcept {
        return true;
    }

    bool HttpHandler::isCaching() const noexcept {
        return true;
    }

    void HttpHandler::operator()(Http::Request & request) const noexcept try {
        assert(request.m_response.m_status == Status::Ok);
        if (request.m_response.m_status != Status::Ok) {
            return;
        }

        std::string idempotencyKey {};

        {
            auto it = request.m_header.find("x-idempotency-key");
            if (it != request.m_header.end()) {
                idempotencyKey.assign(it->second);
            }
        }

        if (request.m_method == Method::Post) {
            if (idempotencyKey.empty()) {
                return request.fail(Status::BadRequest, Mbs::c_invalidXIdempotencyKey);
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
                    return request.fail(Status::BadRequest, Mbs::c_invalidContentType);
                }
            }
        } else if (request.m_method != Method::Get) {
            return request.fail(Status::MethodNotAllowed, Mbs::c_badRequest);
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
                tsLogDebug(Wcs::c_fromCache, request.m_id);
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
            // return request.fail(Status::BadRequest, Mbs::c_badRequest);
            return request.fail(Status::MethodNotAllowed, Mbs::c_methodNotAllowed);
        }
        auto handler = s_handlers.find(handlerKey);
        if (handler == s_handlers.end()) {
            // return request.fail(Status::BadRequest, Mbs::c_unknownMethod);
            return request.fail(Status::MethodNotAllowed, Mbs::c_methodNotAllowed);
        }

        Nln::Json details(Nln::EmptyJsonObject);
        assert(details.is_object());
        if (request.m_method == Method::Post && !request.m_body.empty()) {
            details = Nln::Json::parse(request.m_body);
            if (!details.is_object()) {
                return request.fail(Status::BadRequest, Mbs::c_badRequest);
            }
        }

        Payload payload {
            std::move(serialNumber), std::move(details), request.m_id,
            request.m_method == Method::Get ? Http::c_reportCacheLifeTime : Http::c_receiptCacheLifeTime
        };
        (handler->second)(payload);
        assert(payload.m_result.is_object());

        auto response = std::make_shared<Http::JsonResponse>(std::move(payload.m_result));
        if (!cacheKey.empty()) {
            Cache::store(cacheKey, Cache::expiresAt(payload.m_expireIn), payload.m_status, response);
        }

        assert(request.m_response.m_status == Status::Ok);
        if (request.m_response.m_status == Status::Ok) {
            request.m_response.m_status = payload.m_status;
            request.m_response.m_data = response;
        }

    } catch (Failure & e) {
        request.fail(Status::InternalServerError, Text::convert(e.what()), e.where());
    } catch (std::exception & e) {
        request.fail(Status::InternalServerError, e.what());
    } catch (...) {
        request.fail(Status::InternalServerError, Basic::Mbs::c_somethingWrong);
    }
}
