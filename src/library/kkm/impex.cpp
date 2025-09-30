// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "impex.h"
#include "defaults.h"
#include "variables.h"
#include "strings.h"

namespace Kkm {
    bool assign(Nln::Json & json, const Result & result) {
        bool overrideMessage { false };
        if (
            !json.contains(Json::Mbs::c_successKey)
            || !json[Json::Mbs::c_successKey].is_boolean()
            || (json[Json::Mbs::c_successKey].get<bool>() && !result.m_success)
        ) {
            json[Json::Mbs::c_successKey] = result.m_success;
            overrideMessage = true;
        }
        if (
            overrideMessage
            || !json.contains(Json::Mbs::c_messageKey)
            || !json[Json::Mbs::c_messageKey].is_string()
            || json[Json::Mbs::c_messageKey].empty()
            || json[Json::Mbs::c_messageKey].get<std::string>() == Basic::Mbs::c_ok
        ) {
            json[Json::Mbs::c_messageKey] = Text::convert(result.m_message);
        }
        return json[Json::Mbs::c_successKey].get<bool>();
    }

    bool assign(Nln::Json & json, const StatusResult & result) {
        const Result & base { result };
        if (assign(json, base) && result.m_success) {
            json["status"] = {
                { "blocked", result.m_blocked },
                { "cashDrawerOpened", result.m_cashDrawerOpened },
                { "coverOpened", result.m_coverOpened },
                { "cutError", result.m_cutError },
                { "dateTime", DateTime::cast<std::string>(result.m_dateTime) },
                { "documentNumber", result.m_documentNumber },
                { "documentType", result.m_documentType },
                { "documentTypeText", safeGet(Mbs::c_documentTypeLabels, result.m_documentType) },
                { "fiscal", result.m_fiscal },
                { "fnFiscal", result.m_fnFiscal },
                { "fnPresent", result.m_fnPresent },
                { "invalidFn", result.m_invalidFn },
                { "logicalNumber", result.m_logicalNumber },
                { "mode", result.m_mode },
                { "model", result.m_model },
                { "modelName", Text::convert(result.m_modelName) },
                { "modelText", safeGet(Mbs::c_models, result.m_model) },
                { "operatorId", result.m_operatorId },
                { "operatorRegistered", result.m_operatorRegistered },
                { "paperNearEnd", result.m_paperNearEnd },
                { "printerConnectionLost", result.m_printerConnectionLost },
                { "printerError", result.m_printerError },
                { "printerOverheat", result.m_printerOverheat },
                { "receiptLineLength", result.m_receiptLineLength },
                { "receiptLineLengthPix", result.m_receiptLineLengthPix },
                { "receiptNumber", result.m_receiptNumber },
                { "receiptPaperPresent", result.m_receiptPaperPresent },
                { "receiptSum", result.m_receiptSum },
                { "receiptType", result.m_receiptType },
                { "receiptTypeText", safeGet(Mbs::c_receiptTypeLabels, result.m_receiptType) },
                { "serialNumber", Text::convert(result.m_serialNumber) },
                { "shiftNumber", result.m_shiftNumber },
                { "shiftState", result.m_shiftState },
                { "shiftStateText", safeGet(Mbs::c_shiftStateLabels, result.m_shiftState) },
                { "subMode", result.m_subMode }
            };
            return true;
        }
        return false;
    }

    bool assign(Nln::Json & json, const ShiftStateResult & result) {
        const Result & base { result };
        if (assign(json, base) && result.m_success) {
            json["shiftState"] = {
                { "documentsCount", result.m_documentsCount },
                { "receiptNumber", result.m_receiptNumber },
                { "expiredAt", DateTime::cast<std::string>(result.m_expirationDateTime) },
                { "shiftNumber", result.m_shiftNumber },
                { "shiftState", result.m_shiftState },
                { "shiftStateText", safeGet(Mbs::c_shiftStateLabels, result.m_shiftState) }
            };
            return true;
        }
        return false;
    }

    bool assign(Nln::Json & json, const ReceiptStateResult & result) {
        const Result & base { result };
        if (assign(json, base) && result.m_success) {
            json["receiptState"] = {
                { "receiptType", result.m_receiptType },
                { "receiptTypeText", safeGet(Mbs::c_receiptTypeLabels, result.m_receiptType) },
                { "receiptNumber", result.m_receiptNumber },
                { "documentNumber", result.m_documentNumber },
                { "sum", result.m_sum },
                { "remainder", result.m_remainder },
                { "change", result.m_change }
            };
            return true;
        }
        return false;
    }

    bool assign(Nln::Json & json, const CashStatResult & result) {
        const Result & base { result };
        if (assign(json, base) && result.m_success) {
            json["cashStat"] = {
                { "sellCashSum", result.m_sellCashSum },
                { "sellReturnCashSum", result.m_sellReturnCashSum },
                { "cashSum", result.m_cashSum },
                { "cashInSum", result.m_cashInSum },
                { "cashOutSum", result.m_cashOutSum },
                { "cashInCount", result.m_cashInCount },
                { "cashOutCount", result.m_cashOutCount }
            };
            return true;
        }
        return false;
    }

    bool assign(Nln::Json & json, const FndtOfdExchangeStatusResult & result) {
        const Result & base { result };
        if (assign(json, base) && result.m_success) {
            json["ofdExchangeStatus"] = {
                { "exchangeStatus", result.m_exchangeStatus },
                { "unsentCount", result.m_unsentCount },
                { "firstUnsentNumber", result.m_firstUnsentNumber },
                { "ofdMessageRead", result.m_ofdMessageRead },
                { "firstUnsentDateTime", DateTime::cast<std::string>(result.m_firstUnsentDateTime) },
                { "okpDateTime", DateTime::cast<std::string>(result.m_okpDateTime) },
                { "lastSentDateTime", DateTime::cast<std::string>(result.m_lastSentDateTime) }
            };
            return true;
        }
        return false;
    }

    bool assign(Nln::Json & json, const FndtFnInfoResult & result) {
        const Result & base { result };
        if (assign(json, base) && result.m_success) {
            json["fnInfo"] = {
                { "serial", Text::convert(result.m_serial) },
                { "version", Text::convert(result.m_version) },
                { "execution", Text::convert(result.m_execution) },
                { "type", result.m_type },
                { "state", result.m_state },
                { "flags", result.m_flags },
                { "needReplacement", result.m_needReplacement },
                { "exhausted", result.m_exhausted },
                { "memoryOverflow", result.m_memoryOverflow },
                { "ofdTimeout", result.m_ofdTimeout },
                { "criticalError", result.m_criticalError },
                { "keysUpdaterServerUri", Text::convert(result.m_keysUpdaterServerUri) }
            };
            return true;
        }
        return false;
    }

    bool assign(Nln::Json & json, const FndtRegistrationInfoResult & result) {
        const Result & base { result };
        if (assign(json, base) && result.m_success) {
            json["registrationInfo"] = {
                { "fnsUrl", Text::convert(result.m_fnsUrl) },
                { "organizationAddress", Text::convert(result.m_organizationAddress) },
                { "organizationVATIN", Text::convert(result.m_organizationVATIN) },
                { "organizationName", Text::convert(result.m_organizationName) },
                { "organizationEmail", Text::convert(result.m_organizationEmail) },
                { "paymentsAddress", Text::convert(result.m_paymentsAddress) },
                { "registrationNumber", Text::convert(result.m_registrationNumber) },
                { "machineNumber", Text::convert(result.m_machineNumber) },
                { "ofdVATIN", Text::convert(result.m_ofdVATIN) },
                { "ofdName", Text::convert(result.m_ofdName) },
                {
                    "taxationTypes",
                    {
                        { "osn", static_cast<bool>(result.m_taxationTypes & Atol::LIBFPTR_TT_OSN) },
                        { "usnIncome", static_cast<bool>(result.m_taxationTypes & Atol::LIBFPTR_TT_USN_INCOME) },
                        { "usnIncomeOutcome", static_cast<bool>(result.m_taxationTypes & Atol::LIBFPTR_TT_USN_INCOME_OUTCOME) },
                        { "esn", static_cast<bool>(result.m_taxationTypes & Atol::LIBFPTR_TT_ESN) },
                        { "patent", static_cast<bool>(result.m_taxationTypes & Atol::LIBFPTR_TT_PATENT) },
                    }
                },
                {
                    "agentSign",
                    {
                        { "bankPayingAgent", static_cast<bool>(result.m_agentSign & Atol::LIBFPTR_AT_BANK_PAYING_AGENT) },
                        { "bankPayingSubagent", static_cast<bool>(result.m_agentSign & Atol::LIBFPTR_AT_BANK_PAYING_SUBAGENT) },
                        { "payingAgent", static_cast<bool>(result.m_agentSign & Atol::LIBFPTR_AT_PAYING_AGENT) },
                        { "payingSubagent", static_cast<bool>(result.m_agentSign & Atol::LIBFPTR_AT_PAYING_SUBAGENT) },
                        { "attorney", static_cast<bool>(result.m_agentSign & Atol::LIBFPTR_AT_ATTORNEY) },
                        { "commissionAgent", static_cast<bool>(result.m_agentSign & Atol::LIBFPTR_AT_COMMISSION_AGENT) },
                        { "another", static_cast<bool>(result.m_agentSign & Atol::LIBFPTR_AT_ANOTHER) }
                    }
                },
                { "ffdVersion", safeGet(Mbs::c_ffdVersions, result.m_ffdVersion) },
                { "autoModeSign", result.m_autoModeSign },
                { "offlineModeSign", result.m_offlineModeSign },
                { "encryptionSign", result.m_encryptionSign },
                { "internetSign", result.m_internetSign },
                { "serviceSign", result.m_serviceSign },
                { "bsoSign", result.m_bsoSign },
                { "lotterySign", result.m_lotterySign },
                { "gamblingSign", result.m_gamblingSign },
                { "exciseSign", result.m_exciseSign },
                { "machineInstallationSign", result.m_machineInstallationSign },
                { "tradeMarkedProducts", result.m_tradeMarkedProducts },
                { "insuranceActivity", result.m_insuranceActivity },
                { "pawnShopActivity", result.m_pawnShopActivity },
                { "vending", result.m_vending },
                { "catering", result.m_catering },
                { "wholesale", result.m_wholesale }
            };
            return true;
        }
        return false;
    }

    bool assign(Nln::Json & json, const FndtLastRegistrationResult & result) {
        const Result & base { result };
        if (assign(json, base) && result.m_success) {
            json["lastRegistration"] = {
                { "documentNumber", result.m_documentNumber },
                { "registrationsCount", result.m_registrationsCount },
                { "registrationDateTime", DateTime::cast<std::string>(result.m_registrationDateTime) }
            };
            return true;
        }
        return false;
    }

    bool assign(Nln::Json & json, const FndtLastReceiptResult & result) {
        const Result & base { result };
        if (assign(json, base) && result.m_success) {
            json["lastReceipt"] = {
                { "documentNumber", result.m_documentNumber },
                { "receiptSum", result.m_receiptSum },
                { "fiscalSign", Text::convert(result.m_fiscalSign) },
                { "documentDateTime", DateTime::cast<std::string>(result.m_documentDateTime) }
            };
            return true;
        }
        return false;
    }

    bool assign(Nln::Json & json, const FndtLastDocumentResult & result) {
        const Result & base { result };
        if (assign(json, base) && result.m_success) {
            json["lastDocument"] = {
                { "documentNumber", result.m_documentNumber },
                { "fiscalSign", Text::convert(result.m_fiscalSign) },
                { "documentDateTime", DateTime::cast<std::string>(result.m_documentDateTime) }
            };
            return true;
        }
        return false;
    }

    bool assign(Nln::Json & json, const FndtErrorsResult & result) {
        const Result & base { result };
        if (assign(json, base) && result.m_success) {
            json["fndtErrors"] = {
                { "networkError", result.m_networkError },
                { "networkErrorText", Text::convert(result.m_networkErrorText) },
                { "ofdError", result.m_ofdError },
                { "ofdErrorText", Text::convert(result.m_ofdErrorText) },
                { "fnError", result.m_fnError },
                { "fnErrorText", Text::convert(result.m_fnErrorText) },
                { "failedDocumentNumber", result.m_documentNumber },
                { "failedCommandCode", result.m_commandCode },
                { "successDateTime", DateTime::cast<std::string>(result.m_successDateTime) },
                { "dataForSendIsEmpty", result.m_dataForSendIsEmpty }
            };
            return true;
        }
        return false;
    }

    bool assign(Nln::Json & json, const FfdVersionResult & result) {
        const Result & base { result };
        if (assign(json, base) && result.m_success) {
            json["ffdVersions"] = {
                { "deviceFfd", safeGet(Mbs::c_ffdVersions, result.m_deviceFfdVersion) },
                { "deviceMinFfd", safeGet(Mbs::c_ffdVersions, result.m_devMinFfdVersion) },
                { "deviceMaxFfd", safeGet(Mbs::c_ffdVersions, result.m_devMaxFfdVersion) },
                { "fnFfd", safeGet(Mbs::c_ffdVersions, result.m_fnFfdVersion) },
                { "fnMaxFfd", safeGet(Mbs::c_ffdVersions, result.m_fnMaxFfdVersion) },
                { "ffd", safeGet(Mbs::c_ffdVersions, result.m_ffdVersion) }
            };
            return true;
        }
        return false;
    }

    bool assign(Nln::Json & json, const FwVersionResult & result) {
        const Result & base { result };
        if (assign(json, base) && result.m_success) {
            json["fwVersions"] = {
                { "boot", Text::convert(result.m_bootVersion) },
                { "configuration", Text::convert(result.m_configurationVersion) },
                { "controlUnit", Text::convert(result.m_controlUnitVersion) },
                { "firmware", Text::convert(result.m_firmwareVersion) },
                { "release", Text::convert(result.m_releaseVersion) },
                { "templates", Text::convert(result.m_templatesVersion) }
            };
            return true;
        }
        return false;
    }

    void assign(Details & details, const Nln::Json & json) {
        Json::handleKey(json, "electronically", details.m_electronically);
    }

    void assign(PrintDetails & details, const Nln::Json & json) {
        Details & base { details };
        assign(base, json);
        bool margin { false };
        Json::handleKey(json, "cliche", details.m_cliche);
        Json::handleKey(json, "footer", details.m_footer);
        Json::handleKey(json, "margin", margin);
        if (json.contains("document") && json["document"].is_array()) {
            details.m_document.reserve(json["document"].size());
            int i { 0 };
            for (auto & block : json["document"].get<std::vector<Nln::Json>>()) {
                bool center { false };
                bool magnified { false };
                bool separated { false };
                bool separator { false };
                std::wstring content {};
                auto basePath { std::format(L"document[{}]", i++) };
                Json::handleKey(block, "separator", separator, basePath);
                if (separator) {
                    separated = true;
                } else {
                    bool found = Json::handleKey(block, "content", content, basePath);
                    if (!found) { // TODO: Добавить проверку длинны текста.
                        throw Failure(KKM_WFMT(Wcs::c_requiresProperty2, basePath, L"content")); // NOLINT(*-exception-baseclass)
                    }
                    Json::handleKey(block, "center", center, basePath);
                    Json::handleKey(block, "magnified", magnified, basePath);
                    Json::handleKey(block, "separated", separated, basePath);
                }
                auto actualMargin = margin && !content.empty();
                details.m_document.emplace_back(std::move(content), center, magnified, separated, actualMargin);
            }
        } else {
            throw Failure(KKM_WFMT(Wcs::c_requiresProperty, L"document")); // NOLINT(*-exception-baseclass)
        }
    }

    void assign(OperatorDetails & details, const Nln::Json & json) {
        Details & base { details };
        assign(base, json);
        bool result = Json::handleKey(
            json, "operator",
            [& details] (const Nln::Json & json, const std::wstring & path) -> bool {
                auto found
                    = Json::handleKey(
                        json, "name", details.m_operatorName,
                        Text::Wcs::length(1, 64, Text::Wcs::trim()), path
                    );
                if (!found) {
                    throw Failure(KKM_WFMT(Wcs::c_requiresProperty2, path, L"name")); // NOLINT(*-exception-baseclass)
                }
                Json::handleKey(json, "inn", details.m_operatorInn, Text::Wcs::maxLength(12), path);
                return true;
            }
        );
        if (!result) {
            throw Failure(KKM_WFMT(Wcs::c_requiresProperty, L"operator")); // NOLINT(*-exception-baseclass)
        }
    }

    void assign(CashDetails & details, const Nln::Json & json) {
        OperatorDetails & base { details };
        assign(base, json);
        bool found
            = Json::handleKey(
                json, "cashSum", details.m_cashSum,
                Numeric::between(c_minCashInOut, s_maxCashInOut)
            );
        if (!found) {
            throw Failure(KKM_WFMT(Wcs::c_requiresProperty, L"cashSum")); // NOLINT(*-exception-baseclass)
        }
    }

    void assign(ReceiptDetails & details, const Nln::Json & json) {
        OperatorDetails & base { details };
        assign(base, json);
        Json::handleKey(
            json, "customer",
            [& details] (const Nln::Json & json, const std::wstring & path) -> bool {
                Json::handleKey(json, "account", details.m_customerAccount, Text::Wcs::maxLength(32), path);
                Json::handleKey(json, "contact", details.m_customerContact, Text::Wcs::maxLength(64), path);
                Json::handleKey(json, "name", details.m_customerName, Text::Wcs::maxLength(256), path);
                Json::handleKey(json, "inn", details.m_customerInn, Text::Wcs::maxLength(12), path);
                Json::handleKey(json, "birthdate", details.m_customerBirthdate, Text::Wcs::maxLength(10), path);
                Json::handleKey(json, "citizenship", details.m_customerCitizenship, Text::Wcs::maxLength(3), path);
                Json::handleKey(json, "documentCode", details.m_customerDocumentCode, Text::Wcs::maxLength(32), path);
                Json::handleKey(json, "documentData", details.m_customerDocumentData, Text::Wcs::maxLength(64), path);
                Json::handleKey(json, "address", details.m_customerAddress, Text::Wcs::maxLength(256), path);
                details.m_customerDataIsPresent
                    = !details.m_customerAccount.empty() || !details.m_customerContact.empty()
                      || !details.m_customerName.empty() || !details.m_customerInn.empty()
                      || !details.m_customerBirthdate.empty() || !details.m_customerCitizenship.empty()
                      || !details.m_customerDocumentCode.empty() || !details.m_customerDocumentData.empty()
                      || !details.m_customerAddress.empty();
                return true;
            }
        );
        Json::handleKey(
            json, "seller",
            [& details] (const Nln::Json & json, const std::wstring & path) -> bool {
                Json::handleKey(json, "email", details.m_sellerEmail, Text::Wcs::maxLength(64), path);
                details.m_sellerDataIsPresent = !details.m_sellerEmail.empty();
                return true;
            }
        );
        Json::handleKey(
            json, "text",
            [& details] (const Nln::Json & json, const std::wstring & path) -> bool {
                Json::handleKey(json, "content", details.m_text.m_content, path); // TODO: Добавить проверку длинны текста.
                Json::handleKey(json, "center", details.m_text.m_center, path);
                Json::handleKey(json, "magnified", details.m_text.m_magnified, path);
                Json::handleKey(json, "separated", details.m_text.m_separated, path);
                return true;
            }
        );
        Json::handleKey(
            json, "headerText",
            [& details] (const Nln::Json & json, const std::wstring & path) -> bool {
                Json::handleKey(json, "content", details.m_headerText.m_content, path); // TODO: Добавить проверку длинны текста.
                Json::handleKey(json, "center", details.m_headerText.m_center, path);
                Json::handleKey(json, "magnified", details.m_headerText.m_magnified, path);
                Json::handleKey(json, "separated", details.m_headerText.m_separated, path);
                return true;
            }
        );
        Json::handleKey(
            json, "footerText",
            [& details] (const Nln::Json & json, const std::wstring & path) -> bool {
                Json::handleKey(json, "content", details.m_footerText.m_content, path); // TODO: Добавить проверку длинны текста.
                Json::handleKey(json, "center", details.m_footerText.m_center, path);
                Json::handleKey(json, "magnified", details.m_footerText.m_magnified, path);
                Json::handleKey(json, "separated", details.m_footerText.m_separated, path);
                return true;
            }
        );
        Tax defaultTax { Tax::No };
        bool hasDefaultTax = Json::handleKey(json, "tax", defaultTax, Mbs::c_taxCastMap);
        if (json.contains("items") && json["items"].is_array()) {
            details.m_items.reserve(json["items"].size());
            int i { 0 };
            for (auto & item : json["items"].get<std::vector<Nln::Json>>()) {
                auto basePath { std::format(L"items[{}]", i++) };
                std::wstring title;
                bool found
                    = Json::handleKey(
                        item, "title", title,
                        Text::Wcs::length(1, 128, Text::Wcs::trim()), basePath
                    );
                if (!found) {
                    throw Failure(KKM_WFMT(Wcs::c_requiresProperty2, basePath, L"title")); // NOLINT(*-exception-baseclass)
                }
                double price;
                found
                    = Json::handleKey(
                        item, "price", price,
                        Numeric::between(c_minPrice, s_maxPrice), basePath
                    );
                if (!found) {
                    throw Failure(KKM_WFMT(Wcs::c_requiresProperty2, basePath, L"price")); // NOLINT(*-exception-baseclass)
                }
                double quantity;
                found
                    = Json::handleKey(
                        item, "quantity", quantity,
                        Numeric::between(c_minQuantity, s_maxQuantity), basePath
                    );
                if (!found) {
                    throw Failure(KKM_WFMT(Wcs::c_requiresProperty2, basePath, L"quantity")); // NOLINT(*-exception-baseclass)
                }
                MeasurementUnit unit { MeasurementUnit::Piece };
                Json::handleKey(item, "unit", unit, Mbs::c_measurementUnitMap);
                Tax tax;
                found = Json::handleKey(item, "tax", tax, Mbs::c_taxCastMap);
                if (!found) {
                    if (hasDefaultTax) {
                        tax = defaultTax;
                    } else {
                        throw Failure(KKM_WFMT(Wcs::c_requiresProperty2, basePath, L"tax")); // NOLINT(*-exception-baseclass)
                    }
                }
                details.m_paymentSum += price * quantity;
                details.m_items.emplace_back(std::move(title), price, quantity, unit, tax);
            }
        } else {
            throw Failure(KKM_WFMT(Wcs::c_requiresProperty, L"items")); // NOLINT(*-exception-baseclass)
        }
        bool paymentFound = Json::handleKey(
            json, "payment",
            [& details] (const Nln::Json & json, const std::wstring & path) -> bool {
                std::wstring sum {};
                if (Json::handleKey(json, "sum", sum, path)) {
                    try {
                        Text::lower(sum);
                        if (sum == L"auto") {
                            details.m_paymentSum = -1;
                        } else {
                            details.m_paymentSum = Text::cast<double>(sum);
                        }
                    } catch (...) {
                        throw Failure(KKM_WFMT(Wcs::c_requiresProperty2, path, L"sum")); // NOLINT(*-exception-baseclass)
                    }
                } else {
                    throw Failure(KKM_WFMT(Wcs::c_requiresProperty2, path, L"sum")); // NOLINT(*-exception-baseclass)
                }
                bool found = Json::handleKey(json, "type", details.m_paymentType, Mbs::c_paymentTypeCastMap, path);
                if (!found) {
                    throw Failure(KKM_WFMT(Wcs::c_requiresProperty2, path, L"type")); // NOLINT(*-exception-baseclass)
                }
                if (details.m_paymentType == PaymentType::Electronically) {
                    details.m_electroPaymentInfo
                        = Json::handleKey(
                            json, "electroPaymentInfo",
                            [& details] (const Nln::Json & json, const std::wstring & path) -> bool {
                                bool found = Json::handleKey(json, "method", details.m_electroPaymentMethod, path);
                                if (!found) {
                                    throw Failure(KKM_WFMT(Wcs::c_requiresProperty2, path, L"method")); // NOLINT(*-exception-baseclass)
                                }
                                found
                                    = Json::handleKey(
                                        json, "id", details.m_electroPaymentId,
                                        Text::Wcs::length(1, 256), path
                                    );
                                if (!found) {
                                    throw Failure(KKM_WFMT(Wcs::c_requiresProperty2, path, L"id")); // NOLINT(*-exception-baseclass)
                                }
                                Json::handleKey(
                                    json, "addInfo", details.m_electroPaymentAddInfo,
                                    Text::Wcs::maxLength(256), path
                                );
                                return true;
                            }
                        );
                }
                return true;
            }
        );
        if (!paymentFound) {
            throw Failure(KKM_WFMT(Wcs::c_requiresProperty, L"payment")); // NOLINT(*-exception-baseclass)
        }
    }

    void assign(CloseDetails & details, const Nln::Json & json) {
        OperatorDetails & base { details };
        assign(base, json);
        Json::handleKey(json, "closeShift", details.m_closeShift);
        Json::handleKey(json, "cashOut", details.m_cashOut);
    }
}
