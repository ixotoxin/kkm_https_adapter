// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "device.h"
#include <lib/json.h>
#include <concepts>

namespace Kkm {
    using OptionalResult = std::optional<Nln::Json>;

    bool assign(Nln::Json &, const Result &);
    bool assign(Nln::Json &, const StatusResult &);
    bool assign(Nln::Json &, const ShiftStateResult &);
    bool assign(Nln::Json &, const ReceiptStateResult &);
    bool assign(Nln::Json &, const CashStatResult &);
    bool assign(Nln::Json &, const FndtOfdExchangeStatusResult &);
    bool assign(Nln::Json &, const FndtFnInfoResult &);
    bool assign(Nln::Json &, const FndtRegistrationInfoResult &);
    bool assign(Nln::Json &, const FndtLastRegistrationResult &);
    bool assign(Nln::Json &, const FndtLastReceiptResult &);
    bool assign(Nln::Json &, const FndtLastDocumentResult &);
    bool assign(Nln::Json &, const FndtErrorsResult &);
    bool assign(Nln::Json &, const FfdVersionResult &);
    bool assign(Nln::Json &, const FwVersionResult &);
    void assign(Details &, const Nln::Json &);
    void assign(PrintDetails &, const Nln::Json &);
    void assign(OperatorDetails &, const Nln::Json &);
    void assign(CashDetails &, const Nln::Json &);
    void assign(ReceiptDetails &, const Nln::Json &);
    void assign(CloseDetails &, const Nln::Json &);

    Nln::Json & operator<<(Nln::Json & json, const std::derived_from<Result> auto & result) {
        assign(json, result);
        return json;
    }

    template<std::derived_from<Result> R>
    OptionalResult & operator<<(OptionalResult & optionalResult, const R & result) {
        if constexpr (std::is_same_v<R, Result>) {
            if (!result.m_success || !(result.m_message.empty() || result.m_message == Basic::Wcs::c_ok)) {
                if (!optionalResult.has_value()) {
                    optionalResult.emplace(Nln::EmptyJsonObject);
                }
                assign(optionalResult.value(), result);
             }
        } else {
            if (!optionalResult.has_value()) {
                optionalResult.emplace(Nln::EmptyJsonObject);
            }
            assign(optionalResult.value(), result);
        }
        return optionalResult;
    }

    const Nln::Json & operator>>(const Nln::Json & json, std::derived_from<Details> auto & details) {
        assign(details, json);
        return json;
    }
}
