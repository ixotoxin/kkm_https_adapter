// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "device.h"
#include "variables.h"
#include "strings.h"
#include <log/write.h>

namespace Kkm {
    Device::Device(const std::wstring_view logPrefix)
    : m_kkm {}, m_serialNumber {}, m_logPrefix { logPrefix }, m_lineLength {}, m_needToCancelReceipt { false } {}

    Device::Device(const ConnParams & connParams, std::wstring_view logPrefix)
    : Device(logPrefix) {
        connect(connParams);
    }

    Device::Device(const KnownConnParams & connParams, std::wstring_view logPrefix)
    : Device(logPrefix) {
        connect(connParams);
        if (m_serialNumber != connParams.serialNumber()) {
            throw Failure(KKM_WFMT(Wcs::c_serialNumberMismatch, connParams.serialNumber(), m_serialNumber)); // NOLINT(*-exception-baseclass)
        }
    }

    Device::~Device() {
        if (m_needToCancelReceipt && m_kkm.cancelReceipt() < 0) {
            LOG_WARNING_TS(Wcs::c_cancelingError, m_logPrefix, m_serialNumber, m_kkm.errorDescription());
        }
        m_kkm.close();
    }

    void Device::connect(const ConnParams & connParams) {
        connParams.apply(m_kkm);
        if (m_kkm.open() < 0) {
            throw Failure(m_kkm); // NOLINT(*-exception-baseclass)
        }
        if (!m_kkm.isOpened()) {
            throw Failure(Wcs::c_notAvailable); // NOLINT(*-exception-baseclass)
        }
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DATA_TYPE, Atol::LIBFPTR_DT_SERIAL_NUMBER);
        if (m_kkm.queryData() < 0) {
            throw Failure(m_kkm); // NOLINT(*-exception-baseclass)
        }
        m_serialNumber = m_kkm.getParamString(Atol::LIBFPTR_PARAM_SERIAL_NUMBER);
        if (m_serialNumber.empty()) {
            throw Failure(Wcs::c_exchangeError); // NOLINT(*-exception-baseclass)
        }
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DATA_TYPE, Atol::LIBFPTR_DT_RECEIPT_LINE_LENGTH);
        if (m_kkm.queryData() < 0) {
            throw Failure(m_kkm); // NOLINT(*-exception-baseclass)
        }
        m_lineLength = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_RECEIPT_LINE_LENGTH);
        if (m_lineLength < 1) {
            m_lineLength = s_defaultLineLength;
            LOG_WARNING_TS(Wcs::c_wrongLength, m_logPrefix, m_serialNumber);
        }
    }

    void Device::fail(Result & result, const std::wstring_view message, const std::source_location & location) {
        if (Log::s_appendLocation) {
            LOG_WARNING_TS(KKM_WFMT(Wcs::c_fault, m_logPrefix, m_serialNumber, message) + location);
        } else {
            LOG_WARNING_TS(KKM_WFMT(Wcs::c_fault, m_logPrefix, m_serialNumber, message));
        }
        if (result.m_success) {
            result.m_success = false;
            result.m_message = message;
        }
    }

    void Device::fail(Result & result, const std::wstring & message, const std::source_location & location) {
        if (Log::s_appendLocation) {
            LOG_WARNING_TS(KKM_WFMT(Wcs::c_fault, m_logPrefix, m_serialNumber, message) + location);
        } else {
            LOG_WARNING_TS(KKM_WFMT(Wcs::c_fault, m_logPrefix, m_serialNumber, message));
        }
        if (result.m_success) {
            result.m_success = false;
            result.m_message = message;
        }
    }

    void Device::fail(Result & result, std::wstring && message, const std::source_location & location) {
        if (Log::s_appendLocation) {
            LOG_WARNING_TS(KKM_WFMT(Wcs::c_fault, m_logPrefix, m_serialNumber, message) + location);
        } else {
            LOG_WARNING_TS(KKM_WFMT(Wcs::c_fault, m_logPrefix, m_serialNumber, message));
        }
        if (result.m_success) {
            result.m_success = false;
            result.m_message = std::forward<std::wstring>(message);
        }
    }

    void Device::fail(Result & result, const std::source_location & location) {
        std::wstring message { m_kkm.errorDescription() };
        m_kkm.resetError();
        if (Log::s_appendLocation) {
            LOG_WARNING_TS(KKM_WFMT(Wcs::c_fault, m_logPrefix, m_serialNumber, message) + location);
        } else {
            LOG_WARNING_TS(KKM_WFMT(Wcs::c_fault, m_logPrefix, m_serialNumber, message));
        }
        if (result.m_success) {
            result.m_success = false;
            result.m_message = std::move(message);
        }
    }

    const std::wstring & Device::serialNumber() const {
        return m_serialNumber;
    }

    [[nodiscard, maybe_unused]]
    std::wstring Device::addMargins(std::wstring_view text, int marginTop, int marginBottom) {
        Numeric::doubleClamp(marginTop, marginBottom, 0, 10);
        std::wstring result;
        if (marginTop) {
            result.append(marginTop, L'\n');
        }
        result.append(text);
        if (marginBottom) {
            result.append(marginBottom, L'\n');
        }
        return result;
    }

    [[maybe_unused]]
    void Device::addSeparator(std::wstring & text, int marginTop, int marginBottom) const {
        Numeric::doubleClamp(marginTop, marginBottom, 0, 10);
        ++marginBottom;
        if (marginTop) {
            text.append(marginTop, L'\n');
        }
        text.append(m_lineLength, c_separatorChar);
        if (marginBottom) {
            text.append(marginBottom, L'\n');
        }
    }

    [[nodiscard, maybe_unused]]
    std::wstring Device::addSeparators(const std::wstring_view text, int marginOuter, int marginInner) const {
        std::wstring result;
        Numeric::doubleClamp(marginOuter, marginInner, 0, 10);
        ++marginInner;
        if (marginOuter) {
            result.append(marginOuter, L'\n');
        }
        result.append(m_lineLength, c_separatorChar);
        if (marginInner) {
            result.append(marginInner, L'\n');
        }
        result.append(text);
        if (marginInner) {
            result.append(marginInner, L'\n');
        }
        result.append(m_lineLength, c_separatorChar);
        if (marginOuter) {
            result.append(marginOuter, L'\n');
        }
        return result;
    }

    [[maybe_unused]]
    void Device::subPrintSeparator(int marginTop, int marginBottom) {
        std::wstring separator;
        Numeric::doubleClamp(marginTop, marginBottom, 0, 10);
        if (marginTop) {
            separator.append(marginTop, L'\n');
        }
        separator.append(m_lineLength, c_separatorChar);
        if (marginBottom) {
            separator.append(marginBottom, L'\n');
        }
        m_kkm.setParam(Atol::LIBFPTR_PARAM_ALIGNMENT, Atol::LIBFPTR_ALIGNMENT_CENTER);
        m_kkm.setParam(Atol::LIBFPTR_PARAM_TEXT, separator);
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DEFER, Atol::LIBFPTR_DEFER_NONE);
        if (m_kkm.printText() < 0) {
            throw Failure(m_kkm); // NOLINT(*-exception-baseclass)
        }
    }

    void Device::subPrintText(
        const std::wstring_view content,
        bool center,
        bool magnified,
        bool separated,
        int outerMargin,
        int innerMargin,
        TextPosition position
    ) {
        std::wstring text;
        if (content.empty()) {
            if (separated) {
                center = true;
                magnified = false;
                addSeparator(text, outerMargin, innerMargin);
            } else {
                return;
            }
        } else {
            if (magnified) {
                separated = false;
            }
            if (separated) {
                text.assign(addSeparators(content, outerMargin, innerMargin));
            } else {
                text.assign(addMargins(content, innerMargin));
            }
        }

        /** Печать текста **/
        if (magnified) {
            m_kkm.setParam(Atol::LIBFPTR_PARAM_FONT_DOUBLE_WIDTH, true);
            // m_kkm.setParam(Atol::LIBFPTR_PARAM_FONT_DOUBLE_HEIGHT, true);
        }
        if (center) {
            m_kkm.setParam(Atol::LIBFPTR_PARAM_ALIGNMENT, Atol::LIBFPTR_ALIGNMENT_CENTER);
        }
        m_kkm.setParam(Atol::LIBFPTR_PARAM_TEXT_WRAP, Atol::LIBFPTR_TW_WORDS);
        m_kkm.setParam(Atol::LIBFPTR_PARAM_TEXT, text);
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DEFER, Meta::toUnderlying(position));
        if (m_kkm.printText() < 0) {
            throw Failure(m_kkm); // NOLINT(*-exception-baseclass)
        }
    }

    void Device::subPrintText(const PrintableText & text, const TextPosition position) {
        subPrintText(
            text.m_content, text.m_center, text.m_magnified, text.m_separated,
            text.m_marginOuter, text.m_marginInner, position
        );
    }

    void Device::subCheckDocumentClosed(Result & result) {
        assert(s_documentClosingTimeout >= c_sleepQuantum);
        // ISSUE: Из документации не очень понятно как работать с методом checkDocumentClosed() - описания нет,
        //  приведенный пример выглядит странно и рассчитан скорее всего на интерактивное взаимодействие с ККМ.
        //  В нашем случае интерактивность невозможна. Будем ждать чуда. Если чуда не произойдет, отменяем чек.
        DateTime::SleepUnit::rep i;
        for (i = s_documentClosingTimeout / c_sleepQuantum; m_kkm.checkDocumentClosed() < 0 && i; --i) {
            LOG_WARNING_TS(Wcs::c_closingError, m_logPrefix, m_serialNumber, m_kkm.errorDescription());
            std::this_thread::sleep_for(c_sleepQuantum);
        }
        if (!i || !m_kkm.getParamBool(Atol::LIBFPTR_PARAM_DOCUMENT_CLOSED)) {
            if (m_kkm.cancelReceipt() < 0) {
                LOG_WARNING_TS(Wcs::c_cancelingError, m_logPrefix, m_serialNumber, m_kkm.errorDescription());
            }
            return fail(result, Wcs::c_checkingError);
        }
        if (!m_kkm.getParamBool(Atol::LIBFPTR_PARAM_DOCUMENT_PRINTED)) {
            for (i = s_documentClosingTimeout / c_sleepQuantum; m_kkm.continuePrint() < 0 && i; --i) {
                LOG_WARNING_TS(Wcs::c_printingError, m_logPrefix, m_serialNumber, m_kkm.errorDescription());
                std::this_thread::sleep_for(c_sleepQuantum);
            }
            if (!i) {
                return fail(result, Wcs::c_checkingError);
            }
        }
    }

    void Device::subSetOperator(const OperatorDetails & details) {
        LOG_DEBUG_TS(Wcs::c_subSetOperator, m_logPrefix, m_serialNumber);

        /** Регистрация оператора **/
        m_kkm.setParam(1021, details.m_operatorName);
        if (!details.m_operatorInn.empty()) {
            m_kkm.setParam(1203, details.m_operatorInn);
        }
        if (m_kkm.operatorLogin() < 0) {
            throw Failure(m_kkm); // NOLINT(*-exception-baseclass)
        }
    }

    void Device::subSetCustomer(const ReceiptDetails & details) {
        LOG_DEBUG_TS(Wcs::c_subSetCustomer, m_logPrefix, m_serialNumber);

        /** Регистрация информации о покупателе / клиенте для ФФД >= 1.2 **/
        bool hasRequisite1256 { false };
        if (!details.m_customerName.empty()) {
            m_kkm.setParam(1227, details.m_customerName);
            hasRequisite1256 = true;
        }
        if (!details.m_customerInn.empty()) {
            m_kkm.setParam(1228, details.m_customerInn);
            hasRequisite1256 = true;
        }
        if (!details.m_customerBirthdate.empty()) {
            m_kkm.setParam(1243, details.m_customerBirthdate);
            hasRequisite1256 = true;
        }
        if (!details.m_customerCitizenship.empty()) {
            m_kkm.setParam(1244, details.m_customerCitizenship);
            hasRequisite1256 = true;
        }
        if (!details.m_customerDocumentCode.empty()) {
            m_kkm.setParam(1245, details.m_customerDocumentCode);
            hasRequisite1256 = true;
        }
        if (!details.m_customerDocumentData.empty()) {
            m_kkm.setParam(1246, details.m_customerDocumentData);
            hasRequisite1256 = true;
        }
        if (!details.m_customerAddress.empty()) {
            m_kkm.setParam(1254, details.m_customerAddress);
            hasRequisite1256 = true;
        }
        if (hasRequisite1256) {
            if (m_kkm.utilFormTlv() < 0) {
                throw Failure(m_kkm); // NOLINT(*-exception-baseclass)
            }
            std::vector<uchar> clientInfo = m_kkm.getParamByteArray(Atol::LIBFPTR_PARAM_TAG_VALUE);
            m_kkm.setParam(1256, clientInfo);
        }

        /** Регистрация номера лицевого счёта покупателя **/
        if (!details.m_customerAccount.empty()) {
            std::wstring customerAccount { L"  " };
            customerAccount.append(details.m_customerAccount);
            m_kkm.setParam(1085, s_customerAccountField); // Наименование дополнительного реквизита пользователя
            m_kkm.setParam(1086, customerAccount); // Значение дополнительного реквизита пользователя
            if (m_kkm.utilFormTlv() < 0) {
                throw Failure(m_kkm); // NOLINT(*-exception-baseclass)
            }
            std::vector<uchar> clientInfo = m_kkm.getParamByteArray(Atol::LIBFPTR_PARAM_TAG_VALUE);
            m_kkm.setParam(1084, clientInfo);
        }

        /** Регистрация информации о покупателе / клиенте для ФФД < 1.2 **/
        if (!details.m_customerName.empty()) {
            m_kkm.setParam(1227, details.m_customerName);
        }
        if (!details.m_customerInn.empty()) {
            m_kkm.setParam(1228, details.m_customerInn);
        }

        /** Регистрация информации о покупателе / клиенте **/
        if (!details.m_customerContact.empty()) {
            m_kkm.setParam(1008, details.m_customerContact);
        }
    }

    void Device::subSetSeller(const ReceiptDetails & details) {
        LOG_DEBUG_TS(Wcs::c_subSetSeller, m_logPrefix, m_serialNumber);

        /** Регистрация информации о продавце / поставщике **/
        if (!details.m_customerInn.empty()) {
            m_kkm.setParam(1117, details.m_sellerEmail);
        }
    }

    void Device::getStatus(StatusResult & result) {
        LOG_DEBUG_TS(Wcs::c_statusMethod, m_logPrefix, m_serialNumber);

        /** Запрос информации о ККТ **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DATA_TYPE, Atol::LIBFPTR_DT_STATUS);
        if (m_kkm.queryData() < 0) {
            return fail(result);
        }
        result.m_blocked = m_kkm.getParamBool(Atol::LIBFPTR_PARAM_BLOCKED);
        result.m_cashDrawerOpened = m_kkm.getParamBool(Atol::LIBFPTR_PARAM_CASHDRAWER_OPENED);
        result.m_coverOpened = m_kkm.getParamBool(Atol::LIBFPTR_PARAM_COVER_OPENED);
        result.m_cutError = m_kkm.getParamBool(Atol::LIBFPTR_PARAM_CUT_ERROR);
        result.m_dateTime = m_kkm.getParamDateTime(Atol::LIBFPTR_PARAM_DATE_TIME);
        result.m_documentNumber = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_DOCUMENT_NUMBER);
        result.m_documentType = static_cast<DocumentType>(m_kkm.getParamInt(Atol::LIBFPTR_PARAM_DOCUMENT_TYPE));
        result.m_fiscal = m_kkm.getParamBool(Atol::LIBFPTR_PARAM_FISCAL);
        result.m_fnFiscal = m_kkm.getParamBool(Atol::LIBFPTR_PARAM_FN_FISCAL);
        result.m_fnPresent = m_kkm.getParamBool(Atol::LIBFPTR_PARAM_FN_PRESENT);
        result.m_invalidFn = m_kkm.getParamBool(Atol::LIBFPTR_PARAM_INVALID_FN);
        result.m_logicalNumber = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_LOGICAL_NUMBER);
        result.m_mode = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_MODE);
        result.m_model = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_MODEL);
        result.m_modelName.assign(m_kkm.getParamString(Atol::LIBFPTR_PARAM_MODEL_NAME));
        result.m_operatorId = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_OPERATOR_ID);
        result.m_operatorRegistered = m_kkm.getParamBool(Atol::LIBFPTR_PARAM_OPERATOR_REGISTERED);
        result.m_paperNearEnd = m_kkm.getParamBool(Atol::LIBFPTR_PARAM_PAPER_NEAR_END);
        result.m_printerConnectionLost = m_kkm.getParamBool(Atol::LIBFPTR_PARAM_PRINTER_CONNECTION_LOST);
        result.m_printerError = m_kkm.getParamBool(Atol::LIBFPTR_PARAM_PRINTER_ERROR);
        result.m_printerOverheat = m_kkm.getParamBool(Atol::LIBFPTR_PARAM_PRINTER_OVERHEAT);
        result.m_receiptLineLength = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_RECEIPT_LINE_LENGTH);
        result.m_receiptLineLengthPix = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_RECEIPT_LINE_LENGTH_PIX);
        result.m_receiptNumber = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_RECEIPT_NUMBER);
        result.m_receiptPaperPresent = m_kkm.getParamBool(Atol::LIBFPTR_PARAM_RECEIPT_PAPER_PRESENT);
        result.m_receiptSum = m_kkm.getParamDouble(Atol::LIBFPTR_PARAM_RECEIPT_SUM);
        result.m_receiptType = static_cast<ReceiptType>(m_kkm.getParamInt(Atol::LIBFPTR_PARAM_RECEIPT_TYPE));
        result.m_serialNumber.assign(m_kkm.getParamString(Atol::LIBFPTR_PARAM_SERIAL_NUMBER));
        result.m_shiftNumber = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_SHIFT_NUMBER);
        result.m_shiftState = static_cast<ShiftState>(m_kkm.getParamInt(Atol::LIBFPTR_PARAM_SHIFT_STATE));
        result.m_subMode = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_SUBMODE);
    }

    void Device::getShiftState(ShiftStateResult & result) {
        LOG_DEBUG_TS(Wcs::c_shiftStateMethod, m_logPrefix, m_serialNumber);

        /** Запрос состояния смены **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DATA_TYPE, Atol::LIBFPTR_DT_SHIFT_STATE);
        if (m_kkm.queryData() < 0) {
            return fail(result);
        }
        result.m_shiftNumber = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_SHIFT_NUMBER);
        result.m_shiftState = static_cast<ShiftState>(m_kkm.getParamInt(Atol::LIBFPTR_PARAM_SHIFT_STATE));
        result.m_expirationDateTime = m_kkm.getParamDateTime(Atol::LIBFPTR_PARAM_DATE_TIME);

        /** Запрос информации о текущей смене в ФН **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_FN_DATA_TYPE, Atol::LIBFPTR_FNDT_SHIFT);
        if (m_kkm.fnQueryData() < 0) {
            return fail(result);
        }
        result.m_receiptNumber = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_RECEIPT_NUMBER);
        auto shiftNumber = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_SHIFT_NUMBER);
        // ISSUE: Проверка дублирующейся информации. Не факт, что она должна совпадать.
        if (shiftNumber != result.m_shiftNumber) {
            // throw Failure(Wcs::c_invalidData); // NOLINT(*-exception-baseclass)
            LOG_WARNING_TS(Wcs::c_shiftMismatch, m_logPrefix, m_serialNumber);
        }

        /** Запрос количества ФД за смену **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_FN_DATA_TYPE, Atol::LIBFPTR_FNDT_DOCUMENTS_COUNT_IN_SHIFT);
        if (m_kkm.fnQueryData() < 0) {
            return fail(result);
        }
        result.m_documentsCount = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_DOCUMENTS_COUNT);
    }

    void Device::getReceiptState(ReceiptStateResult & result) {
        LOG_DEBUG_TS(Wcs::c_receiptStateMethod, m_logPrefix, m_serialNumber);

        /** Запрос состояния чека **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DATA_TYPE, Atol::LIBFPTR_DT_RECEIPT_STATE);
        if (m_kkm.queryData() < 0) {
            return fail(result);
        }
        result.m_receiptType = static_cast<ReceiptType>(m_kkm.getParamInt(Atol::LIBFPTR_PARAM_RECEIPT_TYPE));
        result.m_receiptNumber = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_RECEIPT_NUMBER);
        result.m_documentNumber = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_DOCUMENT_NUMBER);
        result.m_sum = m_kkm.getParamDouble(Atol::LIBFPTR_PARAM_RECEIPT_SUM);
        result.m_remainder = m_kkm.getParamDouble(Atol::LIBFPTR_PARAM_REMAINDER);
        result.m_change = m_kkm.getParamDouble(Atol::LIBFPTR_PARAM_CHANGE);
    }

    void Device::getCashStat(CashStatResult & result) {
        LOG_DEBUG_TS(Wcs::c_cashStatMethod, m_logPrefix, m_serialNumber);

        /** Запрос суммы наличных платежей в чеках прихода (продажи) **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DATA_TYPE, Atol::LIBFPTR_DT_PAYMENT_SUM);
        m_kkm.setParam(Atol::LIBFPTR_PARAM_PAYMENT_TYPE, Atol::LIBFPTR_PT_CASH);
        m_kkm.setParam(Atol::LIBFPTR_PARAM_RECEIPT_TYPE, Atol::LIBFPTR_RT_SELL);
        if (m_kkm.queryData() < 0) {
            return fail(result);
        }
        result.m_sellCashSum = m_kkm.getParamDouble(Atol::LIBFPTR_PARAM_SUM);

        /** Запрос суммы наличных платежей в чеках возврата прихода (продажи) **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DATA_TYPE, Atol::LIBFPTR_DT_PAYMENT_SUM);
        m_kkm.setParam(Atol::LIBFPTR_PARAM_PAYMENT_TYPE, Atol::LIBFPTR_PT_CASH);
        m_kkm.setParam(Atol::LIBFPTR_PARAM_RECEIPT_TYPE, Atol::LIBFPTR_RT_SELL_RETURN);
        if (m_kkm.queryData() < 0) {
            return fail(result);
        }
        result.m_sellReturnCashSum = m_kkm.getParamDouble(Atol::LIBFPTR_PARAM_SUM);

        /** Запрос суммы внесений **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DATA_TYPE, Atol::LIBFPTR_DT_CASHIN_SUM);
        if (m_kkm.queryData() < 0) {
            return fail(result);
        }
        result.m_cashInSum = m_kkm.getParamDouble(Atol::LIBFPTR_PARAM_SUM);

        /** Запрос суммы выплат **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DATA_TYPE, Atol::LIBFPTR_DT_CASHOUT_SUM);
        if (m_kkm.queryData() < 0) {
            return fail(result);
        }
        result.m_cashOutSum = m_kkm.getParamDouble(Atol::LIBFPTR_PARAM_SUM);

        /** Запрос количества внесений **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DATA_TYPE, Atol::LIBFPTR_DT_CASHIN_COUNT);
        if (m_kkm.queryData() < 0) {
            return fail(result);
        }
        result.m_cashInCount = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_DOCUMENTS_COUNT);

        /** Запрос количества выплат **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DATA_TYPE, Atol::LIBFPTR_DT_CASHOUT_COUNT);
        if (m_kkm.queryData() < 0) {
            return fail(result);
        }
        result.m_cashOutCount = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_DOCUMENTS_COUNT);

        /** Запрос суммы наличных в денежном ящике **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DATA_TYPE, Atol::LIBFPTR_DT_CASH_SUM);
        if (m_kkm.queryData() < 0) {
            return fail(result);
        }
        result.m_cashSum = m_kkm.getParamDouble(Atol::LIBFPTR_PARAM_SUM);
    }

    void Device::getFndtOfdExchangeStatus(FndtOfdExchangeStatusResult & result) {
        LOG_DEBUG_TS(Wcs::c_ofdExchangeStatusMethod, m_logPrefix, m_serialNumber);

        /** Запрос статуса информационного обмена с ОФД **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_FN_DATA_TYPE, Atol::LIBFPTR_FNDT_OFD_EXCHANGE_STATUS);
        if (m_kkm.fnQueryData() < 0) {
            return fail(result);
        }
        result.m_exchangeStatus = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_OFD_EXCHANGE_STATUS);
        result.m_unsentCount = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_DOCUMENTS_COUNT);
        result.m_firstUnsentNumber = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_DOCUMENT_NUMBER);
        result.m_ofdMessageRead = m_kkm.getParamBool(Atol::LIBFPTR_PARAM_OFD_MESSAGE_READ);
        result.m_firstUnsentDateTime = m_kkm.getParamDateTime(Atol::LIBFPTR_PARAM_DATE_TIME);
        result.m_okpDateTime = m_kkm.getParamDateTime(Atol::LIBFPTR_PARAM_LAST_SUCCESSFUL_OKP);

        /** Запрос даты и времени последней успешной отправки документа в ОФД **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DATA_TYPE, Atol::LIBFPTR_DT_LAST_SENT_OFD_DOCUMENT_DATE_TIME);
        if (m_kkm.queryData() < 0) {
            return fail(result);
        }
        result.m_lastSentDateTime = m_kkm.getParamDateTime(Atol::LIBFPTR_PARAM_DATE_TIME);
    }

    void Device::getFndtFnInfo(FndtFnInfoResult & result) {
        LOG_DEBUG_TS(Wcs::c_fnInfoMethod, m_logPrefix, m_serialNumber);

        /** Запрос информации и статуса ФН **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_FN_DATA_TYPE, Atol::LIBFPTR_FNDT_FN_INFO);
        if (m_kkm.fnQueryData() < 0) {
            return fail(result);
        }
        result.m_serial = m_kkm.getParamString(Atol::LIBFPTR_PARAM_SERIAL_NUMBER);
        result.m_version = m_kkm.getParamString(Atol::LIBFPTR_PARAM_FN_VERSION);
        result.m_execution = m_kkm.getParamString(Atol::LIBFPTR_PARAM_FN_EXECUTION);
        result.m_type = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_FN_TYPE);
        result.m_state = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_FN_STATE);
        result.m_flags = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_FN_FLAGS);
        result.m_needReplacement = m_kkm.getParamBool(Atol::LIBFPTR_PARAM_FN_NEED_REPLACEMENT);
        result.m_exhausted = m_kkm.getParamBool(Atol::LIBFPTR_PARAM_FN_RESOURCE_EXHAUSTED);
        result.m_memoryOverflow = m_kkm.getParamBool(Atol::LIBFPTR_PARAM_FN_MEMORY_OVERFLOW);
        result.m_ofdTimeout = m_kkm.getParamBool(Atol::LIBFPTR_PARAM_FN_OFD_TIMEOUT);
        result.m_criticalError = m_kkm.getParamBool(Atol::LIBFPTR_PARAM_FN_CRITICAL_ERROR);
        if (m_kkm.getParamBool(Atol::LIBFPTR_PARAM_FN_CONTAINS_KEYS_UPDATER_SERVER_URI)) {
            result.m_keysUpdaterServerUri = m_kkm.getParamString(Atol::LIBFPTR_PARAM_FN_KEYS_UPDATER_SERVER_URI);
        }
    }

    void Device::getFndtRegistrationInfo(FndtRegistrationInfoResult & result) {
        LOG_DEBUG_TS(Wcs::c_registrationInfoMethod, m_logPrefix, m_serialNumber);

        /** Запрос реквизитов регистрации ККТ **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_FN_DATA_TYPE, Atol::LIBFPTR_FNDT_REG_INFO);
        if (m_kkm.fnQueryData() < 0) {
            return fail(result);
        }

        result.m_fnsUrl = m_kkm.getParamString(1060);
        result.m_organizationAddress = m_kkm.getParamString(1009);
        result.m_organizationVATIN = m_kkm.getParamString(1018);
        result.m_organizationName = m_kkm.getParamString(1048);
        result.m_organizationEmail = m_kkm.getParamString(1117);
        result.m_paymentsAddress = m_kkm.getParamString(1187);
        result.m_registrationNumber = m_kkm.getParamString(1037);
        result.m_machineNumber = m_kkm.getParamString(1036);
        result.m_ofdVATIN = m_kkm.getParamString(1017);
        result.m_ofdName = m_kkm.getParamString(1046);
        result.m_taxationTypes = m_kkm.getParamInt(1062);
        result.m_agentSign = m_kkm.getParamInt(1057);
        result.m_ffdVersion = static_cast<FfdVersion>(m_kkm.getParamInt(1209));
        result.m_autoModeSign = m_kkm.getParamBool(1001);
        result.m_offlineModeSign = m_kkm.getParamBool(1002);
        result.m_encryptionSign = m_kkm.getParamBool(1056);
        result.m_internetSign = m_kkm.getParamBool(1108);
        result.m_serviceSign = m_kkm.getParamBool(1109);
        result.m_bsoSign = m_kkm.getParamBool(1110);
        result.m_lotterySign = m_kkm.getParamBool(1126);
        result.m_gamblingSign = m_kkm.getParamBool(1193);
        result.m_exciseSign = m_kkm.getParamBool(1207);
        result.m_machineInstallationSign = m_kkm.getParamBool(1221);
        result.m_tradeMarkedProducts = m_kkm.getParamBool(Atol::LIBFPTR_PARAM_TRADE_MARKED_PRODUCTS);
        result.m_insuranceActivity = m_kkm.getParamBool(Atol::LIBFPTR_PARAM_INSURANCE_ACTIVITY);
        result.m_pawnShopActivity = m_kkm.getParamBool(Atol::LIBFPTR_PARAM_PAWN_SHOP_ACTIVITY);
        result.m_vending = m_kkm.getParamBool(Atol::LIBFPTR_PARAM_VENDING);
        result.m_catering = m_kkm.getParamBool(Atol::LIBFPTR_PARAM_CATERING);
        result.m_wholesale = m_kkm.getParamBool(Atol::LIBFPTR_PARAM_WHOLESALE);
    }

    void Device::getFndtLastRegistration(FndtLastRegistrationResult & result) {
        LOG_DEBUG_TS(Wcs::c_lastRegistrationMethod, m_logPrefix, m_serialNumber);

        /** Запрос информации о последней регистрации / перерегистрации **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_FN_DATA_TYPE, Atol::LIBFPTR_FNDT_LAST_REGISTRATION);
        if (m_kkm.fnQueryData() < 0) {
            return fail(result);
        }
        result.m_documentNumber = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_DOCUMENT_NUMBER);
        result.m_registrationsCount = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_REGISTRATIONS_COUNT);
        result.m_registrationDateTime = m_kkm.getParamDateTime(Atol::LIBFPTR_PARAM_DATE_TIME);
    }

    void Device::getFndtLastReceipt(FndtLastReceiptResult & result) {
        LOG_DEBUG_TS(Wcs::c_lastReceiptMethod, m_logPrefix, m_serialNumber);

        /** Запрос информации о последнем чеке **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_FN_DATA_TYPE, Atol::LIBFPTR_FNDT_LAST_RECEIPT);
        if (m_kkm.fnQueryData() < 0) {
            return fail(result);
        }
        result.m_documentNumber = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_DOCUMENT_NUMBER);
        result.m_receiptSum = m_kkm.getParamDouble(Atol::LIBFPTR_PARAM_RECEIPT_SUM);
        result.m_fiscalSign = m_kkm.getParamString(Atol::LIBFPTR_PARAM_FISCAL_SIGN);
        result.m_documentDateTime = m_kkm.getParamDateTime(Atol::LIBFPTR_PARAM_DATE_TIME);
    }

    void Device::getFndtLastDocument(FndtLastDocumentResult & result) {
        LOG_DEBUG_TS(Wcs::c_lastDocumentMethod, m_logPrefix, m_serialNumber);

        /** Запрос информации о последнем фискальном документе **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_FN_DATA_TYPE, Atol::LIBFPTR_FNDT_LAST_DOCUMENT);
        if (m_kkm.fnQueryData() < 0) {
            return fail(result);
        }
        result.m_documentNumber = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_DOCUMENT_NUMBER);
        result.m_fiscalSign = m_kkm.getParamString(Atol::LIBFPTR_PARAM_FISCAL_SIGN);
        result.m_documentDateTime = m_kkm.getParamDateTime(Atol::LIBFPTR_PARAM_DATE_TIME);
    }

    void Device::getFndtErrors(FndtErrorsResult & result) {
        LOG_DEBUG_TS(Wcs::c_errorsMethod, m_logPrefix, m_serialNumber);

        /** Запрос ошибок обмена с ОФД **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_FN_DATA_TYPE, Atol::LIBFPTR_FNDT_ERRORS);
        if (m_kkm.fnQueryData() < 0) {
            return fail(result);
        }
        result.m_networkError = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_NETWORK_ERROR);
        result.m_networkErrorText = m_kkm.getParamString(Atol::LIBFPTR_PARAM_NETWORK_ERROR_TEXT);
        result.m_ofdError = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_OFD_ERROR);
        result.m_ofdErrorText = m_kkm.getParamString(Atol::LIBFPTR_PARAM_OFD_ERROR_TEXT);
        result.m_fnError = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_FN_ERROR);
        result.m_fnErrorText = m_kkm.getParamString(Atol::LIBFPTR_PARAM_FN_ERROR_TEXT);
        result.m_documentNumber = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_DOCUMENT_NUMBER);
        result.m_commandCode = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_COMMAND_CODE);
        result.m_successDateTime = m_kkm.getParamDateTime(Atol::LIBFPTR_PARAM_DATE_TIME);
        result.m_dataForSendIsEmpty = m_kkm.getParamBool(Atol::LIBFPTR_PARAM_DATA_FOR_SEND_IS_EMPTY);
    }

    void Device::getFfdVersion(FfdVersionResult & result) {
        LOG_DEBUG_TS(Wcs::c_ffdVersionMethod, m_logPrefix, m_serialNumber);

        /** Запрос версий ФФД **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_FN_DATA_TYPE, Atol::LIBFPTR_FNDT_FFD_VERSIONS);
        if (m_kkm.fnQueryData() < 0) {
            return fail(result);
        }

        result.m_deviceFfdVersion = static_cast<FfdVersion>(m_kkm.getParamInt(Atol::LIBFPTR_PARAM_DEVICE_FFD_VERSION));
        result.m_devMinFfdVersion = static_cast<FfdVersion>(m_kkm.getParamInt(Atol::LIBFPTR_PARAM_DEVICE_MIN_FFD_VERSION));
        result.m_devMaxFfdVersion = static_cast<FfdVersion>(m_kkm.getParamInt(Atol::LIBFPTR_PARAM_DEVICE_MAX_FFD_VERSION));
        result.m_fnFfdVersion = static_cast<FfdVersion>(m_kkm.getParamInt(Atol::LIBFPTR_PARAM_FN_FFD_VERSION));
        result.m_fnMaxFfdVersion = static_cast<FfdVersion>(m_kkm.getParamInt(Atol::LIBFPTR_PARAM_FN_MAX_FFD_VERSION));
        result.m_ffdVersion = static_cast<FfdVersion>(m_kkm.getParamInt(Atol::LIBFPTR_PARAM_FFD_VERSION));
        // result.m_kktVersion = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_VERSION);
    }

    void Device::getFwVersion(FwVersionResult & result) {
        LOG_DEBUG_TS(Wcs::c_fwVersionMethod, m_logPrefix, m_serialNumber);

        /** Запрос версии прошивки **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DATA_TYPE, Atol::LIBFPTR_DT_UNIT_VERSION);
        m_kkm.setParam(Atol::LIBFPTR_PARAM_UNIT_TYPE, Atol::LIBFPTR_UT_FIRMWARE);
        if (m_kkm.queryData() < 0) {
            return fail(result);
        }
        result.m_firmwareVersion.assign(m_kkm.getParamString(Atol::LIBFPTR_PARAM_UNIT_VERSION));

        /** Запрос версии конфигурации **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DATA_TYPE, Atol::LIBFPTR_DT_UNIT_VERSION);
        m_kkm.setParam(Atol::LIBFPTR_PARAM_UNIT_TYPE, Atol::LIBFPTR_UT_CONFIGURATION);
        if (m_kkm.queryData() < 0) {
            return fail(result);
        }
        result.m_configurationVersion.assign(m_kkm.getParamString(Atol::LIBFPTR_PARAM_UNIT_VERSION));
        result.m_releaseVersion.assign(m_kkm.getParamString(Atol::LIBFPTR_PARAM_UNIT_RELEASE_VERSION));

        /** Запрос версии движка шаблонов **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DATA_TYPE, Atol::LIBFPTR_DT_UNIT_VERSION);
        m_kkm.setParam(Atol::LIBFPTR_PARAM_UNIT_TYPE, Atol::LIBFPTR_UT_TEMPLATES);
        if (m_kkm.queryData() < 0) {
            return fail(result);
        }
        result.m_templatesVersion.assign(m_kkm.getParamString(Atol::LIBFPTR_PARAM_UNIT_VERSION));

        /** Запрос версии блока управления **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DATA_TYPE, Atol::LIBFPTR_DT_UNIT_VERSION);
        m_kkm.setParam(Atol::LIBFPTR_PARAM_UNIT_TYPE, Atol::LIBFPTR_UT_CONTROL_UNIT);
        if (m_kkm.queryData() < 0) {
            return fail(result);
        }
        result.m_controlUnitVersion.assign(m_kkm.getParamString(Atol::LIBFPTR_PARAM_UNIT_VERSION));

        /** Запрос версии загрузчика **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DATA_TYPE, Atol::LIBFPTR_DT_UNIT_VERSION);
        m_kkm.setParam(Atol::LIBFPTR_PARAM_UNIT_TYPE, Atol::LIBFPTR_UT_BOOT);
        if (m_kkm.queryData() < 0) {
            return fail(result);
        }
        result.m_bootVersion.assign(m_kkm.getParamString(Atol::LIBFPTR_PARAM_UNIT_VERSION));
    }

    void Device::printHello() {
        LOG_DEBUG_TS(Wcs::c_printHelloMethod, m_logPrefix, m_serialNumber);

        /** Открытие нефискального документа **/
        if (m_kkm.beginNonfiscalDocument() < 0) {
            throw Failure(m_kkm); // NOLINT(*-exception-baseclass)
        }

        subPrintSeparator(0, 1);
        subPrintText(Wcs::c_helloTitle, true, true);
        subPrintText(Wcs::c_helloText, true);
        subPrintSeparator(1, 0);

        LOG_DEBUG_TS(Wcs::c_subPrint, m_logPrefix, m_serialNumber);

        /** Закрытие нефискального документа без печати подвала **/
        if (m_kkm.endNonfiscalDocument() < 0) {
            throw Failure(m_kkm); // NOLINT(*-exception-baseclass)
        }
    }

    void Device::printDemo(Result & result) {
        LOG_DEBUG_TS(Wcs::c_printDemoMethod, m_logPrefix, m_serialNumber);
        LOG_DEBUG_TS(Wcs::c_subPrint, m_logPrefix, m_serialNumber);

        /** Демо-печать **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_REPORT_TYPE, Atol::LIBFPTR_RT_KKT_DEMO);
        if (m_kkm.report() < 0) {
            result.m_success = false;
            result.m_message.assign(m_kkm.errorDescription());
        }
    }

    void Device::printNonFiscalDocument(const PrintDetails & details, Result & result) {
        LOG_DEBUG_TS(Wcs::c_printNfDocumentMethod, m_logPrefix, m_serialNumber);

        /** Открытие нефискального документа **/
        if (m_kkm.beginNonfiscalDocument() < 0) {
            return fail(result);
        }
        if (details.m_cliche && m_kkm.printCliche() < 0) {
            return fail(result);
        }

        for (auto & block : details.m_document) {
            subPrintText(block);
        }

        LOG_DEBUG_TS(Wcs::c_subPrint, m_logPrefix, m_serialNumber);

        /** Закрытие нефискального документа **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_PRINT_FOOTER, details.m_footer);
        if (m_kkm.endNonfiscalDocument() < 0) {
            return fail(result);
        }
    }

    void Device::printInfo(Result & result) {
        LOG_DEBUG_TS(Wcs::c_printInfoMethod, m_logPrefix, m_serialNumber);
        LOG_DEBUG_TS(Wcs::c_subPrint, m_logPrefix, m_serialNumber);

        /** Печать информации о ККТ **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_REPORT_TYPE, Atol::LIBFPTR_RT_KKT_INFO);
        if (m_kkm.report() < 0) {
            return fail(result);
        }
    }

    void Device::printFnRegistrations(Result & result) {
        LOG_DEBUG_TS(Wcs::c_printFnRegistrationsMethod, m_logPrefix, m_serialNumber);
        LOG_DEBUG_TS(Wcs::c_subPrint, m_logPrefix, m_serialNumber);

        /** Печать итогов регистрации / перерегистрации **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_REPORT_TYPE, Atol::LIBFPTR_RT_FN_REGISTRATIONS);
        if (m_kkm.report() < 0) {
            return fail(result);
        }
    }

    void Device::printOfdExchangeStatus(Result & result) {
        LOG_DEBUG_TS(Wcs::c_printOfdExchangeStatusMethod, m_logPrefix, m_serialNumber);
        LOG_DEBUG_TS(Wcs::c_subPrint, m_logPrefix, m_serialNumber);

        /** Отчет о состоянии расчетов **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_REPORT_TYPE, Atol::LIBFPTR_RT_OFD_EXCHANGE_STATUS);
        if (m_kkm.report() < 0) {
            return fail(result);
        }
        subCheckDocumentClosed(result);
    }

    void Device::printOfdTest(Result & result) {
        LOG_DEBUG_TS(Wcs::c_printOfdTestMethod, m_logPrefix, m_serialNumber);
        LOG_DEBUG_TS(Wcs::c_subPrint, m_logPrefix, m_serialNumber);

        /** Диагностика соединения с ОФД **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_REPORT_TYPE, Atol::LIBFPTR_RT_OFD_TEST);
        if (m_kkm.report() < 0) {
            return fail(result);
        }
    }

    void Device::printCloseShiftReports(Result & result) {
        LOG_DEBUG_TS(Wcs::c_printShiftReportsMethod, m_logPrefix, m_serialNumber);
        LOG_DEBUG_TS(Wcs::c_subPrint, m_logPrefix, m_serialNumber);

        /** Печать нераспечатанных отчетов о закрытии смены **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_REPORT_TYPE, Atol::LIBFPTR_RT_CLOSE_SHIFT_REPORTS);
        if (m_kkm.report() < 0) {
            return fail(result);
        }
    }

    void Device::printLastDocument(Result & result) {
        LOG_DEBUG_TS(Wcs::c_printLastDocumentMethod, m_logPrefix, m_serialNumber);
        LOG_DEBUG_TS(Wcs::c_subPrint, m_logPrefix, m_serialNumber);

        /** Копия последнего документа **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_REPORT_TYPE, Atol::LIBFPTR_RT_LAST_DOCUMENT);
        if (m_kkm.report() < 0) {
            return fail(result);
        }
    }

    void Device::registerCashIn(const CashDetails & details, Result & result) {
        LOG_DEBUG_TS(Wcs::c_cashInMethod, m_logPrefix, m_serialNumber);

        subSetOperator(details);

        LOG_DEBUG_TS(Wcs::c_subRegisterCashInAndPrint, m_logPrefix, m_serialNumber);

        /** Внесение **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_SUM, details.m_cashSum);
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DOCUMENT_ELECTRONICALLY, details.m_electronically);
        if (m_kkm.cashIncome() < 0) {
            return fail(result);
        }
    }

    void Device::registerCashOut(const CashDetails & details, Result & result) {
        LOG_DEBUG_TS(Wcs::c_cashOutMethod, m_logPrefix, m_serialNumber);

        subSetOperator(details);

        LOG_DEBUG_TS(Wcs::c_subRegisterCashOutAndPrint, m_logPrefix, m_serialNumber);

        /** Выплата **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_SUM, details.m_cashSum);
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DOCUMENT_ELECTRONICALLY, details.m_electronically);
        if (m_kkm.cashOutcome() < 0) {
            return fail(result);
        }
    }

    void Device::subRegisterReceipt(
        const ReceiptType type,
        const ReceiptDetails & details,
        Result & result
    ) {
        if (type != ReceiptType::Sell && type != ReceiptType::SellReturn) {
            throw Failure(Wcs::c_notImplemented); // NOLINT(*-exception-baseclass)
        }

        if (details.m_items.empty()) {
            throw Failure(Wcs::c_requiresItems); // NOLINT(*-exception-baseclass)
        }

        /** Печать текста после клише **/
        if (details.m_headerText) {
            subPrintText(details.m_headerText, TextPosition::Pre);
        }

        /** Открытие чека **/
        subSetOperator(details);
        if (details.m_customerDataIsPresent) {
            subSetCustomer(details);
        }
        if (details.m_sellerDataIsPresent) {
            subSetSeller(details);
        }
        m_kkm.setParam(Atol::LIBFPTR_PARAM_RECEIPT_TYPE, Meta::toUnderlying(type));
        m_kkm.setParam(Atol::LIBFPTR_PARAM_RECEIPT_ELECTRONICALLY, details.m_electronically); // Открытие электрочека
        if (m_kkm.openReceipt() < 0) {
            return fail(result);
        }
        m_needToCancelReceipt = true;

        /** Печать текста внутри чека **/
        bool topSeparator { true };
        if (details.m_text) {
            subPrintText(details.m_text, TextPosition::Auto);
            topSeparator = !details.m_text.m_separated || details.m_text.m_magnified;
        }
        if (topSeparator) {
            subPrintSeparator(0);
        }
        if (type == ReceiptType::Sell) {
            subPrintText(Wcs::c_typeSell, false, false, false, 0, 0);
        } else if (type == ReceiptType::SellReturn) {
            subPrintText(Wcs::c_typeSellReturn, false, false, false, 0, 0);
        }
        subPrintSeparator(0);

        /** Печать текста после подвала **/
        if (details.m_footerText) {
            subPrintText(details.m_footerText, TextPosition::Post);
        }

        LOG_DEBUG_TS(Wcs::c_subRegisterItems, m_logPrefix, m_serialNumber);

        /** Регистрация позиций **/
        for (auto & item : details.m_items) {
            m_kkm.setParam(Atol::LIBFPTR_PARAM_COMMODITY_NAME, item.m_commodityName);
            m_kkm.setParam(Atol::LIBFPTR_PARAM_PRICE, item.m_price);
            m_kkm.setParam(Atol::LIBFPTR_PARAM_QUANTITY, item.m_quantity);
            m_kkm.setParam(Atol::LIBFPTR_PARAM_MEASUREMENT_UNIT, Meta::toUnderlying(item.m_unit));
            m_kkm.setParam(Atol::LIBFPTR_PARAM_TAX_TYPE, Meta::toUnderlying(item.m_tax));
            if (m_kkm.registration() < 0) {
                return fail(result);
            }
        }

        LOG_DEBUG_TS(Wcs::c_subRegisterPayment, m_logPrefix, m_serialNumber);

        /** Оплата чека **/
        if (details.m_paymentSum > 0) {
            m_kkm.setParam(Atol::LIBFPTR_PARAM_PAYMENT_TYPE, Meta::toUnderlying(details.m_paymentType));
            m_kkm.setParam(Atol::LIBFPTR_PARAM_PAYMENT_SUM, details.m_paymentSum);
            if (m_kkm.payment() < 0) {
                return fail(result);
            }
            // result.m_remainder = m_kkm.getParamDouble(Atol::LIBFPTR_PARAM_REMAINDER); // Неоплаченный остаток чека
            // result.m_change = m_kkm.getParamDouble(Atol::LIBFPTR_PARAM_CHANGE); // Сдача по чеку

            /**
             * «Сведения обо всех оплатах по чеку безналичными»
             * (необязательный реквизит, приказ ФНС России от 26.03.2025 № ЕД-7-20/236@).
             * https://www.nalog.gov.ru/rn71/news/activities_fts/16524721/
             **/
            if (details.m_electroPaymentInfo) {
                m_kkm.setParam(Atol::LIBFPTR_PARAM_PAYMENT_TYPE, Atol::LIBFPTR_PT_ADD_INFO);
                m_kkm.setParam(Atol::LIBFPTR_PARAM_PAYMENT_SUM, details.m_paymentSum);
                m_kkm.setParam(Atol::LIBFPTR_PARAM_ELECTRONICALLY_PAYMENT_METHOD, details.m_electroPaymentMethod);
                m_kkm.setParam(Atol::LIBFPTR_PARAM_ELECTRONICALLY_ID, details.m_electroPaymentId);
                if (!details.m_electroPaymentAddInfo.empty()) {
                    m_kkm.setParam(Atol::LIBFPTR_PARAM_ELECTRONICALLY_ADD_INFO, details.m_electroPaymentAddInfo);
                }
                if (m_kkm.payment() < 0) {
                    return fail(result);
                }
            }
        }

        LOG_DEBUG_TS(Wcs::c_subRegisterReceiptAndPrint, m_logPrefix, m_serialNumber);

        /** Закрытие чека **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_PAYMENT_TYPE, Meta::toUnderlying(details.m_paymentType));
        if (m_kkm.closeReceipt() < 0) {
            return fail(result);
        }

        m_needToCancelReceipt = false;
        subCheckDocumentClosed(result);
    }

    void Device::registerSell(const ReceiptDetails & details, Result & result) {
        LOG_DEBUG_TS(Wcs::c_sellMethod, m_logPrefix, m_serialNumber);
        subRegisterReceipt(ReceiptType::Sell, details, result);
    }

    void Device::registerSellReturn(const ReceiptDetails & details, Result & result) {
        LOG_DEBUG_TS(Wcs::c_sellReturnMethod, m_logPrefix, m_serialNumber);
        subRegisterReceipt(ReceiptType::SellReturn, details, result);
    }

    void Device::subCloseShift(const OperatorDetails & details, Result & result) {
        /** Запрос состояния смены **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DATA_TYPE, Atol::LIBFPTR_DT_SHIFT_STATE);
        if (m_kkm.queryData() < 0) {
            return fail(result);
        }
        if (static_cast<ShiftState>(m_kkm.getParamInt(Atol::LIBFPTR_PARAM_SHIFT_STATE)) == ShiftState::Closed) {
            LOG_DEBUG_TS(Wcs::c_subCloseShiftNoNeed, m_logPrefix, m_serialNumber);
        } else {
            subSetOperator(details);
            LOG_DEBUG_TS(Wcs::c_subCloseShift, m_logPrefix, m_serialNumber);
            // LOG_DEBUG_TS(Wcs::c_subPrint, m_logPrefix, m_serialNumber);
            /** Закрытие смены **/
            m_kkm.setParam(Atol::LIBFPTR_PARAM_REPORT_TYPE, Atol::LIBFPTR_RT_CLOSE_SHIFT);
            if (m_kkm.report() < 0) {
                return fail(result);
            }
            subCheckDocumentClosed(result);
        }
    }

    void Device::subCashOut(const OperatorDetails & details, Result & result) {
        /** Запрос суммы наличных в денежном ящике **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DATA_TYPE, Atol::LIBFPTR_DT_CASH_SUM);
        if (m_kkm.queryData() < 0) {
            return fail(result);
        }
        double cashSum { m_kkm.getParamDouble(Atol::LIBFPTR_PARAM_SUM) };
        if (cashSum > 0) {
            subSetOperator(details);
            LOG_DEBUG_TS(Wcs::c_subCashOut, m_logPrefix, m_serialNumber);
            /** Выплата **/
            m_kkm.setParam(Atol::LIBFPTR_PARAM_SUM, cashSum);
            m_kkm.setParam(Atol::LIBFPTR_PARAM_DOCUMENT_ELECTRONICALLY, details.m_electronically);
            if (m_kkm.cashOutcome() < 0) {
                return fail(result);
            }
        } else {
            LOG_DEBUG_TS(Wcs::c_subCashOutNoNeed, m_logPrefix, m_serialNumber);
        }
    }

    void Device::closeShift(const CloseDetails & details, Result & result) {
        LOG_DEBUG_TS(Wcs::c_closeShiftMethod, m_logPrefix, m_serialNumber);

        if (details.m_cashOut) {
            subCashOut(details, result);
        }

        subCloseShift(details, result);
    }

    void Device::reportX(const CloseDetails & details, Result & result) {
        LOG_DEBUG_TS(Wcs::c_reportXMethod, m_logPrefix, m_serialNumber);

        if (details.m_cashOut) {
            subCashOut(details, result);
        }
        if (details.m_closeShift) {
            subCloseShift(details, result);
        }

        LOG_DEBUG_TS(Wcs::c_subPrint, m_logPrefix, m_serialNumber);

        /** Печать X-отчета **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_REPORT_TYPE, Atol::LIBFPTR_RT_X);
        if (m_kkm.report() < 0) {
            fail(result);
        }
    }

    void Device::resetState(const CloseDetails & details, Result & result) {
        LOG_DEBUG_TS(Wcs::c_resetStateMethod, m_logPrefix, m_serialNumber);

        /** Запрос состояния чека **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DATA_TYPE, Atol::LIBFPTR_DT_RECEIPT_STATE);
        if (m_kkm.queryData() < 0) {
            return fail(result);
        }
        if (static_cast<ReceiptType>(m_kkm.getParamInt(Atol::LIBFPTR_PARAM_RECEIPT_TYPE)) == ReceiptType::Closed) {
            LOG_DEBUG_TS(Wcs::c_subCancelReceiptNoNeed, m_logPrefix, m_serialNumber);
        } else {
            LOG_DEBUG_TS(Wcs::c_subCancelReceipt, m_logPrefix, m_serialNumber);
            /** Отмена чека **/
            if (m_kkm.cancelReceipt() < 0) {
                return fail(result);
            }
        }

        if (details.m_cashOut) {
            subCashOut(details, result);
        }
        if (details.m_closeShift) {
            subCloseShift(details, result);
        }
    }
}
