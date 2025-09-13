// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <lib/except.h>
#include <string_view>
#include <fptr10.h>

namespace Kkm {
    namespace Atol = Atol::Fptr;

    class Device;

    class Failure : public Basic::Failure {
    public:
        Failure() = delete;
        Failure(const Failure &) = default;
        Failure(Failure &&) noexcept = default;

        explicit Failure(
            const std::wstring_view message,
            std::source_location && location = std::source_location::current()
        ) : Basic::Failure(message, std::forward<std::source_location>(location)) {}

        explicit Failure(
            Atol::Fptr & kkm,
            std::source_location && location = std::source_location::current()
        ) : Basic::Failure(kkm.errorDescription(), std::forward<std::source_location>(location)) {
            kkm.resetError();
        }

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

    enum class TextPosition {
        Auto = Atol::LIBFPTR_DEFER_NONE,
        Pre = Atol::LIBFPTR_DEFER_PRE,
        Post = Atol::LIBFPTR_DEFER_POST
    };
}
