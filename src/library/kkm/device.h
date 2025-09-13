// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "types.h"
#include <lib/json.h>
#include <vector>
#include <ctime>

namespace Kkm {
    class Device {
        class ConnParams;

    public:
        class NewConnParams;
        class KnownConnParams;
        class PrintableText;

        struct Call {
            class Result;
            class StatusResult;
            class ShiftStateResult;
            class ReceiptStateResult;
            class CashStatResult;
            class FndtOfdExchangeStatusResult;
            class FndtFnInfoResult;
            class FndtRegistrationInfoResult;
            class FndtLastRegistrationResult;
            class FndtLastReceiptResult;
            class FndtLastDocumentResult;
            class FndtErrorsResult;
            class FfdVersionResult;
            class FwVersionResult;
            class Details;
            class OperatorDetails;
            class CashDetails;
            class ReceiptDetails;
            class CloseDetails;
            class PrintDetails;
        };

        Device() = delete;
        Device(const Device &) = delete;
        Device(Device &&) = delete;
        explicit Device(const ConnParams &, std::wstring_view = {});
        explicit Device(const KnownConnParams &, std::wstring_view = {});
        ~Device();

        Device & operator=(const Device &) = delete;
        Device & operator=(Device &&) = delete;

        [[nodiscard]] const std::wstring & serialNumber() const;
        void getStatus(Call::StatusResult &);
        void getShiftState(Call::ShiftStateResult &);
        void getReceiptState(Call::ReceiptStateResult &);
        void getCashStat(Call::CashStatResult &);
        void getFndtOfdExchangeStatus(Call::FndtOfdExchangeStatusResult &);
        void getFndtFnInfo(Call::FndtFnInfoResult &);
        void getFndtRegistrationInfo(Call::FndtRegistrationInfoResult &);
        void getFndtLastRegistration(Call::FndtLastRegistrationResult &);
        void getFndtLastReceipt(Call::FndtLastReceiptResult &);
        void getFndtLastDocument(Call::FndtLastDocumentResult &);
        void getFndtErrors(Call::FndtErrorsResult &);
        void getFfdVersion(Call::FfdVersionResult &);
        void getFwVersion(Call::FwVersionResult &);
        void printHello();
        void printDemo(Call::Result &);
        void printNonFiscalDocument(const Call::PrintDetails &, Call::Result &);
        void printInfo(Call::Result &);
        void printFnRegistrations(Call::Result &);
        void printOfdExchangeStatus(Call::Result &);
        void printOfdTest(Call::Result &);
        void printCloseShiftReports(Call::Result &);
        void printLastDocument(Call::Result &);
        void registerCashIn(const Call::CashDetails &, Call::Result &);
        void registerCashOut(const Call::CashDetails &, Call::Result &);
        void registerSell(const Call::ReceiptDetails &, Call::Result &);
        void registerSellReturn(const Call::ReceiptDetails &, Call::Result &);
        void reportX(const Call::CloseDetails &, Call::Result &);
        void closeShift(const Call::CloseDetails &, Call::Result &);
        void resetState(const Call::CloseDetails &, Call::Result &);

    private:
        Atol::Fptr m_kkm;
        std::wstring m_serialNumber;
        std::wstring m_logPrefix;
        unsigned int m_lineLength;
        bool m_needToCancelReceipt;

        explicit Device(std::wstring_view);

        void connect(const ConnParams &);
        [[nodiscard, maybe_unused]] static std::wstring addMargins(std::wstring_view, int = 1, int = -1);
        [[maybe_unused]] void addSeparator(std::wstring &, int = 0, int = -1) const;
        [[nodiscard, maybe_unused]] std::wstring addSeparators(std::wstring_view, int = 0, int = -1) const;
        [[maybe_unused]] void subPrintSeparator(int = 0, int = -1);

        void subPrintText(
            std::wstring_view, bool = false, bool = false, bool = false,
            int = 1, int = 1, TextPosition = TextPosition::Auto
        );

        void subPrintText(const PrintableText &, TextPosition = TextPosition::Auto);
        void subCheckDocumentClosed(Call::Result &);
        void subSetOperator(const Call::OperatorDetails &);
        void subSetCustomer(const Call::ReceiptDetails &);
        void subSetSeller(const Call::ReceiptDetails &);
        void subRegisterReceipt(ReceiptType, const Call::ReceiptDetails &, Call::Result &);
        void subCashOut(const Call::OperatorDetails &, Call::Result &);
        void subCloseShift(const Call::OperatorDetails &, Call::Result &);
    };

    class Device::ConnParams {
        friend class Device;

    public:
        using Container = std::vector<std::wstring>;

        ConnParams(const ConnParams &) = default;
        ConnParams(ConnParams &&) noexcept = default;
        ~ConnParams() = default;

        ConnParams & operator=(const ConnParams &) = default;
        ConnParams & operator=(ConnParams &&) noexcept = default;
        explicit operator std::wstring() const;

        void apply(Device &) const;

    protected:
        Container m_params {};

        ConnParams() = default;
        explicit ConnParams(const Container &);
        explicit ConnParams(Container &&);

        void applyCommon(Device &) const;
        void applyCom(Device &) const;
        void applyUsb(Device &) const;
        void applyTcpIp(Device &) const;
        void applyBluetooth(Device &) const;
    };

    class Device::NewConnParams : public Device::ConnParams {
        friend class Device;

    public:
        NewConnParams() = delete;
        NewConnParams(const NewConnParams &) = default;
        NewConnParams(NewConnParams &&) noexcept = default;
        explicit NewConnParams(const std::wstring &);
        ~NewConnParams() = default;

        NewConnParams & operator=(const NewConnParams &) = default;
        NewConnParams & operator=(NewConnParams &&) noexcept = default;

        void save(const std::wstring &);
    };

    class Device::KnownConnParams : public Device::ConnParams {
        friend class Device;

    public:
        KnownConnParams() = delete;
        KnownConnParams(const KnownConnParams &) = default;
        KnownConnParams(KnownConnParams &&) noexcept = default;
        KnownConnParams(const NewConnParams &, std::wstring_view);
        KnownConnParams(NewConnParams &&, std::wstring_view);
        explicit KnownConnParams(const wchar_t *);
        explicit KnownConnParams(const std::filesystem::path &);
        ~KnownConnParams() = default;

        KnownConnParams & operator=(const KnownConnParams &) = default;
        KnownConnParams & operator=(KnownConnParams &&) noexcept = default;

        [[nodiscard]] const std::wstring & serialNumber() const;
        [[nodiscard]] static std::wstring serialNumber(const std::filesystem::path &);

    protected:
        std::wstring m_serialNumber {};

        explicit KnownConnParams(std::wstring &&);
    };

    class Device::PrintableText {
        friend class Device;

        std::wstring m_content {};
        int m_marginOuter { 0 };
        int m_marginInner { 1 };
        bool m_center { false };
        bool m_magnified { false };
        bool m_separated { false };

    public:
        PrintableText() = default;

        PrintableText(std::wstring && content, bool center, bool magnified, bool separated, bool margin)
        : m_content(std::forward<std::wstring>(content)), m_marginInner(margin ? 1 : 0),
          m_center(center), m_magnified(magnified), m_separated(separated) {}

        inline explicit operator bool() const {
            return !m_content.empty();
        }
    };

    class Device::Call::Result {
        friend class Device;

        std::wstring m_message { Basic::Wcs::c_ok };
        bool m_success { true };

    public:
        Result() = default;
        Result(const Result &) = default;
        Result(Result &&) noexcept = default;
        virtual ~Result() = default;

        Result & operator=(const Result &) = default;
        Result & operator=(Result &&) noexcept = default;

        void fail(std::wstring_view, const std::source_location & = std::source_location::current());
        void fail(Device & kkm, const std::source_location & = std::source_location::current());
        virtual bool exportTo(Nln::Json &);
    };

    class Device::Call::StatusResult : public Result {
        friend class Device;

        std::tm m_dateTime {};
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

    public:
        StatusResult() = default;
        StatusResult(const StatusResult &) = default;
        StatusResult(StatusResult &&) = default;
        ~StatusResult() override = default;

        StatusResult & operator=(const StatusResult &) = default;
        StatusResult & operator=(StatusResult &&) = default;

        bool exportTo(Nln::Json &) override;
    };

    class Device::Call::ShiftStateResult : public Result {
        friend class Device;

        std::tm m_expirationDateTime {};
        ShiftState m_shiftState {};
        unsigned int m_documentsCount {};
        unsigned int m_receiptNumber {};
        unsigned int m_shiftNumber {};

    public:
        ShiftStateResult() = default;
        ShiftStateResult(const ShiftStateResult &) = default;
        ShiftStateResult(ShiftStateResult &&) = default;
        ~ShiftStateResult() override = default;

        ShiftStateResult & operator=(const ShiftStateResult &) = default;
        ShiftStateResult & operator=(ShiftStateResult &&) = default;

        bool exportTo(Nln::Json &) override;
    };

    class Device::Call::ReceiptStateResult : public Result {
        friend class Device;

        double m_change {};
        double m_remainder {};
        double m_sum {};
        unsigned int m_documentNumber {};
        unsigned int m_receiptNumber {};
        ReceiptType m_receiptType {};

    public:
        ReceiptStateResult() = default;
        ReceiptStateResult(const ReceiptStateResult &) = default;
        ReceiptStateResult(ReceiptStateResult &&) = default;
        ~ReceiptStateResult() override = default;

        ReceiptStateResult & operator=(const ReceiptStateResult &) = default;
        ReceiptStateResult & operator=(ReceiptStateResult &&) = default;

        bool exportTo(Nln::Json &) override;
    };

    class Device::Call::CashStatResult : public Result {
        friend class Device;

        double m_cashSum {};
        double m_cashInSum {};
        double m_cashOutSum {};
        double m_sellCashSum {};
        double m_sellReturnCashSum {};
        unsigned int m_cashInCount {};
        unsigned int m_cashOutCount {};

    public:
        CashStatResult() = default;
        CashStatResult(const CashStatResult &) = default;
        CashStatResult(CashStatResult &&) = default;
        ~CashStatResult() override = default;

        CashStatResult & operator=(const CashStatResult &) = default;
        CashStatResult & operator=(CashStatResult &&) = default;

        bool exportTo(Nln::Json &) override;
    };

    class Device::Call::FndtOfdExchangeStatusResult : public Result {
        friend class Device;

        std::tm m_firstUnsentDateTime {};
        std::tm m_okpDateTime {};
        std::tm m_lastSentDateTime {};
        unsigned int m_exchangeStatus {};
        unsigned int m_firstUnsentNumber {};
        unsigned int m_unsentCount {};
        bool m_ofdMessageRead {};

    public:
        FndtOfdExchangeStatusResult() = default;
        FndtOfdExchangeStatusResult(const FndtOfdExchangeStatusResult &) = default;
        FndtOfdExchangeStatusResult(FndtOfdExchangeStatusResult &&) = default;
        ~FndtOfdExchangeStatusResult() override = default;

        FndtOfdExchangeStatusResult & operator=(const FndtOfdExchangeStatusResult &) = default;
        FndtOfdExchangeStatusResult & operator=(FndtOfdExchangeStatusResult &&) = default;

        bool exportTo(Nln::Json &) override;
    };

    class Device::Call::FndtFnInfoResult : public Result {
        friend class Device;

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

    public:
        FndtFnInfoResult() = default;
        FndtFnInfoResult(const FndtFnInfoResult &) = default;
        FndtFnInfoResult(FndtFnInfoResult &&) = default;
        ~FndtFnInfoResult() override = default;

        FndtFnInfoResult & operator=(const FndtFnInfoResult &) = default;
        FndtFnInfoResult & operator=(FndtFnInfoResult &&) = default;

        bool exportTo(Nln::Json &) override;
    };

    class Device::Call::FndtRegistrationInfoResult : public Result {
        friend class Device;

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

    public:
        FndtRegistrationInfoResult() = default;
        FndtRegistrationInfoResult(const FndtRegistrationInfoResult &) = default;
        FndtRegistrationInfoResult(FndtRegistrationInfoResult &&) = default;
        ~FndtRegistrationInfoResult() override = default;

        FndtRegistrationInfoResult & operator=(const FndtRegistrationInfoResult &) = default;
        FndtRegistrationInfoResult & operator=(FndtRegistrationInfoResult &&) = default;

        bool exportTo(Nln::Json &) override;
    };

    class Device::Call::FndtLastRegistrationResult : public Result {
        friend class Device;

        std::tm m_registrationDateTime {};
        unsigned int m_documentNumber {};
        unsigned int m_registrationsCount {};

    public:
        FndtLastRegistrationResult() = default;
        FndtLastRegistrationResult(const FndtLastRegistrationResult &) = default;
        FndtLastRegistrationResult(FndtLastRegistrationResult &&) = default;
        ~FndtLastRegistrationResult() override = default;

        FndtLastRegistrationResult & operator=(const FndtLastRegistrationResult &) = default;
        FndtLastRegistrationResult & operator=(FndtLastRegistrationResult &&) = default;

        bool exportTo(Nln::Json &) override;
    };

    class Device::Call::FndtLastReceiptResult : public Result {
        friend class Device;

        std::tm m_documentDateTime {};
        std::wstring m_fiscalSign {};
        double m_receiptSum {};
        unsigned int m_documentNumber {};

    public:
        FndtLastReceiptResult() = default;
        FndtLastReceiptResult(const FndtLastReceiptResult &) = default;
        FndtLastReceiptResult(FndtLastReceiptResult &&) = default;
        ~FndtLastReceiptResult() override = default;

        FndtLastReceiptResult & operator=(const FndtLastReceiptResult &) = default;
        FndtLastReceiptResult & operator=(FndtLastReceiptResult &&) = default;

        bool exportTo(Nln::Json &) override;
    };

    class Device::Call::FndtLastDocumentResult : public Result {
        friend class Device;

        std::tm m_documentDateTime {};
        std::wstring m_fiscalSign {};
        unsigned int m_documentNumber {};

    public:
        FndtLastDocumentResult() = default;
        FndtLastDocumentResult(const FndtLastDocumentResult &) = default;
        FndtLastDocumentResult(FndtLastDocumentResult &&) = default;
        ~FndtLastDocumentResult() override = default;

        FndtLastDocumentResult & operator=(const FndtLastDocumentResult &) = default;
        FndtLastDocumentResult & operator=(FndtLastDocumentResult &&) = default;

        bool exportTo(Nln::Json &) override;
    };

    class Device::Call::FndtErrorsResult : public Result {
        friend class Device;

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

    public:
        FndtErrorsResult() = default;
        FndtErrorsResult(const FndtErrorsResult &) = default;
        FndtErrorsResult(FndtErrorsResult &&) = default;
        ~FndtErrorsResult() override = default;

        FndtErrorsResult & operator=(const FndtErrorsResult &) = default;
        FndtErrorsResult & operator=(FndtErrorsResult &&) = default;

        bool exportTo(Nln::Json &) override;
    };

    class Device::Call::FfdVersionResult : public Result {
        friend class Device;

        FfdVersion m_deviceFfdVersion { FfdVersion::Unknown };
        FfdVersion m_devMaxFfdVersion { FfdVersion::Unknown };
        FfdVersion m_devMinFfdVersion { FfdVersion::Unknown };
        FfdVersion m_fnFfdVersion { FfdVersion::Unknown };
        FfdVersion m_fnMaxFfdVersion { FfdVersion::Unknown };
        FfdVersion m_ffdVersion { FfdVersion::Unknown };
        // unsigned int m_kktVersion {};

    public:
        FfdVersionResult() = default;
        FfdVersionResult(const FfdVersionResult &) = default;
        FfdVersionResult(FfdVersionResult &&) = default;
        ~FfdVersionResult() override = default;

        FfdVersionResult & operator=(const FfdVersionResult &) = default;
        FfdVersionResult & operator=(FfdVersionResult &&) = default;

        bool exportTo(Nln::Json &) override;
    };

    class Device::Call::FwVersionResult : public Result {
        friend class Device;

        std::wstring m_bootVersion {};
        std::wstring m_configurationVersion {};
        std::wstring m_controlUnitVersion {};
        std::wstring m_firmwareVersion {};
        std::wstring m_releaseVersion {};
        std::wstring m_templatesVersion {};

    public:
        FwVersionResult() = default;
        FwVersionResult(const FwVersionResult &) = default;
        FwVersionResult(FwVersionResult &&) = default;
        ~FwVersionResult() override = default;

        FwVersionResult & operator=(const FwVersionResult &) = default;
        FwVersionResult & operator=(FwVersionResult &&) = default;

        bool exportTo(Nln::Json &) override;
    };

    class Device::Call::Details {
        friend class Device;

        bool m_electronically { false };

    public:
        Details() = default;
        Details(const Details &) = default;
        Details(Details &&) noexcept = default;
        explicit Details(const Nln::Json &);
        virtual ~Details() = default;

        Details & operator=(const Details &) = default;
        Details & operator=(Details &&) = default;
    };

    class Device::Call::PrintDetails : public Details {
        friend class Device;

        std::vector<PrintableText> m_document {};
        bool m_cliche { false };
        bool m_footer { false };

    public:
        PrintDetails() = default;
        PrintDetails(const PrintDetails &) = default;
        PrintDetails(PrintDetails &&) noexcept = default;
        explicit PrintDetails(const Nln::Json &);
        ~PrintDetails() override = default;

        PrintDetails & operator=(const PrintDetails &) = default;
        PrintDetails & operator=(PrintDetails &&) = default;
    };

    class Device::Call::OperatorDetails : public Details {
        friend class Device;

        std::wstring m_operatorName {};
        std::wstring m_operatorInn {};

    public:
        OperatorDetails() = default;
        OperatorDetails(const OperatorDetails &) = default;
        OperatorDetails(OperatorDetails &&) noexcept = default;
        explicit OperatorDetails(const Nln::Json &);
        OperatorDetails(std::wstring_view, std::wstring_view);
        ~OperatorDetails() override = default;

        OperatorDetails & operator=(const OperatorDetails &) = default;
        OperatorDetails & operator=(OperatorDetails &&) = default;
    };

    class Device::Call::CashDetails : public OperatorDetails {
        friend class Device;

        double m_cashSum {};

    public:
        CashDetails() = default;
        CashDetails(const CashDetails &) = default;
        CashDetails(CashDetails &&) = default;
        explicit CashDetails(const Nln::Json &);
        ~CashDetails() override = default;

        CashDetails & operator=(const CashDetails &) = default;
        CashDetails & operator=(CashDetails &&) = default;
    };

    class Device::Call::ReceiptDetails : public OperatorDetails {
        friend class Device;

    public:
        class ItemDetails {
            friend class Device;

            std::wstring m_commodityName;
            double m_price;
            double m_quantity;
            MeasurementUnit m_unit;
            Tax m_tax;

        public:
            ItemDetails() = delete;
            ItemDetails(const ItemDetails &) = default;
            ItemDetails(ItemDetails &&) noexcept = default;
            ItemDetails(std::wstring_view, double, double, MeasurementUnit, Tax);
            ItemDetails(std::wstring &&, double, double, MeasurementUnit, Tax);
            ~ItemDetails() = default;

            ItemDetails & operator=(const ItemDetails &) = default;
            ItemDetails & operator=(ItemDetails &&) noexcept = default;
        };

    private:
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
        std::vector<ItemDetails> m_items;
        double m_paymentSum { 0 };
        PaymentType m_paymentType { PaymentType::Cash };
        int m_electroPaymentMethod { 0 };
        bool m_customerDataIsPresent { false };
        bool m_sellerDataIsPresent { false };
        bool m_electroPaymentInfo { false };

    public:
        ReceiptDetails() = default;
        ReceiptDetails(const ReceiptDetails &) = default;
        ReceiptDetails(ReceiptDetails &&) = default;
        explicit ReceiptDetails(const Nln::Json &);
        ~ReceiptDetails() override = default;

        ReceiptDetails & operator=(const ReceiptDetails &) = default;
        ReceiptDetails & operator=(ReceiptDetails &&) = default;
    };

    class Device::Call::CloseDetails : public OperatorDetails {
        friend class Device;

        bool m_closeShift { false };
        bool m_cashOut { false };

    public:
        CloseDetails() = default;
        CloseDetails(const CloseDetails &) = default;
        CloseDetails(CloseDetails &&) noexcept = default;
        explicit CloseDetails(const Nln::Json &);
        CloseDetails(std::wstring_view, std::wstring_view, bool, bool);
        ~CloseDetails() override = default;

        CloseDetails & operator=(const CloseDetails &) = default;
        CloseDetails & operator=(CloseDetails &&) = default;
    };

    template<std::derived_from<Device::Call::Result> ResultType>
    using UndetailedMethod = void (Device::*)(ResultType &);

    template<class ResultType>
    inline void callUndetailedMethod(Device & kkm, UndetailedMethod<ResultType> method, Nln::Json & result) {
        assert(result.is_object());
        ResultType data {};
        (kkm.*method)(data);
        data.exportTo(result);
    }

    template<typename ... METHODS>
    inline void collectDataFromMethods(Nln::Json & result, Device & kkm, METHODS ... method) {
        (callUndetailedMethod(kkm, method, result), ...);
    }

    template<std::derived_from<Device::Call::Result> ResultType, std::derived_from<Device::Call::Details> DetailsType>
    using DetailedMethod = void (Device::*)(const DetailsType &, ResultType &);

    template<class ResultType, class DetailsType>
    inline void callDetailedMethod(
        Device & kkm,
        DetailedMethod<ResultType, DetailsType> method,
        const DetailsType & details,
        Nln::Json & result
    ) {
        assert(result.is_object());
        ResultType data {};
        (kkm.*method)(details, data);
        data.exportTo(result);
    }
}
