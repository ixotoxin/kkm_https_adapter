// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "types.h"
#include <string>
#include <vector>
#include <ctime>

namespace Kkm {
    struct PrintableText {
        std::wstring m_content {};
        int m_marginOuter { 0 };
        int m_marginInner { 1 };
        bool m_center { false };
        bool m_magnified { false };
        bool m_separated { false };

        PrintableText() = default;
        PrintableText(const PrintableText &) = default;
        PrintableText(PrintableText &&) noexcept = default;
        [[maybe_unused]] PrintableText(std::wstring &&, bool, bool, bool, bool);
        ~PrintableText() = default;

        PrintableText & operator=(const PrintableText &) = default;
        PrintableText & operator=(PrintableText &&) noexcept = default;

        [[maybe_unused]]
        explicit operator bool() const {
            return !m_content.empty();
        }
    };

    struct Result {
        std::wstring m_message { Basic::Wcs::c_ok };
        bool m_success { true };
    };

    struct StatusResult : Result {
        std::tm m_dateTime {};
        std::wstring m_modelName {};
        std::wstring m_serialNumber {};
        double m_receiptSum {};
        unsigned int m_documentNumber {};
        DocumentType m_documentType {};
        unsigned int m_logicalNumber {};
        unsigned int m_mode {};
        unsigned int m_model {};
        unsigned int m_operatorId {};
        unsigned int m_receiptLineLength {};
        unsigned int m_receiptLineLengthPix {};
        unsigned int m_receiptNumber {};
        ReceiptType m_receiptType {};
        unsigned int m_shiftNumber {};
        ShiftState m_shiftState {};
        unsigned int m_subMode {};
        bool m_blocked {};
        bool m_cashDrawerOpened {};
        bool m_coverOpened {};
        bool m_cutError {};
        bool m_fiscal {};
        bool m_fnFiscal {};
        bool m_fnPresent {};
        bool m_invalidFn {};
        bool m_operatorRegistered {};
        bool m_paperNearEnd {};
        bool m_printerConnectionLost {};
        bool m_printerError {};
        bool m_printerOverheat {};
        bool m_receiptPaperPresent {};
    };

    struct ShiftStateResult : Result {
        std::tm m_expirationDateTime {};
        ShiftState m_shiftState {};
        unsigned int m_documentsCount {};
        unsigned int m_receiptNumber {};
        unsigned int m_shiftNumber {};
    };

    struct ReceiptStateResult : Result {
        double m_change {};
        double m_remainder {};
        double m_sum {};
        unsigned int m_documentNumber {};
        unsigned int m_receiptNumber {};
        ReceiptType m_receiptType {};
    };

    struct CashStatResult : Result {
        double m_cashSum {};
        double m_cashInSum {};
        double m_cashOutSum {};
        double m_sellCashSum {};
        double m_sellReturnCashSum {};
        unsigned int m_cashInCount {};
        unsigned int m_cashOutCount {};
    };

    struct FndtOfdExchangeStatusResult : Result {
        std::tm m_firstUnsentDateTime {};
        std::tm m_okpDateTime {};
        std::tm m_lastSentDateTime {};
        unsigned int m_exchangeStatus {};
        unsigned int m_firstUnsentNumber {};
        unsigned int m_unsentCount {};
        bool m_ofdMessageRead {};
    };

    struct FndtFnInfoResult : Result {
        std::wstring m_execution {};
        std::wstring m_keysUpdaterServerUri {};
        std::wstring m_serial {};
        std::wstring m_version {};
        unsigned int m_flags {};
        unsigned int m_state {};
        unsigned int m_type {};
        bool m_criticalError {};
        bool m_exhausted {};
        bool m_memoryOverflow {};
        bool m_needReplacement {};
        bool m_ofdTimeout {};
    };

    struct FndtRegistrationInfoResult : Result {
        std::wstring m_fnsUrl {};
        std::wstring m_organizationAddress {};
        std::wstring m_organizationVATIN {};
        std::wstring m_organizationName {};
        std::wstring m_organizationEmail {};
        std::wstring m_paymentsAddress {};
        std::wstring m_registrationNumber {};
        std::wstring m_machineNumber {};
        std::wstring m_ofdVATIN {};
        std::wstring m_ofdName {};
        unsigned int m_taxationTypes {};
        unsigned int m_agentSign {};
        FfdVersion m_ffdVersion { FfdVersion::Unknown };
        bool m_autoModeSign {};
        bool m_offlineModeSign {};
        bool m_encryptionSign {};
        bool m_internetSign {};
        bool m_serviceSign {};
        bool m_bsoSign {};
        bool m_lotterySign {};
        bool m_gamblingSign {};
        bool m_exciseSign {};
        bool m_machineInstallationSign {};
        bool m_tradeMarkedProducts {};
        bool m_insuranceActivity {};
        bool m_pawnShopActivity {};
        bool m_vending {};
        bool m_catering {};
        bool m_wholesale {};
    };

    struct FndtLastRegistrationResult : Result {
        std::tm m_registrationDateTime {};
        unsigned int m_documentNumber {};
        unsigned int m_registrationsCount {};
    };

    struct FndtLastReceiptResult : Result {
        std::tm m_documentDateTime {};
        std::wstring m_fiscalSign {};
        double m_receiptSum {};
        unsigned int m_documentNumber {};
    };

    struct FndtLastDocumentResult : Result {
        std::tm m_documentDateTime {};
        std::wstring m_fiscalSign {};
        unsigned int m_documentNumber {};
    };

    struct FndtErrorsResult : Result {
        std::tm m_successDateTime {};
        std::wstring m_fnErrorText {};
        std::wstring m_networkErrorText {};
        std::wstring m_ofdErrorText {};
        unsigned int m_commandCode {};
        unsigned int m_documentNumber {};
        unsigned int m_fnError {};
        unsigned int m_networkError {};
        unsigned int m_ofdError {};
        bool m_dataForSendIsEmpty {};
    };

    struct FfdVersionResult : Result {
        FfdVersion m_deviceFfdVersion { FfdVersion::Unknown };
        FfdVersion m_devMaxFfdVersion { FfdVersion::Unknown };
        FfdVersion m_devMinFfdVersion { FfdVersion::Unknown };
        FfdVersion m_fnFfdVersion { FfdVersion::Unknown };
        FfdVersion m_fnMaxFfdVersion { FfdVersion::Unknown };
        FfdVersion m_ffdVersion { FfdVersion::Unknown };
    };

    struct FwVersionResult : Result {
        std::wstring m_bootVersion {};
        std::wstring m_configurationVersion {};
        std::wstring m_controlUnitVersion {};
        std::wstring m_firmwareVersion {};
        std::wstring m_releaseVersion {};
        std::wstring m_templatesVersion {};
    };

    struct Details {
        bool m_electronically { false };
    };

    struct PrintDetails : Details {
        std::vector<PrintableText> m_document {};
        bool m_cliche { false };
        bool m_footer { false };
    };

    struct OperatorDetails : Details {
        std::wstring m_operatorName {};
        std::wstring m_operatorInn {};

        OperatorDetails() = default;
        OperatorDetails(const OperatorDetails &) = default;
        OperatorDetails(OperatorDetails &&) noexcept = default;
        [[maybe_unused]] OperatorDetails(std::wstring_view, std::wstring_view);
        ~OperatorDetails() = default;

        OperatorDetails & operator=(const OperatorDetails &) = default;
        OperatorDetails & operator=(OperatorDetails &&) = default;
    };

    struct CashDetails : OperatorDetails {
        double m_cashSum {};
    };

    struct ReceiptItemDetails {
        std::wstring m_commodityName;
        double m_price;
        double m_quantity;
        MeasurementUnit m_unit;
        Tax m_tax;

        ReceiptItemDetails() = delete;
        ReceiptItemDetails(const ReceiptItemDetails &) = default;
        ReceiptItemDetails(ReceiptItemDetails &&) noexcept = default;
        [[maybe_unused]] ReceiptItemDetails(std::wstring_view, double, double, MeasurementUnit, Tax);
        [[maybe_unused]] ReceiptItemDetails(std::wstring &&, double, double, MeasurementUnit, Tax);
        ~ReceiptItemDetails() = default;

        ReceiptItemDetails & operator=(const ReceiptItemDetails &) = default;
        ReceiptItemDetails & operator=(ReceiptItemDetails &&) noexcept = default;
    };

    struct ReceiptDetails : OperatorDetails {
        PrintableText m_text {};
        PrintableText m_headerText {};
        PrintableText m_footerText {};
        std::wstring m_sellerEmail {};
        std::wstring m_customerAccount {};
        std::wstring m_customerContact {}; // Телефон или электронный адрес покупателя
        std::wstring m_customerName {}; // Покупатель (клиент)
        std::wstring m_customerInn {}; // ИНН покупателя (клиента)
        std::wstring m_customerBirthdate {}; // Дата рождения покупателя (клиента) [строка формата "ДД.ММ.ГГГГ"]
        std::wstring m_customerCitizenship {}; // Гражданство [строка формата "ЦЦЦ"]
        std::wstring m_customerDocumentCode {}; // Код вида документа, удостоверяющего личность [строка формата "ЦЦ"]
        std::wstring m_customerDocumentData {}; // Данные документа, удостоверяющего личность
        std::wstring m_customerAddress {}; // Адрес покупателя (клиента)
        std::wstring m_electroPaymentId {};
        std::wstring m_electroPaymentAddInfo {};
        std::vector<ReceiptItemDetails> m_items;
        double m_paymentSum { 0 };
        PaymentType m_paymentType { PaymentType::Cash };
        int m_electroPaymentMethod { 0 };
        bool m_customerDataIsPresent { false };
        bool m_sellerDataIsPresent { false };
        bool m_electroPaymentInfo { false };
    };

    struct CloseDetails : OperatorDetails {
        bool m_closeShift { false };
        bool m_cashOut { false };

        CloseDetails() = default;
        CloseDetails(const CloseDetails &) = default;
        CloseDetails(CloseDetails &&) noexcept = default;
        [[maybe_unused]] CloseDetails(std::wstring_view, std::wstring_view, bool, bool);
        ~CloseDetails() = default;

        CloseDetails & operator=(const CloseDetails &) = default;
        CloseDetails & operator=(CloseDetails &&) = default;
    };
}
