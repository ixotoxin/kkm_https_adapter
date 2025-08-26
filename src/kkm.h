// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <lib/json.h>
#include <filesystem>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <fptr10.h>
#include "defaults.h"
#include "messages.h"

namespace Kkm {
    namespace Wcs {
        using Basic::Wcs::c_ok;
    }

    namespace Mbs {
        using Basic::Mbs::c_ok;
    }

    namespace Atol = Atol::Fptr;

    class Device;

    class Failure : public Basic::Failure {
    public:
        Failure() = delete;
        Failure(const Failure &) = default;
        Failure(Failure &&) noexcept = default;
        explicit Failure(std::wstring_view, std::source_location && = std::source_location::current());
        explicit Failure(Device &, std::source_location && = std::source_location::current());
        ~Failure() override = default;

        Failure & operator=(const Failure &) = default;
        Failure & operator=(Failure &&) noexcept = default;
    };

    enum class TimeZone : int {
        Device = Atol::LIBFPTR_TIME_ZONE_DEVICE,
        Zone1 = Atol::LIBFPTR_TIME_ZONE_1,
        Zone2 = Atol::LIBFPTR_TIME_ZONE_2,
        Zone3 = Atol::LIBFPTR_TIME_ZONE_3,
        Zone4 = Atol::LIBFPTR_TIME_ZONE_4,
        Zone5 = Atol::LIBFPTR_TIME_ZONE_5,
        Zone6 = Atol::LIBFPTR_TIME_ZONE_6,
        Zone7 = Atol::LIBFPTR_TIME_ZONE_7,
        Zone8 = Atol::LIBFPTR_TIME_ZONE_8,
        Zone9 = Atol::LIBFPTR_TIME_ZONE_9,
        Zone10 = Atol::LIBFPTR_TIME_ZONE_10,
        Zone11 = Atol::LIBFPTR_TIME_ZONE_11
    };

    enum class FfdVersion : int {
        Unknown = Atol::LIBFPTR_FFD_UNKNOWN,
        V_1_0_5 = Atol::LIBFPTR_FFD_1_0_5,
        V_1_1 = Atol::LIBFPTR_FFD_1_1,
        V_1_2 = Atol::LIBFPTR_FFD_1_2
    };

    enum class ShiftState : int {
        Closed = Atol::LIBFPTR_SS_CLOSED,
        Opened = Atol::LIBFPTR_SS_OPENED,
        Expired = Atol::LIBFPTR_SS_EXPIRED
    };

    enum class ReceiptType : int {
        Closed = Atol::LIBFPTR_RT_CLOSED,
        Sell = Atol::LIBFPTR_RT_SELL,
        SellReturn = Atol::LIBFPTR_RT_SELL_RETURN,
        SellCorrection = Atol::LIBFPTR_RT_SELL_CORRECTION,
        SellReturnCorrection = Atol::LIBFPTR_RT_SELL_RETURN_CORRECTION,
        Buy = Atol::LIBFPTR_RT_BUY,
        BuyReturn = Atol::LIBFPTR_RT_BUY_RETURN,
        BuyCorrection = Atol::LIBFPTR_RT_BUY_CORRECTION,
        BuyReturnCorrection = Atol::LIBFPTR_RT_BUY_RETURN_CORRECTION
    };

    enum class DocumentType : int {
        Closed = Atol::LIBFPTR_DT_CLOSED,
        ReceiptSell = Atol::LIBFPTR_DT_RECEIPT_SELL,
        ReceiptSellReturn = Atol::LIBFPTR_DT_RECEIPT_SELL_RETURN,
        ReceiptBuy = Atol::LIBFPTR_DT_RECEIPT_BUY,
        ReceiptBuyReturn = Atol::LIBFPTR_DT_RECEIPT_BUY_RETURN,
        OpenShift = Atol::LIBFPTR_DT_OPEN_SHIFT,
        CloseShift = Atol::LIBFPTR_DT_CLOSE_SHIFT,
        Registration = Atol::LIBFPTR_DT_REGISTRATION,
        CloseArchive = Atol::LIBFPTR_DT_CLOSE_ARCHIVE,
        OfdExchangeStatus = Atol::LIBFPTR_DT_OFD_EXCHANGE_STATUS,
        ReceiptSellCorrection = Atol::LIBFPTR_DT_RECEIPT_SELL_CORRECTION,
        ReceiptBuyCorrection = Atol::LIBFPTR_DT_RECEIPT_BUY_CORRECTION,
        ReceiptSellReturnCorrection = Atol::LIBFPTR_DT_RECEIPT_SELL_RETURN_CORRECTION,
        ReceiptBuyReturnCorrection = Atol::LIBFPTR_DT_RECEIPT_BUY_RETURN_CORRECTION,
        DocumentService = Atol::LIBFPTR_DT_DOCUMENT_SERVICE,
        DocumentCopy = Atol::LIBFPTR_DT_DOCUMENT_COPY
    };

    enum class MeasurementUnit : int {
        Piece = Atol::LIBFPTR_IU_PIECE,
        Gram = Atol::LIBFPTR_IU_GRAM,
        Kilogram = Atol::LIBFPTR_IU_KILOGRAM,
        Ton = Atol::LIBFPTR_IU_TON,
        Centimeter = Atol::LIBFPTR_IU_CENTIMETER,
        Decimeter = Atol::LIBFPTR_IU_DECIMETER,
        Meter = Atol::LIBFPTR_IU_METER,
        SquareCentimeter = Atol::LIBFPTR_IU_SQUARE_CENTIMETER,
        SquareDecimeter = Atol::LIBFPTR_IU_SQUARE_DECIMETER,
        SquareMeter = Atol::LIBFPTR_IU_SQUARE_METER,
        Milliliter = Atol::LIBFPTR_IU_MILLILITER,
        Liter = Atol::LIBFPTR_IU_LITER,
        CubicMeter = Atol::LIBFPTR_IU_CUBIC_METER,
        KilowattHour = Atol::LIBFPTR_IU_KILOWATT_HOUR,
        Gkal = Atol::LIBFPTR_IU_GKAL,
        Day = Atol::LIBFPTR_IU_DAY,
        Hour = Atol::LIBFPTR_IU_HOUR,
        Minute = Atol::LIBFPTR_IU_MINUTE,
        Second = Atol::LIBFPTR_IU_SECOND,
        Kilobyte = Atol::LIBFPTR_IU_KILOBYTE,
        Megabyte = Atol::LIBFPTR_IU_MEGABYTE,
        Gigabyte = Atol::LIBFPTR_IU_GIGABYTE,
        Terabyte = Atol::LIBFPTR_IU_TERABYTE,
        Other = Atol::LIBFPTR_IU_OTHER
    };

    enum class Tax : int {
        No = Atol::LIBFPTR_TAX_NO,
        Vat0 = Atol::LIBFPTR_TAX_VAT0,
        Vat5 = Atol::LIBFPTR_TAX_VAT5,
        Vat105 = Atol::LIBFPTR_TAX_VAT105,
        Vat7 = Atol::LIBFPTR_TAX_VAT7,
        Vat107 = Atol::LIBFPTR_TAX_VAT107,
        Vat10 = Atol::LIBFPTR_TAX_VAT10,
        Vat110 = Atol::LIBFPTR_TAX_VAT110,
        // Vat18 = Atol::LIBFPTR_TAX_VAT18,
        // Vat118 = Atol::LIBFPTR_TAX_VAT118,
        Vat20 = Atol::LIBFPTR_TAX_VAT20,
        Vat120 = Atol::LIBFPTR_TAX_VAT120,
        // Department = Atol::LIBFPTR_TAX_DEPARTMENT
    };

    enum class PaymentType : int {
        Cash = Atol::LIBFPTR_PT_CASH,
        Electronically = Atol::LIBFPTR_PT_ELECTRONICALLY,
        // Prepaid = Atol::LIBFPTR_PT_PREPAID,
        // Credit = Atol::LIBFPTR_PT_CREDIT,
        // Other = Atol::LIBFPTR_PT_OTHER,
        // Pt6 = Atol::LIBFPTR_PT_6,
        // Pt7 = Atol::LIBFPTR_PT_7,
        // Pt8 = Atol::LIBFPTR_PT_8,
        // Pt9 = Atol::LIBFPTR_PT_9,
        // Pt10 = Atol::LIBFPTR_PT_10
    };

    class Device {
        friend class Failure;

        class ConnParams;

    public:
        class NewConnParams;
        class KnownConnParams;

        class PrintableText {
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
        enum class TextPosition {
            Auto = Atol::LIBFPTR_DEFER_NONE,
            Pre = Atol::LIBFPTR_DEFER_PRE,
            Post = Atol::LIBFPTR_DEFER_POST
        };

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

    class Device::Call::Result {
        friend class Device;

        std::wstring m_message { Wcs::c_ok };
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

    namespace Wcs {
        inline const std::array<std::wstring, 11> c_allowedBaudRate {
            std::to_wstring(Atol::LIBFPTR_PORT_BR_1200),
            std::to_wstring(Atol::LIBFPTR_PORT_BR_2400),
            std::to_wstring(Atol::LIBFPTR_PORT_BR_4800),
            std::to_wstring(Atol::LIBFPTR_PORT_BR_9600),
            std::to_wstring(Atol::LIBFPTR_PORT_BR_19200),
            std::to_wstring(Atol::LIBFPTR_PORT_BR_38400),
            std::to_wstring(Atol::LIBFPTR_PORT_BR_57600),
            std::to_wstring(Atol::LIBFPTR_PORT_BR_115200),
            std::to_wstring(Atol::LIBFPTR_PORT_BR_230400),
            std::to_wstring(Atol::LIBFPTR_PORT_BR_460800),
            std::to_wstring(Atol::LIBFPTR_PORT_BR_921600)
        };
    }

    namespace Mbs {
        inline const std::unordered_map<unsigned int, std::string_view> c_models {
            { Atol::LIBFPTR_MODEL_ALLIANCE_20F, "АЛЬЯНС 20Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_1F, "АТОЛ 1Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_2F, "АТОЛ 2Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_11F, "АТОЛ 11Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_15F, "АТОЛ 15Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_20F, "АТОЛ 20Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_22F, "АТОЛ 22Ф (АТОЛ FPrint-22ПТК)" },
            { Atol::LIBFPTR_MODEL_ATOL_22V2F, "АТОЛ 22 v2 Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_25F, "АТОЛ 25Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_27F, "АТОЛ 27Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_27_FP7_F, "АТОЛ 27 FP7 Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_30F, "АТОЛ 30Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_35F, "АТОЛ 35Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_42FA, "АТОЛ 42ФА" },
            { Atol::LIBFPTR_MODEL_ATOL_42FS, "АТОЛ 42ФС" },
            { Atol::LIBFPTR_MODEL_ATOL_47FA, "АТОЛ 47ФА" },
            { Atol::LIBFPTR_MODEL_ATOL_50F, "АТОЛ 50Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_52F, "АТОЛ 52Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_55F, "АТОЛ 55Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_55V2F, "АТОЛ 55 v2 Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_60F, "АТОЛ 60Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_77F, "АТОЛ 77Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_90F, "АТОЛ 90Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_91F, "АТОЛ 91Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_92F, "АТОЛ 92Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_PT_5F, "АТОЛ PT-5Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_SIGMA_10, "АТОЛ Sigma 10" },
            { Atol::LIBFPTR_MODEL_ATOL_SIGMA_7F, "АТОЛ Sigma 7Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_SIGMA_8F, "АТОЛ Sigma 8Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_STB_6F, "АТОЛ СТБ 6Ф" },
            { Atol::LIBFPTR_MODEL_KAZNACHEY_FA, "Казначей ФА" }
        };

        inline const std::unordered_map<std::string, TimeZone> c_timeZoneMap {
            { "device", TimeZone::Device },
            { "tz" + std::to_string(static_cast<int>(TimeZone::Device)), TimeZone::Device },
            { "+2", TimeZone::Zone1 },
            { "utc+2", TimeZone::Zone1 },
            { "tz" + std::to_string(static_cast<int>(TimeZone::Zone1)), TimeZone::Zone1 },
            { "+3", TimeZone::Zone2 },
            { "utc+3", TimeZone::Zone2 },
            { "tz" + std::to_string(static_cast<int>(TimeZone::Zone2)), TimeZone::Zone2 },
            { "+4", TimeZone::Zone3 },
            { "utc+4", TimeZone::Zone3 },
            { "tz" + std::to_string(static_cast<int>(TimeZone::Zone3)), TimeZone::Zone3 },
            { "+5", TimeZone::Zone4 },
            { "utc+5", TimeZone::Zone4 },
            { "tz" + std::to_string(static_cast<int>(TimeZone::Zone4)), TimeZone::Zone4 },
            { "+6", TimeZone::Zone5 },
            { "utc+6", TimeZone::Zone5 },
            { "tz" + std::to_string(static_cast<int>(TimeZone::Zone5)), TimeZone::Zone5 },
            { "+7", TimeZone::Zone6 },
            { "utc+7", TimeZone::Zone6 },
            { "tz" + std::to_string(static_cast<int>(TimeZone::Zone6)), TimeZone::Zone6 },
            { "+8", TimeZone::Zone7 },
            { "utc+8", TimeZone::Zone7 },
            { "tz" + std::to_string(static_cast<int>(TimeZone::Zone7)), TimeZone::Zone7 },
            { "+9", TimeZone::Zone8 },
            { "utc+9", TimeZone::Zone8 },
            { "tz" + std::to_string(static_cast<int>(TimeZone::Zone8)), TimeZone::Zone8 },
            { "+10", TimeZone::Zone9 },
            { "utc+10", TimeZone::Zone9 },
            { "tz" + std::to_string(static_cast<int>(TimeZone::Zone9)), TimeZone::Zone9 },
            { "+11", TimeZone::Zone10 },
            { "utc+11", TimeZone::Zone10 },
            { "tz" + std::to_string(static_cast<int>(TimeZone::Zone10)), TimeZone::Zone10 },
            { "+12", TimeZone::Zone11 },
            { "utc+12", TimeZone::Zone11 },
            { "tz" + std::to_string(static_cast<int>(TimeZone::Zone11)), TimeZone::Zone11 }
        };

        inline const std::unordered_map<FfdVersion, std::string_view> c_ffdVersions {
            { FfdVersion::Unknown, "[неизвестная версия]" },
            { FfdVersion::V_1_0_5, "1.0.5" },
            { FfdVersion::V_1_1, "1.1" },
            { FfdVersion::V_1_2, "1.2" }
        };

        inline const std::unordered_map<ShiftState, std::string_view> c_shiftStateLabels {
            { ShiftState::Closed, Mbs::c_closedShift },
            { ShiftState::Opened, Mbs::c_openedShift },
            { ShiftState::Expired, Mbs::c_expiredShift },
        };

        inline const std::unordered_map<ReceiptType, std::string_view> c_receiptTypeLabels {
            { ReceiptType::Closed, Mbs::c_closedReceipt },
            { ReceiptType::Sell, Mbs::c_sellReceipt },
            { ReceiptType::SellReturn, Mbs::c_sellReturnReceipt },
            { ReceiptType::SellCorrection, Mbs::c_sellCorrectionReceipt },
            { ReceiptType::SellReturnCorrection, Mbs::c_sellReturnCorrectionReceipt },
            { ReceiptType::Buy, Mbs::c_buyReceipt },
            { ReceiptType::BuyReturn, Mbs::c_buyReturnReceipt },
            { ReceiptType::BuyCorrection, Mbs::c_buyCorrectionReceipt },
            { ReceiptType::BuyReturnCorrection, Mbs::c_buyReturnCorrectionReceipt },
        };

        inline const std::unordered_map<DocumentType, std::string_view> c_documentTypeLabels {
            { DocumentType::Closed, Mbs::c_closedDocument },
            { DocumentType::ReceiptSell, Mbs::c_sellReceipt },
            { DocumentType::ReceiptSellReturn, Mbs::c_sellReturnReceipt },
            { DocumentType::ReceiptSellCorrection, Mbs::c_sellCorrectionReceipt },
            { DocumentType::ReceiptSellReturnCorrection, Mbs::c_sellReturnCorrectionReceipt },
            { DocumentType::ReceiptBuy, Mbs::c_buyReceipt },
            { DocumentType::ReceiptBuyReturn, Mbs::c_buyReturnReceipt },
            { DocumentType::ReceiptBuyCorrection, Mbs::c_buyCorrectionReceipt },
            { DocumentType::ReceiptBuyReturnCorrection, Mbs::c_buyReturnCorrectionReceipt },
            { DocumentType::OpenShift, Mbs::c_openShiftDocument },
            { DocumentType::CloseShift, Mbs::c_closeShiftDocument },
            { DocumentType::Registration, Mbs::c_registrationDocument },
            { DocumentType::CloseArchive, Mbs::c_closeArchiveDocument },
            { DocumentType::OfdExchangeStatus, Mbs::c_ofdExchangeStatusDocument },
            { DocumentType::DocumentService, Mbs::c_serviceDocument },
            { DocumentType::DocumentCopy, Mbs::c_documentCopy },
        };

        inline const std::unordered_map<std::string, MeasurementUnit> c_measurementUnitMap {
            { "piece", MeasurementUnit::Piece },
            { std::to_string(static_cast<int>(MeasurementUnit::Piece)), MeasurementUnit::Piece },
            { "gram", MeasurementUnit::Gram },
            { std::to_string(static_cast<int>(MeasurementUnit::Gram)), MeasurementUnit::Gram },
            { "kilogram", MeasurementUnit::Kilogram },
            { std::to_string(static_cast<int>(MeasurementUnit::Kilogram)), MeasurementUnit::Kilogram },
            { "ton", MeasurementUnit::Ton },
            { std::to_string(static_cast<int>(MeasurementUnit::Ton)), MeasurementUnit::Ton },
            { "centimeter", MeasurementUnit::Centimeter },
            { std::to_string(static_cast<int>(MeasurementUnit::Centimeter)), MeasurementUnit::Centimeter },
            { "decimeter", MeasurementUnit::Decimeter },
            { std::to_string(static_cast<int>(MeasurementUnit::Decimeter)), MeasurementUnit::Decimeter },
            { "meter", MeasurementUnit::Meter },
            { std::to_string(static_cast<int>(MeasurementUnit::Meter)), MeasurementUnit::Meter },
            { "squarecentimeter", MeasurementUnit::SquareCentimeter },
            { "square_centimeter", MeasurementUnit::SquareCentimeter },
            { std::to_string(static_cast<int>(MeasurementUnit::SquareCentimeter)), MeasurementUnit::SquareCentimeter },
            { "squaredecimeter", MeasurementUnit::SquareDecimeter },
            { "square_decimeter", MeasurementUnit::SquareDecimeter },
            { std::to_string(static_cast<int>(MeasurementUnit::SquareDecimeter)), MeasurementUnit::SquareDecimeter },
            { "squaremeter", MeasurementUnit::SquareMeter },
            { "square_meter", MeasurementUnit::SquareMeter },
            { std::to_string(static_cast<int>(MeasurementUnit::SquareMeter)), MeasurementUnit::SquareMeter },
            { "milliliter", MeasurementUnit::Milliliter },
            { std::to_string(static_cast<int>(MeasurementUnit::Milliliter)), MeasurementUnit::Milliliter },
            { "liter", MeasurementUnit::Liter },
            { std::to_string(static_cast<int>(MeasurementUnit::Liter)), MeasurementUnit::Liter },
            { "cubicmeter", MeasurementUnit::CubicMeter },
            { "cubic_meter", MeasurementUnit::CubicMeter },
            { std::to_string(static_cast<int>(MeasurementUnit::CubicMeter)), MeasurementUnit::CubicMeter },
            { "kilowatthour", MeasurementUnit::KilowattHour },
            { "kilowatt_hour", MeasurementUnit::KilowattHour },
            { std::to_string(static_cast<int>(MeasurementUnit::KilowattHour)), MeasurementUnit::KilowattHour },
            { "gkal", MeasurementUnit::Gkal },
            { std::to_string(static_cast<int>(MeasurementUnit::Gkal)), MeasurementUnit::Gkal },
            { "day", MeasurementUnit::Day },
            { std::to_string(static_cast<int>(MeasurementUnit::Day)), MeasurementUnit::Day },
            { "hour", MeasurementUnit::Hour },
            { std::to_string(static_cast<int>(MeasurementUnit::Hour)), MeasurementUnit::Hour },
            { "minute", MeasurementUnit::Minute },
            { std::to_string(static_cast<int>(MeasurementUnit::Minute)), MeasurementUnit::Minute },
            { "second", MeasurementUnit::Second },
            { std::to_string(static_cast<int>(MeasurementUnit::Second)), MeasurementUnit::Second },
            { "kilobyte", MeasurementUnit::Kilobyte },
            { std::to_string(static_cast<int>(MeasurementUnit::Kilobyte)), MeasurementUnit::Kilobyte },
            { "megabyte", MeasurementUnit::Megabyte },
            { std::to_string(static_cast<int>(MeasurementUnit::Megabyte)), MeasurementUnit::Megabyte },
            { "gigabyte", MeasurementUnit::Gigabyte },
            { std::to_string(static_cast<int>(MeasurementUnit::Gigabyte)), MeasurementUnit::Gigabyte },
            { "terabyte", MeasurementUnit::Terabyte },
            { std::to_string(static_cast<int>(MeasurementUnit::Terabyte)), MeasurementUnit::Terabyte },
            { "other", MeasurementUnit::Other },
            { std::to_string(static_cast<int>(MeasurementUnit::Other)), MeasurementUnit::Other },
        };

        inline const std::unordered_map<std::string, Tax> c_taxCastMap {
            { "no", Tax::No },
            { std::to_string(static_cast<int>(Tax::No)), Tax::No },
            { "vat0", Tax::Vat0 },
            { std::to_string(static_cast<int>(Tax::Vat0)), Tax::Vat0 },
            { "vat5", Tax::Vat5 },
            { std::to_string(static_cast<int>(Tax::Vat5)), Tax::Vat5 },
            { "vat105", Tax::Vat105 },
            { std::to_string(static_cast<int>(Tax::Vat105)), Tax::Vat105 },
            { "vat7", Tax::Vat7 },
            { std::to_string(static_cast<int>(Tax::Vat7)), Tax::Vat7 },
            { "vat107", Tax::Vat107 },
            { std::to_string(static_cast<int>(Tax::Vat107)), Tax::Vat107 },
            { "vat10", Tax::Vat10 },
            { std::to_string(static_cast<int>(Tax::Vat10)), Tax::Vat10 },
            { "vat110", Tax::Vat110 },
            { std::to_string(static_cast<int>(Tax::Vat110)), Tax::Vat110 },
            { "vat20", Tax::Vat20 },
            { std::to_string(static_cast<int>(Tax::Vat20)), Tax::Vat20 },
            { "vat120", Tax::Vat120 },
            { std::to_string(static_cast<int>(Tax::Vat120)), Tax::Vat120 }
        };

        inline const std::unordered_map<std::string, PaymentType> c_paymentTypeCastMap {
            { "cash", PaymentType::Cash },
            { std::to_string(static_cast<int>(PaymentType::Cash)), PaymentType::Cash },
            { "electronically", PaymentType::Electronically },
            { std::to_string(static_cast<int>(PaymentType::Electronically)), PaymentType::Electronically }
        };
    }

    inline std::wstring s_dbDirectory { c_defDbDirectory };
    inline std::wstring s_defaultBaudRate { c_defBaudRate };
    inline size_t s_defaultLineLength { c_defLineLength };
    inline TimeZone s_timeZone { TimeZone::Device };
    inline bool s_timeZoneConfigured { false };
    inline auto s_documentClosingTimeout = c_defDocumentClosingTimeout;
    inline std::wstring s_cliOperatorName { c_defCliOperatorName };
    inline std::wstring s_cliOperatorInn { c_defCliOperatorInn };
    inline std::wstring s_customerAccountField { c_customerAccountField };
    inline double s_maxCashInOut { c_defMaxCashInOut };
    inline double s_maxPrice { c_defMaxPrice };
    inline double s_maxQuantity { c_defMaxQuantity };
}
