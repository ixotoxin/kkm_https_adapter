// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "callparams.h"
#include "defaults.h"
#include "variables.h"
#include "strings.h"

namespace Kkm {
    using Basic::DataError;

    [[maybe_unused]]
    PrintableText::PrintableText(
        std::wstring && content,
        bool center,
        bool magnified,
        bool separated,
        bool margin
    ) : m_content(std::forward<std::wstring>(content)), m_marginInner(margin ? 1 : 0),
        m_center(center), m_magnified(magnified), m_separated(separated) {}

    [[maybe_unused]]
    OperatorDetails::OperatorDetails(
        const std::wstring_view name,
        const std::wstring_view inn
    ) : Details(), m_operatorName(name), m_operatorInn(inn) {}

    [[maybe_unused]]
    CloseDetails::CloseDetails(
        const std::wstring_view name,
        const std::wstring_view inn,
        const bool closeShift,
        const bool cashOut
    ) : OperatorDetails(name, inn), m_closeShift(closeShift), m_cashOut(cashOut) {}

    [[maybe_unused]]
    ReceiptItemDetails::ReceiptItemDetails(
        const std::wstring_view commodityName,
        const double price,
        const double quantity,
        const MeasurementUnit unit,
        const Tax tax
    ) : m_commodityName(commodityName), m_price(price), m_quantity(quantity), m_unit(unit), m_tax(tax) {
        if (m_commodityName.empty()) {
            throw DataError(Wcs::c_invalidData, L"commodityName"); // NOLINT(*-exception-baseclass)
        }
        if (m_price < c_minMaxPrice || m_price > s_maxPrice) {
            throw DataError(Wcs::c_invalidData, L"price"); // NOLINT(*-exception-baseclass)
        }
        if (m_quantity < c_minQuantity || m_quantity > s_maxQuantity) {
            throw DataError(Wcs::c_invalidData, L"quantity"); // NOLINT(*-exception-baseclass)
        }
    }

    [[maybe_unused]]
    ReceiptItemDetails::ReceiptItemDetails(
        std::wstring && commodityName,
        const double price,
        const double quantity,
        const MeasurementUnit unit,
        const Tax tax
    ) : m_commodityName(std::forward<std::wstring>(commodityName)),
        m_price(price), m_quantity(quantity), m_unit(unit), m_tax(tax) {
        if (m_commodityName.empty()) {
            throw DataError(Wcs::c_invalidData, L"commodityName"); // NOLINT(*-exception-baseclass)
        }
        if (m_price < c_minMaxPrice || m_price > s_maxPrice) {
            throw DataError(Wcs::c_invalidData, L"price"); // NOLINT(*-exception-baseclass)
        }
        if (m_quantity < c_minQuantity || m_quantity > s_maxQuantity) {
            throw DataError(Wcs::c_invalidData, L"quantity"); // NOLINT(*-exception-baseclass)
        }
    }
}
