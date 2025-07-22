// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "kkm.h"
#include <fstream>
#include "log.h"

namespace Kkm {
    namespace Mbs {
        using Json::Mbs::c_successKey;
        using Json::Mbs::c_messageKey;
    }

    using Basic::DataError;
    using Basic::c_sleepQuantum;

    Failure::Failure(const std::wstring_view message, std::source_location && location)
    : Basic::Failure(message, std::forward<std::source_location>(location)) {}

    Failure::Failure(Device & kkm, std::source_location && location)
    : Basic::Failure(kkm.m_kkm.errorDescription(), std::forward<std::source_location>(location)) {
        kkm.m_kkm.resetError();
    }

    Device::ConnParams::ConnParams(const Container & params)
    : m_params { params } {}

    Device::ConnParams::ConnParams(Container && params)
    : m_params { std::forward<Container>(params) } {}

    Device::ConnParams::operator std::wstring() const {
        std::wstring params;
        Text::joinTo(params, m_params, c_connParamsSeparator);
        return params;
    }

    void Device::ConnParams::apply(Device & kkm) const {
        if (m_params[0] == L"com") {
            applyCom(kkm);
        } else if (m_params[0] == L"usb") {
            applyUsb(kkm);
        } else if (m_params[0] == L"tcpip" || m_params[0] == L"ip") {
            applyTcpIp(kkm);
        } else if (m_params[0] == L"bluetooth" || m_params[0] == L"bt") {
            applyBluetooth(kkm);
        } else {
            throw Failure(Wcs::c_invalidConnParams); // NOLINT(*-exception-baseclass)
        }
    }

    void Device::ConnParams::applyCommon(Device & kkm) const { // NOLINT(*-convert-member-functions-to-static)
        kkm.m_kkm.setSingleSetting(Atol::LIBFPTR_SETTING_MODEL, std::to_wstring(Atol::LIBFPTR_MODEL_ATOL_AUTO));
        if (s_timeZoneConfigured) {
            kkm.m_kkm.setSingleSetting(Atol::LIBFPTR_SETTING_TIME_ZONE, std::to_wstring(static_cast<int>(s_timeZone)));
        }
        kkm.m_kkm.setSingleSetting(Atol::LIBFPTR_SETTING_OFD_CHANNEL, std::to_wstring(Atol::LIBFPTR_OFD_CHANNEL_AUTO));
        // ISSUE: Из документации не ясно, что передавать в качестве значения параметра.
        //  Но нам этот параметр не очень нужен, потому как при формировании чека единицы у нас
        //  всегда передаются и проблем с ФФД 1.2+ не должно возникнуть.
        // kkm.m_kkm.setSingleSetting(Atol::LIBFPTR_SETTING_AUTO_MEASUREMENT_UNIT, ???);
    }

    void Device::ConnParams::applyCom(Device & kkm) const {
        if (m_params.size() < 2) {
            throw Failure(Wcs::c_invalidConnParams); // NOLINT(*-exception-baseclass)
        }
        std::wstring port { L"COM" };
        port.append(m_params[1]);
        std::wstring baudRate;
        if (m_params.size() < 3) {
            baudRate.assign(c_defBaudRate);
        } else if (std::ranges::find(s_allowedBaudRate, m_params[2]) != s_allowedBaudRate.end()) {
            baudRate.assign(m_params[2]);
        } else {
            throw Failure(Wcs::c_invalidConnParams); // NOLINT(*-exception-baseclass)
        }
        kkm.m_kkm.setSingleSetting(Atol::LIBFPTR_SETTING_PORT, std::to_wstring(Atol::LIBFPTR_PORT_COM));
        kkm.m_kkm.setSingleSetting(Atol::LIBFPTR_SETTING_COM_FILE, port);
        kkm.m_kkm.setSingleSetting(Atol::LIBFPTR_SETTING_BAUDRATE, baudRate);
        applyCommon(kkm);
        if (kkm.m_kkm.applySingleSettings() < 0) {
            throw Failure(kkm); // NOLINT(*-exception-baseclass)
        }
    }

    void Device::ConnParams::applyUsb(Device &) const { // NOLINT(*-convert-member-functions-to-static)
        // TODO: Реализовать.
        // if (m_params.size() < 2) {
        //     throw Failure(Wcs::c_invalidConnParams); // NOLINT(*-exception-baseclass)
        // }
        // kkm.m_kkm.setSingleSetting(Atol::LIBFPTR_SETTING_PORT, std::to_wstring(Atol::LIBFPTR_PORT_USB));
        // ...
        throw Failure(Wcs::c_notImplemented); // NOLINT(*-exception-baseclass)
    }

    void Device::ConnParams::applyTcpIp(Device &) const { // NOLINT(*-convert-member-functions-to-static)
        // TODO: Реализовать.
        // if (m_params.size() < 2) {
        //     throw Failure(Wcs::c_invalidConnParams); // NOLINT(*-exception-baseclass)
        // }
        // kkm.m_kkm.setSingleSetting(Atol::LIBFPTR_SETTING_PORT, std::to_wstring(Atol::LIBFPTR_PORT_TCPIP));
        // ...
        throw Failure(Wcs::c_notImplemented); // NOLINT(*-exception-baseclass)
    }

    void Device::ConnParams::applyBluetooth(Device &) const { // NOLINT(*-convert-member-functions-to-static)
        // TODO: Реализовать.
        // if (m_params.size() < 2) {
        //     throw Failure(Wcs::c_invalidConnParams); // NOLINT(*-exception-baseclass)
        // }
        // kkm.m_kkm.setSingleSetting(Atol::LIBFPTR_SETTING_PORT, std::to_wstring(Atol::LIBFPTR_PORT_BLUETOOTH));
        // ...
        throw Failure(Wcs::c_notImplemented); // NOLINT(*-exception-baseclass)
    }

    Device::NewConnParams::NewConnParams(const std::wstring & params)
    : ConnParams() {
        Text::splitTo(m_params, params, c_connParamsSeparator);
        if (m_params.size() < 2) {
            throw Failure(Wcs::c_invalidConnParams); // NOLINT(*-exception-baseclass)
        }
        Text::lower(m_params[0]);
    }

    void Device::NewConnParams::save(const std::wstring & serialNumber) {
        if (serialNumber.empty()) {
            throw Failure(std::format(Wcs::c_cantSave, L"-")); // NOLINT(*-exception-baseclass)
        }
        std::filesystem::path filePath { s_directory };
        if (!std::filesystem::is_directory(filePath)) {
            std::filesystem::create_directories(filePath);
            if (!std::filesystem::is_directory(filePath)) {
                throw Failure(std::format(Wcs::c_cantSave, serialNumber)); // NOLINT(*-exception-baseclass)
            }
        }
        filePath /= serialNumber + L".json";
        Nln::Json json = Text::convert(m_params);
        std::ofstream file { filePath };
        file << json.dump();
        file.close();
        if (!std::filesystem::is_regular_file(filePath)) {
            throw Failure(std::format(Wcs::c_cantSave, serialNumber)); // NOLINT(*-exception-baseclass)
        }
    }

    Device::KnownConnParams::KnownConnParams(const NewConnParams & params, const std::wstring_view serialNumber)
    : ConnParams(params.m_params), m_serialNumber(serialNumber) {}

    Device::KnownConnParams::KnownConnParams(NewConnParams && params, const std::wstring_view serialNumber)
    : ConnParams(std::forward<Container>(params.m_params)), m_serialNumber(serialNumber) {}

    Device::KnownConnParams::KnownConnParams(std::wstring && serialNumber)
    : ConnParams(), m_serialNumber(std::forward<std::wstring>(serialNumber)) {}

    Device::KnownConnParams::KnownConnParams(const wchar_t * serialNumber)
    : KnownConnParams(std::wstring(serialNumber)) {
        std::filesystem::path filePath { s_directory };
        filePath /= m_serialNumber + L".json";
        if (!std::filesystem::is_regular_file(filePath)) {
            throw Failure(std::format(Wcs::c_loadingError, m_serialNumber)); // NOLINT(*-exception-baseclass)
        }
        Nln::Json json(Nln::Json::parse(std::ifstream(filePath)));
        Json::handle(json, m_params);
    }

    Device::KnownConnParams::KnownConnParams(const std::filesystem::path & filePath)
    : KnownConnParams(serialNumber(filePath)) {
        if (!std::filesystem::is_regular_file(filePath)) {
            throw Failure(std::format(Wcs::c_loadingError, m_serialNumber)); // NOLINT(*-exception-baseclass)
        }
        Nln::Json json(Nln::Json::parse(std::ifstream(filePath)));
        Json::handle(json, m_params);
    }

    [[nodiscard]]
    const std::wstring & Device::KnownConnParams::serialNumber() const {
        return m_serialNumber;
    }

    [[nodiscard]]
    std::wstring Device::KnownConnParams::serialNumber(const std::filesystem::path & filePath) {
        auto fileName = filePath.filename();
        fileName.replace_extension(L"");
        return fileName.c_str();
    }

    Device::Device(const std::wstring_view logPrefix)
    : m_kkm {}, m_serialNumber {}, m_logPrefix { logPrefix }, m_lineLength {}, m_needToCancelReceipt { false } {}

    Device::Device(const ConnParams & connParams, std::wstring_view logPrefix)
    : Device(logPrefix) {
        connect(connParams);
    }

    Device::Device(const KnownConnParams & connParams, std::wstring_view logPrefix)
    : Device(logPrefix) {
        connect(connParams);
        if (m_serialNumber != connParams.m_serialNumber) {
            throw Failure(std::format(Wcs::c_serialNumberMismatch, connParams.m_serialNumber, m_serialNumber)); // NOLINT(*-exception-baseclass)
        }
    }

    Device::~Device() {
        if (m_needToCancelReceipt && m_kkm.cancelReceipt() < 0) {
            tsLogWarning(Wcs::c_cancelingError, m_logPrefix, m_serialNumber, m_kkm.errorDescription());
        }
        m_kkm.close();
    }

    void Device::connect(const ConnParams & connParams) {
        connParams.apply(*this);
        if (m_kkm.open() < 0) {
            throw Failure(*this); // NOLINT(*-exception-baseclass)
        }
        if (!m_kkm.isOpened()) {
            throw Failure(Wcs::c_notAvailable); // NOLINT(*-exception-baseclass)
        }
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DATA_TYPE, Atol::LIBFPTR_DT_SERIAL_NUMBER);
        if (m_kkm.queryData() < 0) {
            throw Failure(*this); // NOLINT(*-exception-baseclass)
        }
        m_serialNumber = m_kkm.getParamString(Atol::LIBFPTR_PARAM_SERIAL_NUMBER);
        if (m_serialNumber.empty()) {
            throw Failure(Wcs::c_exchangeError); // NOLINT(*-exception-baseclass)
        }
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DATA_TYPE, Atol::LIBFPTR_DT_RECEIPT_LINE_LENGTH);
        if (m_kkm.queryData() < 0) {
            throw Failure(*this); // NOLINT(*-exception-baseclass)
        }
        m_lineLength = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_RECEIPT_LINE_LENGTH);
        if (m_lineLength < 1) {
            m_lineLength = s_defaultLineLength;
            tsLogWarning(Wcs::c_wrongLength, m_logPrefix, m_serialNumber);
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
        Numeric::doubleClamp(marginOuter, marginInner, 0, 10);
        ++marginInner;
        std::wstring result;
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
            throw Failure(*this); // NOLINT(*-exception-baseclass)
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
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DEFER, static_cast<int>(position));
        if (m_kkm.printText() < 0) {
            throw Failure(*this); // NOLINT(*-exception-baseclass)
        }
    }

    void Device::subPrintText(const PrintableText & text, const TextPosition position) {
        subPrintText(
            text.m_content, text.m_center, text.m_magnified, text.m_separated,
            text.m_marginOuter, text.m_marginInner, position
        );
    }

    void Device::subCheckDocumentClosed(Call::Result & result) {
        assert(s_documentClosingTimeout >= c_sleepQuantum);
        // ISSUE: Из документации не очень понятно как работать с методом checkDocumentClosed() - описания нет,
        //  приведенный пример выглядит странно и рассчитан скорее всего на интерактивное взаимодействие с ККМ.
        //  В нашем случае интерактивность невозможна. Будем ждать чуда. Если чуда не произойдет, отменять чек.
        decltype(s_documentClosingTimeout) i;
        for (i = s_documentClosingTimeout / c_sleepQuantum; m_kkm.checkDocumentClosed() < 0 && i; --i) {
            tsLogWarning(Wcs::c_closingError, m_logPrefix, m_serialNumber, m_kkm.errorDescription());
            // ::Sleep(c_sleepQuantum);
            std::this_thread::sleep_for(std::chrono::milliseconds(c_sleepQuantum));
        }
        if (!i || !m_kkm.getParamBool(Atol::LIBFPTR_PARAM_DOCUMENT_CLOSED)) {
            if (m_kkm.cancelReceipt() < 0) {
                tsLogWarning(Wcs::c_cancelingError, m_logPrefix, m_serialNumber, m_kkm.errorDescription());
            }
            return result.fail(Wcs::c_checkingError);
        }
        if (!m_kkm.getParamBool(Atol::LIBFPTR_PARAM_DOCUMENT_PRINTED)) {
            for (i = s_documentClosingTimeout / c_sleepQuantum; m_kkm.continuePrint() < 0 && i; --i) {
                tsLogWarning(Wcs::c_printingError, m_logPrefix, m_serialNumber, m_kkm.errorDescription());
                // ::Sleep(c_sleepQuantum);
                std::this_thread::sleep_for(std::chrono::milliseconds(c_sleepQuantum));
            }
            if (!i) {
                return result.fail(Wcs::c_checkingError);
            }
        }
    }

    void Device::subSetOperator(const Call::OperatorDetails & details) {
        tsLogDebug(Wcs::c_subSetOperator, m_logPrefix, m_serialNumber);

        /** Регистрация оператора **/
        m_kkm.setParam(1021, details.m_operatorName);
        if (!details.m_operatorInn.empty()) {
            m_kkm.setParam(1203, details.m_operatorInn);
        }
        if (m_kkm.operatorLogin() < 0) {
            throw Failure(*this); // NOLINT(*-exception-baseclass)
        }
    }

    void Device::subSetCustomer(const Call::ReceiptDetails & details) {
        tsLogDebug(Wcs::c_subSetCustomer, m_logPrefix, m_serialNumber);

        /** Регистрация информации о покупателе / клиенте **/
        if (!details.m_customerContact.empty()) {
            m_kkm.setParam(1008, details.m_customerContact);
        }
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
                throw Failure(*this); // NOLINT(*-exception-baseclass)
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
                throw Failure(*this); // NOLINT(*-exception-baseclass)
            }
            std::vector<uchar> clientInfo = m_kkm.getParamByteArray(Atol::LIBFPTR_PARAM_TAG_VALUE);
            m_kkm.setParam(1084, clientInfo);
        }
    }

    void Device::Call::Result::fail(std::wstring_view message, const std::source_location & location) {
        if (m_success) {
            m_success = false;
            m_message = message;
        }
        if (Log::s_appendLocation) {
            std::wstring messageWithLocation { message };
            messageWithLocation += location;
            tsLogWarning(messageWithLocation);
        } else {
            tsLogWarning(message);
        }
    }

    void Device::Call::Result::fail(Device & kkm, const std::source_location & location) {
        std::wstring message { kkm.m_kkm.errorDescription() };
        if (m_success) {
            m_success = false;
            m_message = message;
        }
        std::wstring logMessage { std::format(Wcs::c_fault, kkm.m_logPrefix, kkm.m_serialNumber, message) };
        if (Log::s_appendLocation) {
            logMessage += location;
        }
        tsLogWarning(logMessage);
        kkm.m_kkm.resetError();
    }

    bool Device::Call::Result::exportTo(Nln::Json & json) {
        bool overrideMessage { false };
        if (
            !json.contains(Mbs::c_successKey)
            || !json[Mbs::c_successKey].is_boolean()
            || (json[Mbs::c_successKey].get<bool>() && !m_success)
        ) {
            json[Mbs::c_successKey] = m_success;
            overrideMessage = true;
        }
        if (
            overrideMessage
            || !json.contains(Mbs::c_messageKey)
            || !json[Mbs::c_messageKey].is_string()
            || json[Mbs::c_messageKey].empty()
            || json[Mbs::c_messageKey].get<std::string>() == Mbs::c_ok
        ) {
            json[Mbs::c_messageKey] = Text::convert(m_message);
        }
        return json[Mbs::c_successKey].get<bool>();
    }

    bool Device::Call::StatusResult::exportTo(Nln::Json & json) {
        if (this->Device::Call::Result::exportTo(json) && m_success) {
            json["status"] = {
                { "blocked", m_blocked },
                { "cashDrawerOpened", m_cashDrawerOpened },
                { "coverOpened", m_coverOpened },
                { "cutError", m_cutError },
                { "dateTime", DateTime::cast<std::string>(m_dateTime) },
                { "documentNumber", m_documentNumber },
                { "documentType", m_documentType },
                { "documentTypeText", Text::convert(s_documentTypeLabels.at(m_documentType)) },
                { "fiscal", m_fiscal },
                { "fnFiscal", m_fnFiscal },
                { "fnPresent", m_fnPresent },
                { "invalidFn", m_invalidFn },
                { "logicalNumber", m_logicalNumber },
                { "mode", m_mode },
                { "model", m_model },
                { "operatorId", m_operatorId },
                { "operatorRegistered", m_operatorRegistered },
                { "paperNearEnd", m_paperNearEnd },
                { "printerConnectionLost", m_printerConnectionLost },
                { "printerError", m_printerError },
                { "printerOverheat", m_printerOverheat },
                { "receiptLineLength", m_receiptLineLength },
                { "receiptLineLengthPix", m_receiptLineLengthPix },
                { "receiptNumber", m_receiptNumber },
                { "receiptPaperPresent", m_receiptPaperPresent },
                { "receiptSum", m_receiptSum },
                { "receiptType", m_receiptType },
                { "receiptTypeText", Text::convert(s_receiptTypeLabels.at(m_receiptType)) },
                { "serialNumber", Text::convert(m_serialNumber) },
                { "shiftNumber", m_shiftNumber },
                { "shiftState", m_shiftState },
                { "shiftStateText", Text::convert(s_shiftStateLabels.at(m_shiftState)) },
                { "subMode", m_subMode }
            };
            return true;
        }
        return false;
    }

    void Device::getStatus(Call::StatusResult & result) {
        tsLogDebug(Wcs::c_statusMethod, m_logPrefix, m_serialNumber);

        /** Запрос информации о ККТ **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DATA_TYPE, Atol::LIBFPTR_DT_STATUS);
        if (m_kkm.queryData() < 0) {
            return result.fail(*this); // throw Failure(*this); // NOLINT(*-exception-baseclass)
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

    bool Device::Call::ShiftStateResult::exportTo(Nln::Json & json) {
        if (this->Device::Call::Result::exportTo(json) && m_success) {
            json["shiftState"] = {
                { "documentsCount", m_documentsCount },
                { "receiptNumber", m_receiptNumber },
                { "expiredAt", DateTime::cast<std::string>(m_expirationDateTime) },
                { "shiftNumber", m_shiftNumber },
                { "shiftState", m_shiftState },
                { "shiftStateText", Text::convert(s_shiftStateLabels.at(m_shiftState)) }
            };
            return true;
        }
        return false;
    }

    void Device::getShiftState(Call::ShiftStateResult & result) {
        tsLogDebug(Wcs::c_shiftStateMethod, m_logPrefix, m_serialNumber);

        /** Запрос состояния смены **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DATA_TYPE, Atol::LIBFPTR_DT_SHIFT_STATE);
        if (m_kkm.queryData() < 0) {
            return result.fail(*this); // throw Failure(*this); // NOLINT(*-exception-baseclass)
        }
        result.m_shiftNumber = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_SHIFT_NUMBER);
        result.m_shiftState = static_cast<ShiftState>(m_kkm.getParamInt(Atol::LIBFPTR_PARAM_SHIFT_STATE));
        result.m_expirationDateTime = m_kkm.getParamDateTime(Atol::LIBFPTR_PARAM_DATE_TIME);

        /** Запрос информации о текущей смене в ФН **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_FN_DATA_TYPE, Atol::LIBFPTR_FNDT_SHIFT);
        if (m_kkm.fnQueryData() < 0) {
            return result.fail(*this); // throw Failure(*this); // NOLINT(*-exception-baseclass)
        }
        result.m_receiptNumber = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_RECEIPT_NUMBER);
        auto shiftNumber = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_SHIFT_NUMBER);
        // ISSUE: Проверка дублирующейся информации. Не факт, что она должна совпадать.
        if (shiftNumber != result.m_shiftNumber) {
            // throw Failure(Wcs::c_invalidData); // NOLINT(*-exception-baseclass)
            tsLogWarning(Wcs::c_shiftMismatch, m_logPrefix, m_serialNumber);
        }

        /** Запрос количества ФД за смену **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_FN_DATA_TYPE, Atol::LIBFPTR_FNDT_DOCUMENTS_COUNT_IN_SHIFT);
        if (m_kkm.fnQueryData() < 0) {
            return result.fail(*this); // throw Failure(*this); // NOLINT(*-exception-baseclass)
        }
        result.m_documentsCount = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_DOCUMENTS_COUNT);
    }

    bool Device::Call::ReceiptStateResult::exportTo(Nln::Json & json) {
        if (this->Device::Call::Result::exportTo(json) && m_success) {
            json["receiptState"] = {
                { "receiptType", m_receiptType },
                { "receiptTypeText", Text::convert(s_receiptTypeLabels.at(m_receiptType)) },
                { "receiptNumber", m_receiptNumber },
                { "documentNumber", m_documentNumber },
                { "sum", m_sum },
                { "remainder", m_remainder },
                { "change", m_change }
            };
            return true;
        }
        return false;
    }

    void Device::getReceiptState(Call::ReceiptStateResult & result) {
        tsLogDebug(Wcs::c_receiptStateMethod, m_logPrefix, m_serialNumber);

        /** Запрос состояния чека **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DATA_TYPE, Atol::LIBFPTR_DT_RECEIPT_STATE);
        if (m_kkm.queryData() < 0) {
            return result.fail(*this); // throw Failure(*this); // NOLINT(*-exception-baseclass)
        }
        result.m_receiptType = static_cast<ReceiptType>(m_kkm.getParamInt(Atol::LIBFPTR_PARAM_RECEIPT_TYPE));
        result.m_receiptNumber = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_RECEIPT_NUMBER);
        result.m_documentNumber = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_DOCUMENT_NUMBER);
        result.m_sum = m_kkm.getParamDouble(Atol::LIBFPTR_PARAM_RECEIPT_SUM);
        result.m_remainder = m_kkm.getParamDouble(Atol::LIBFPTR_PARAM_REMAINDER);
        result.m_change = m_kkm.getParamDouble(Atol::LIBFPTR_PARAM_CHANGE);
    }

    bool Device::Call::CashStatResult::exportTo(Nln::Json & json) {
        if (this->Device::Call::Result::exportTo(json) && m_success) {
            json["cashStat"] = {
                { "sellCashSum", m_sellCashSum },
                { "sellReturnCashSum", m_sellReturnCashSum },
                { "cashSum", m_cashSum },
                { "cashInSum", m_cashInSum },
                { "cashOutSum", m_cashOutSum },
                { "cashInCount", m_cashInCount },
                { "cashOutCount", m_cashOutCount }
            };
            return true;
        }
        return false;
    }

    void Device::getCashStat(Call::CashStatResult & result) {
        tsLogDebug(Wcs::c_cashStatMethod, m_logPrefix, m_serialNumber);

        /** Запрос суммы наличных платежей в чеках прихода (продажи) **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DATA_TYPE, Atol::LIBFPTR_DT_PAYMENT_SUM);
        m_kkm.setParam(Atol::LIBFPTR_PARAM_PAYMENT_TYPE, Atol::LIBFPTR_PT_CASH);
        m_kkm.setParam(Atol::LIBFPTR_PARAM_RECEIPT_TYPE, Atol::LIBFPTR_RT_SELL);
        if (m_kkm.queryData() < 0) {
            return result.fail(*this); // throw Failure(*this); // NOLINT(*-exception-baseclass)
        }
        result.m_sellCashSum = m_kkm.getParamDouble(Atol::LIBFPTR_PARAM_SUM);

        /** Запрос суммы наличных платежей в чеках возврата прихода (продажи) **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DATA_TYPE, Atol::LIBFPTR_DT_PAYMENT_SUM);
        m_kkm.setParam(Atol::LIBFPTR_PARAM_PAYMENT_TYPE, Atol::LIBFPTR_PT_CASH);
        m_kkm.setParam(Atol::LIBFPTR_PARAM_RECEIPT_TYPE, Atol::LIBFPTR_RT_SELL_RETURN);
        if (m_kkm.queryData() < 0) {
            return result.fail(*this); // throw Failure(*this); // NOLINT(*-exception-baseclass)
        }
        result.m_sellReturnCashSum = m_kkm.getParamDouble(Atol::LIBFPTR_PARAM_SUM);

        /** Запрос суммы внесений **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DATA_TYPE, Atol::LIBFPTR_DT_CASHIN_SUM);
        if (m_kkm.queryData() < 0) {
            return result.fail(*this); // throw Failure(*this); // NOLINT(*-exception-baseclass)
        }
        result.m_cashInSum = m_kkm.getParamDouble(Atol::LIBFPTR_PARAM_SUM);

        /** Запрос суммы выплат **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DATA_TYPE, Atol::LIBFPTR_DT_CASHOUT_SUM);
        if (m_kkm.queryData() < 0) {
            return result.fail(*this); // throw Failure(*this); // NOLINT(*-exception-baseclass)
        }
        result.m_cashOutSum = m_kkm.getParamDouble(Atol::LIBFPTR_PARAM_SUM);

        /** Запрос количества внесений **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DATA_TYPE, Atol::LIBFPTR_DT_CASHIN_COUNT);
        if (m_kkm.queryData() < 0) {
            return result.fail(*this); // throw Failure(*this); // NOLINT(*-exception-baseclass)
        }
        result.m_cashInCount = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_DOCUMENTS_COUNT);

        /** Запрос количества выплат **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DATA_TYPE, Atol::LIBFPTR_DT_CASHOUT_COUNT);
        if (m_kkm.queryData() < 0) {
            return result.fail(*this); // throw Failure(*this); // NOLINT(*-exception-baseclass)
        }
        result.m_cashOutCount = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_DOCUMENTS_COUNT);

        /** Запрос суммы наличных в денежном ящике **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DATA_TYPE, Atol::LIBFPTR_DT_CASH_SUM);
        if (m_kkm.queryData() < 0) {
            return result.fail(*this); // throw Failure(*this); // NOLINT(*-exception-baseclass)
        }
        result.m_cashSum = m_kkm.getParamDouble(Atol::LIBFPTR_PARAM_SUM);
    }

    bool Device::Call::FndtOfdExchangeStatusResult::exportTo(Nln::Json & json) {
        if (this->Device::Call::Result::exportTo(json) && m_success) {
            json["ofdExchangeStatus"] = {
                { "exchangeStatus", m_exchangeStatus },
                { "unsentCount", m_unsentCount },
                { "firstUnsentNumber", m_firstUnsentNumber },
                { "ofdMessageRead", m_ofdMessageRead },
                { "firstUnsentDateTime", DateTime::cast<std::string>(m_firstUnsentDateTime) },
                { "okpDateTime", DateTime::cast<std::string>(m_okpDateTime) }
            };
            return true;
        }
        return false;
    }

    void Device::getFndtOfdExchangeStatus(Call::FndtOfdExchangeStatusResult & result) {
        tsLogDebug(Wcs::c_ofdExchangeStatusMethod, m_logPrefix, m_serialNumber);

        /** Запрос статуса информационного обмена с ОФД **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_FN_DATA_TYPE, Atol::LIBFPTR_FNDT_OFD_EXCHANGE_STATUS);
        if (m_kkm.fnQueryData() < 0) {
            return result.fail(*this); // throw Failure(*this); // NOLINT(*-exception-baseclass)
        }
        result.m_exchangeStatus = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_OFD_EXCHANGE_STATUS);
        result.m_unsentCount = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_DOCUMENTS_COUNT);
        result.m_firstUnsentNumber = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_DOCUMENT_NUMBER);
        result.m_ofdMessageRead = m_kkm.getParamBool(Atol::LIBFPTR_PARAM_OFD_MESSAGE_READ);
        result.m_firstUnsentDateTime = m_kkm.getParamDateTime(Atol::LIBFPTR_PARAM_DATE_TIME);
        result.m_okpDateTime = m_kkm.getParamDateTime(Atol::LIBFPTR_PARAM_LAST_SUCCESSFUL_OKP);
    }

    bool Device::Call::FndtFnInfoResult::exportTo(Nln::Json & json) {
        if (this->Device::Call::Result::exportTo(json) && m_success) {
            json["fnInfo"] = {
                { "serial", Text::convert(m_serial) },
                { "version", Text::convert(m_version) },
                { "execution", Text::convert(m_execution) },
                { "type", m_type },
                { "state", m_state },
                { "flags", m_flags },
                { "needReplacement", m_needReplacement },
                { "exhausted", m_exhausted },
                { "memoryOverflow", m_memoryOverflow },
                { "ofdTimeout", m_ofdTimeout },
                { "criticalError", m_criticalError },
                { "keysUpdaterServerUri", Text::convert(m_keysUpdaterServerUri) }
            };
            return true;
        }
        return false;
    }

    void Device::getFndtFnInfo(Call::FndtFnInfoResult & result) {
        tsLogDebug(Wcs::c_fnInfoMethod, m_logPrefix, m_serialNumber);

        /** Запрос информации и статуса ФН **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_FN_DATA_TYPE, Atol::LIBFPTR_FNDT_FN_INFO);
        if (m_kkm.fnQueryData() < 0) {
            return result.fail(*this); // throw Failure(*this); // NOLINT(*-exception-baseclass)
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

    bool Device::Call::FndtLastRegistrationResult::exportTo(Nln::Json & json) {
        if (this->Device::Call::Result::exportTo(json) && m_success) {
            json["lastRegistration"] = {
                { "documentNumber", m_documentNumber },
                { "registrationsCount", m_registrationsCount },
                { "registrationDateTime", DateTime::cast<std::string>(m_registrationDateTime) }
            };
            return true;
        }
        return false;
    }

    void Device::getFndtLastRegistration(Call::FndtLastRegistrationResult & result) {
        tsLogDebug(Wcs::c_lastRegistrationMethod, m_logPrefix, m_serialNumber);

        /** Запрос информации о последней регистрации / перерегистрации **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_FN_DATA_TYPE, Atol::LIBFPTR_FNDT_LAST_REGISTRATION);
        if (m_kkm.fnQueryData() < 0) {
            return result.fail(*this); // throw Failure(*this); // NOLINT(*-exception-baseclass)
        }
        result.m_documentNumber = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_DOCUMENT_NUMBER);
        result.m_registrationsCount = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_REGISTRATIONS_COUNT);
        result.m_registrationDateTime = m_kkm.getParamDateTime(Atol::LIBFPTR_PARAM_DATE_TIME);
    }

    bool Device::Call::FndtLastReceiptResult::exportTo(Nln::Json & json) {
        if (this->Device::Call::Result::exportTo(json) && m_success) {
            json["lastReceipt"] = {
                { "documentNumber", m_documentNumber },
                { "receiptSum", m_receiptSum },
                { "fiscalSign", Text::convert(m_fiscalSign) },
                { "documentDateTime", DateTime::cast<std::string>(m_documentDateTime) }
            };
            return true;
        }
        return false;
    }

    void Device::getFndtLastReceipt(Call::FndtLastReceiptResult & result) {
        tsLogDebug(Wcs::c_lastReceiptMethod, m_logPrefix, m_serialNumber);

        /** Запрос информации о последнем чеке **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_FN_DATA_TYPE, Atol::LIBFPTR_FNDT_LAST_RECEIPT);
        if (m_kkm.fnQueryData() < 0) {
            return result.fail(*this); // throw Failure(*this); // NOLINT(*-exception-baseclass)
        }
        result.m_documentNumber = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_DOCUMENT_NUMBER);
        result.m_receiptSum = m_kkm.getParamDouble(Atol::LIBFPTR_PARAM_RECEIPT_SUM);
        result.m_fiscalSign = m_kkm.getParamString(Atol::LIBFPTR_PARAM_FISCAL_SIGN);
        result.m_documentDateTime = m_kkm.getParamDateTime(Atol::LIBFPTR_PARAM_DATE_TIME);
    }

    bool Device::Call::FndtLastDocumentResult::exportTo(Nln::Json & json) {
        if (this->Device::Call::Result::exportTo(json) && m_success) {
            json["lastDocument"] = {
                { "documentNumber", m_documentNumber },
                { "fiscalSign", Text::convert(m_fiscalSign) },
                { "documentDateTime", DateTime::cast<std::string>(m_documentDateTime) }
            };
            return true;
        }
        return false;
    }

    void Device::getFndtLastDocument(Call::FndtLastDocumentResult & result) {
        tsLogDebug(Wcs::c_lastDocumentMethod, m_logPrefix, m_serialNumber);

        /** Запрос информации о последнем фискальном документе **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_FN_DATA_TYPE, Atol::LIBFPTR_FNDT_LAST_DOCUMENT);
        if (m_kkm.fnQueryData() < 0) {
            return result.fail(*this); // throw Failure(*this); // NOLINT(*-exception-baseclass)
        }
        result.m_documentNumber = m_kkm.getParamInt(Atol::LIBFPTR_PARAM_DOCUMENT_NUMBER);
        result.m_fiscalSign = m_kkm.getParamString(Atol::LIBFPTR_PARAM_FISCAL_SIGN);
        result.m_documentDateTime = m_kkm.getParamDateTime(Atol::LIBFPTR_PARAM_DATE_TIME);
    }

    bool Device::Call::FndtErrorsResult::exportTo(Nln::Json & json) {
        if (this->Device::Call::Result::exportTo(json) && m_success) {
            json["fndtErrors"] = {
                { "networkError", m_networkError },
                { "networkErrorText", Text::convert(m_networkErrorText) },
                { "ofdError", m_ofdError },
                { "ofdErrorText", Text::convert(m_ofdErrorText) },
                { "fnError", m_fnError },
                { "fnErrorText", Text::convert(m_fnErrorText) },
                { "failedDocumentNumber", m_documentNumber },
                { "failedCommandCode", m_commandCode },
                { "successDateTime", DateTime::cast<std::string>(m_successDateTime) },
                { "dataForSendIsEmpty", m_dataForSendIsEmpty }
            };
            return true;
        }
        return false;
    }

    void Device::getFndtErrors(Call::FndtErrorsResult & result) {
        tsLogDebug(Wcs::c_errorsMethod, m_logPrefix, m_serialNumber);

        /** Запрос ошибок обмена с ОФД **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_FN_DATA_TYPE, Atol::LIBFPTR_FNDT_ERRORS);
        if (m_kkm.fnQueryData() < 0) {
            return result.fail(*this); // throw Failure(*this); // NOLINT(*-exception-baseclass)
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

    bool Device::Call::VersionResult::exportTo(Nln::Json & json) {
        if (this->Device::Call::Result::exportTo(json) && m_success) {
            json["versions"] = {
                { "boot", Text::convert(m_bootVersion) },
                { "configuration", Text::convert(m_configurationVersion) },
                { "controlUnit", Text::convert(m_controlUnitVersion) },
                { "firmware", Text::convert(m_firmwareVersion) },
                { "release", Text::convert(m_releaseVersion) },
                { "templates", Text::convert(m_templatesVersion) }
            };
            return true;
        }
        return false;
    }

    void Device::getVersion(Call::VersionResult & result) {
        tsLogDebug(Wcs::c_versionMethod, m_logPrefix, m_serialNumber);

        /** Запрос версии прошивки **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DATA_TYPE, Atol::LIBFPTR_DT_UNIT_VERSION);
        m_kkm.setParam(Atol::LIBFPTR_PARAM_UNIT_TYPE, Atol::LIBFPTR_UT_FIRMWARE);
        if (m_kkm.queryData() < 0) {
            return result.fail(*this); // throw Failure(*this); // NOLINT(*-exception-baseclass)
        }
        result.m_firmwareVersion.assign(m_kkm.getParamString(Atol::LIBFPTR_PARAM_UNIT_VERSION));

        /** Запрос версии конфигурации **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DATA_TYPE, Atol::LIBFPTR_DT_UNIT_VERSION);
        m_kkm.setParam(Atol::LIBFPTR_PARAM_UNIT_TYPE, Atol::LIBFPTR_UT_CONFIGURATION);
        if (m_kkm.queryData() < 0) {
            return result.fail(*this); // throw Failure(*this); // NOLINT(*-exception-baseclass)
        }
        result.m_configurationVersion.assign(m_kkm.getParamString(Atol::LIBFPTR_PARAM_UNIT_VERSION));
        result.m_releaseVersion.assign(m_kkm.getParamString(Atol::LIBFPTR_PARAM_UNIT_RELEASE_VERSION));

        /** Запрос версии движка шаблонов **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DATA_TYPE, Atol::LIBFPTR_DT_UNIT_VERSION);
        m_kkm.setParam(Atol::LIBFPTR_PARAM_UNIT_TYPE, Atol::LIBFPTR_UT_TEMPLATES);
        if (m_kkm.queryData() < 0) {
            return result.fail(*this); // throw Failure(*this); // NOLINT(*-exception-baseclass)
        }
        result.m_templatesVersion.assign(m_kkm.getParamString(Atol::LIBFPTR_PARAM_UNIT_VERSION));

        /** Запрос версии блока управления **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DATA_TYPE, Atol::LIBFPTR_DT_UNIT_VERSION);
        m_kkm.setParam(Atol::LIBFPTR_PARAM_UNIT_TYPE, Atol::LIBFPTR_UT_CONTROL_UNIT);
        if (m_kkm.queryData() < 0) {
            return result.fail(*this); // throw Failure(*this); // NOLINT(*-exception-baseclass)
        }
        result.m_controlUnitVersion.assign(m_kkm.getParamString(Atol::LIBFPTR_PARAM_UNIT_VERSION));

        /** Запрос версии загрузчика **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DATA_TYPE, Atol::LIBFPTR_DT_UNIT_VERSION);
        m_kkm.setParam(Atol::LIBFPTR_PARAM_UNIT_TYPE, Atol::LIBFPTR_UT_BOOT);
        if (m_kkm.queryData() < 0) {
            return result.fail(*this); // throw Failure(*this); // NOLINT(*-exception-baseclass)
        }
        result.m_bootVersion.assign(m_kkm.getParamString(Atol::LIBFPTR_PARAM_UNIT_VERSION));
    }

    void Device::printHello() {
        tsLogDebug(Wcs::c_printHelloMethod, m_logPrefix, m_serialNumber);

        /** Открытие нефискального документа **/
        if (m_kkm.beginNonfiscalDocument() < 0) {
            throw Failure(*this); // NOLINT(*-exception-baseclass)
        }

        subPrintSeparator(0, 1);
        subPrintText(Wcs::c_helloTitle, true, true);
        subPrintText(Wcs::c_helloText, true);
        subPrintSeparator(1, 0);

        tsLogDebug(Wcs::c_subPrint, m_logPrefix, m_serialNumber);

        /** Закрытие нефискального документа без печати подвала **/
        if (m_kkm.endNonfiscalDocument() < 0) {
            throw Failure(*this); // NOLINT(*-exception-baseclass)
        }
    }

    void Device::printDemo(Call::Result & result) {
        tsLogDebug(Wcs::c_printDemoMethod, m_logPrefix, m_serialNumber);
        tsLogDebug(Wcs::c_subPrint, m_logPrefix, m_serialNumber);

        /** Демо-печать **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_REPORT_TYPE, Atol::LIBFPTR_RT_KKT_DEMO);
        if (m_kkm.report() < 0) {
            result.m_success = false;
            result.m_message.assign(m_kkm.errorDescription());
        }
    }

    Device::Call::Details::Details(const Nln::Json & details)
    : m_electronically(false) {
        Json::handleKey(details, "electronically", m_electronically);
    }

    Device::Call::PrintDetails::PrintDetails(const Nln::Json & details)
    : Details(details) {
        bool margin { false };
        Json::handleKey(details, "cliche", m_cliche);
        Json::handleKey(details, "footer", m_footer);
        Json::handleKey(details, "margin", margin);
        if (details.contains("document") && details["document"].is_array()) {
            for (auto & block : details["document"].get<std::vector<Nln::Json>>()) {
                bool center { false };
                bool magnified { false };
                bool separated { false };
                bool separator { false };
                std::wstring content {};
                Json::handleKey(block, "separator", separator, L"document[].separator");
                if (separator) {
                    separated = true;
                } else {
                    if (!Json::handleKey(block, "content", content, L"document[].content")) {
                        throw Failure(std::format(Wcs::c_requiresProperty, L"document[].content")); // NOLINT(*-exception-baseclass)
                    }
                    Json::handleKey(block, "center", center, L"document[].center");
                    Json::handleKey(block, "magnified", magnified, L"document[].magnified");
                    Json::handleKey(block, "separated", separated, L"document[].separated");
                }
                auto actualMargin = margin && !content.empty();
                m_document.emplace_back(std::move(content), center, magnified, separated, actualMargin);
            }
        } else {
            throw Failure(std::format(Wcs::c_requiresProperty, L"document")); // NOLINT(*-exception-baseclass)
        }
    }

    void Device::printNonFiscalDocument(const Call::PrintDetails & details, Call::Result & result) {
        tsLogDebug(Wcs::c_printNfDocumentMethod, m_logPrefix, m_serialNumber);

        /** Открытие нефискального документа **/
        if (m_kkm.beginNonfiscalDocument() < 0) {
            return result.fail(*this); // throw Failure(*this); // NOLINT(*-exception-baseclass)
        }
        if (details.m_cliche && m_kkm.printCliche() < 0) {
            return result.fail(*this); // throw Failure(*this); // NOLINT(*-exception-baseclass)
        }

        for (auto & block : details.m_document) {
            subPrintText(block);
        }

        tsLogDebug(Wcs::c_subPrint, m_logPrefix, m_serialNumber);

        /** Закрытие нефискального документа **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_PRINT_FOOTER, details.m_footer);
        if (m_kkm.endNonfiscalDocument() < 0) {
            return result.fail(*this); // throw Failure(*this); // NOLINT(*-exception-baseclass)
        }
    }

    void Device::printInfo(Call::Result & result) {
        tsLogDebug(Wcs::c_printInfoMethod, m_logPrefix, m_serialNumber);
        tsLogDebug(Wcs::c_subPrint, m_logPrefix, m_serialNumber);

        /** Печать информации о ККТ **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_REPORT_TYPE, Atol::LIBFPTR_RT_KKT_INFO);
        if (m_kkm.report() < 0) {
            return result.fail(*this); // throw Failure(*this); // NOLINT(*-exception-baseclass)
        }
    }

    void Device::printFnRegistrations(Call::Result & result) {
        tsLogDebug(Wcs::c_printFnRegistrationsMethod, m_logPrefix, m_serialNumber);
        tsLogDebug(Wcs::c_subPrint, m_logPrefix, m_serialNumber);

        /** Печать итогов регистрации / перерегистрации **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_REPORT_TYPE, Atol::LIBFPTR_RT_FN_REGISTRATIONS);
        if (m_kkm.report() < 0) {
            return result.fail(*this); // throw Failure(*this); // NOLINT(*-exception-baseclass)
        }
    }

    void Device::printOfdExchangeStatus(Call::Result & result) {
        tsLogDebug(Wcs::c_printOfdExchangeStatusMethod, m_logPrefix, m_serialNumber);
        tsLogDebug(Wcs::c_subPrint, m_logPrefix, m_serialNumber);

        /** Отчет о состоянии расчетов **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_REPORT_TYPE, Atol::LIBFPTR_RT_OFD_EXCHANGE_STATUS);
        if (m_kkm.report() < 0) {
            return result.fail(*this); // throw Failure(*this); // NOLINT(*-exception-baseclass)
        }
        subCheckDocumentClosed(result);
    }

    void Device::printOfdTest(Call::Result & result) {
        tsLogDebug(Wcs::c_printOfdTestMethod, m_logPrefix, m_serialNumber);
        tsLogDebug(Wcs::c_subPrint, m_logPrefix, m_serialNumber);

        /** Диагностика соединения с ОФД **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_REPORT_TYPE, Atol::LIBFPTR_RT_OFD_TEST);
        if (m_kkm.report() < 0) {
            return result.fail(*this); // throw Failure(*this); // NOLINT(*-exception-baseclass)
        }
    }

    void Device::printCloseShiftReports(Call::Result & result) {
        tsLogDebug(Wcs::c_printShiftReportsMethod, m_logPrefix, m_serialNumber);
        tsLogDebug(Wcs::c_subPrint, m_logPrefix, m_serialNumber);

        /** Печать нераспечатанных отчетов о закрытии смены **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_REPORT_TYPE, Atol::LIBFPTR_RT_CLOSE_SHIFT_REPORTS);
        if (m_kkm.report() < 0) {
            return result.fail(*this); // throw Failure(*this); // NOLINT(*-exception-baseclass)
        }
    }

    void Device::printLastDocument(Call::Result & result) {
        tsLogDebug(Wcs::c_printLastDocumentMethod, m_logPrefix, m_serialNumber);
        tsLogDebug(Wcs::c_subPrint, m_logPrefix, m_serialNumber);

        /** Копия последнего документа **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_REPORT_TYPE, Atol::LIBFPTR_RT_LAST_DOCUMENT);
        if (m_kkm.report() < 0) {
            return result.fail(*this); // throw Failure(*this); // NOLINT(*-exception-baseclass)
        }
    }

    Device::Call::OperatorDetails::OperatorDetails(const Nln::Json & details)
    : Details() {
        bool result = Json::handleKey(
            details, "operator",
            [this] (const Nln::Json & json, const std::wstring & path) -> bool {
                using NameType = decltype(this->m_operatorName);
                auto success
                    = Json::handleKey(
                        json, "name",
                        this->m_operatorName,
                        Text::maxLength<NameType>(c_maxLength, Text::trim<NameType>()),
                        path
                    );
                if (!success) {
                    throw Failure(std::format(Wcs::c_requiresProperty, L"operator.name")); // NOLINT(*-exception-baseclass)
                }
                Json::handleKey(json, "inn", this->m_operatorInn, path);
                return true;
            }
        );
        if (!result) {
            throw Failure(std::format(Wcs::c_requiresProperty, L"operator")); // NOLINT(*-exception-baseclass)
        }
    }

    Device::Call::OperatorDetails::OperatorDetails(const std::wstring_view name, const std::wstring_view inn)
    : Details(), m_operatorName(name), m_operatorInn(inn) {}

    Device::Call::CashDetails::CashDetails(const Nln::Json & details)
    : OperatorDetails(details) {
        if (!Json::handleKey(details, "cashSum", m_cashSum, Numeric::between(c_minCashInOut, s_maxCashInOut))) {
            throw Failure(std::format(Wcs::c_requiresProperty, L"cashSum")); // NOLINT(*-exception-baseclass)
        }
    }

    void Device::registerCashIn(const Call::CashDetails & details, Call::Result & result) {
        tsLogDebug(Wcs::c_cashInMethod, m_logPrefix, m_serialNumber);

        subSetOperator(details);

        tsLogDebug(Wcs::c_subRegisterCashInAndPrint, m_logPrefix, m_serialNumber);

        /** Внесение **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_SUM, details.m_cashSum);
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DOCUMENT_ELECTRONICALLY, details.m_electronically);
        if (m_kkm.cashIncome() < 0) {
            return result.fail(*this); // throw Failure(*this); // NOLINT(*-exception-baseclass)
        }
    }

    void Device::registerCashOut(const Call::CashDetails & details, Call::Result & result) {
        tsLogDebug(Wcs::c_cashOutMethod, m_logPrefix, m_serialNumber);

        subSetOperator(details);

        tsLogDebug(Wcs::c_subRegisterCashOutAndPrint, m_logPrefix, m_serialNumber);

        /** Выплата **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_SUM, details.m_cashSum);
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DOCUMENT_ELECTRONICALLY, details.m_electronically);
        if (m_kkm.cashOutcome() < 0) {
            return result.fail(*this); // throw Failure(*this); // NOLINT(*-exception-baseclass)
        }
    }

    Device::Call::ReceiptDetails::ItemDetails::ItemDetails(
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

    Device::Call::ReceiptDetails::ItemDetails::ItemDetails(
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

    Device::Call::ReceiptDetails::ReceiptDetails(const Nln::Json & details)
    : OperatorDetails(details) {
        Json::handleKey(
            details, "customer",
            [this] (const Nln::Json & json, const std::wstring & path) -> bool {
                Json::handleKey(json, "account", this->m_customerAccount, path);
                Json::handleKey(json, "contact", this->m_customerContact, path);
                Json::handleKey(json, "name", this->m_customerName, path);
                Json::handleKey(json, "inn", this->m_customerInn, path);
                Json::handleKey(json, "birthdate", this->m_customerBirthdate, path);
                Json::handleKey(json, "citizenship", this->m_customerCitizenship, path);
                Json::handleKey(json, "documentCode", this->m_customerDocumentCode, path);
                Json::handleKey(json, "documentData", this->m_customerDocumentData, path);
                Json::handleKey(json, "address", this->m_customerAddress, path);
                this->m_customerDataIsPresent
                    = !this->m_customerAccount.empty() || !this->m_customerContact.empty()
                    || !this->m_customerName.empty() || !this->m_customerInn.empty()
                    || !this->m_customerBirthdate.empty() || !this->m_customerCitizenship.empty()
                    || !this->m_customerDocumentCode.empty() || !this->m_customerDocumentData.empty()
                    || !this->m_customerAddress.empty();
                return true;
            }
        );
        Json::handleKey(
            details, "text",
            [this] (const Nln::Json & json, const std::wstring & path) -> bool {
                Json::handleKey(json, "content", this->m_text.m_content, path);
                Json::handleKey(json, "center", this->m_text.m_center, path);
                Json::handleKey(json, "magnified", this->m_text.m_magnified, path);
                Json::handleKey(json, "separated", this->m_text.m_separated, path);
                return true;
            }
        );
        Json::handleKey(
            details, "headerText",
            [this] (const Nln::Json & json, const std::wstring & path) -> bool {
                Json::handleKey(json, "content", this->m_headerText.m_content, path);
                Json::handleKey(json, "center", this->m_headerText.m_center, path);
                Json::handleKey(json, "magnified", this->m_headerText.m_magnified, path);
                Json::handleKey(json, "separated", this->m_headerText.m_separated, path);
                return true;
            }
        );
        Json::handleKey(
            details, "footerText",
            [this] (const Nln::Json & json, const std::wstring & path) -> bool {
                Json::handleKey(json, "content", this->m_footerText.m_content, path);
                Json::handleKey(json, "center", this->m_footerText.m_center, path);
                Json::handleKey(json, "magnified", this->m_footerText.m_magnified, path);
                Json::handleKey(json, "separated", this->m_footerText.m_separated, path);
                return true;
            }
        );
        Tax defaultTax { Tax::No };
        bool hasDefaultTax = Json::handleKey(details, "tax", defaultTax, s_taxCastMap);
        if (details.contains("items") && details["items"].is_array()) {
            for (auto & item : details["items"].get<std::vector<Nln::Json>>()) {
                std::wstring title;
                if (!Json::handleKey(item, "title", title, Text::Wcs::noEmpty(Text::Wcs::trim()))) {
                    throw Failure(std::format(Wcs::c_requiresProperty, L"items[].title")); // NOLINT(*-exception-baseclass)
                }
                double price;
                if (!Json::handleKey(item, "price", price, Numeric::between(c_minPrice, s_maxPrice))) {
                    throw Failure(std::format(Wcs::c_requiresProperty, L"items[].price")); // NOLINT(*-exception-baseclass)
                }
                double quantity;
                if (!Json::handleKey(item, "quantity", quantity, Numeric::between(c_minQuantity, s_maxQuantity))) {
                    throw Failure(std::format(Wcs::c_requiresProperty, L"items[].quantity")); // NOLINT(*-exception-baseclass)
                }
                MeasurementUnit unit { MeasurementUnit::Piece };
                Json::handleKey(item, "unit", unit, s_measurementUnitMap);
                Tax tax;
                if (!Json::handleKey(item, "tax", tax, s_taxCastMap)) {
                    if (hasDefaultTax) {
                        tax = defaultTax;
                    } else {
                        throw Failure(std::format(Wcs::c_requiresProperty, L"items[].tax")); // NOLINT(*-exception-baseclass)
                    }
                }
                m_paymentSum += price * quantity;
                m_items.emplace_back(std::move(title), price, quantity, unit, tax);
            }
        } else {
            throw Failure(std::format(Wcs::c_requiresProperty, L"items")); // NOLINT(*-exception-baseclass)
        }
        bool payment = Json::handleKey(
            details,
            "payment",
            [this] (const Nln::Json & json, const std::wstring & path) -> bool {
                std::wstring sum {};
                if (Json::handleKey(json, "sum", sum, path)) {
                    try {
                        Text::lower(sum);
                        if (sum == L"auto") {
                            this->m_paymentSum = -1;
                        } else {
                            this->m_paymentSum = Text::cast<double>(sum);
                        }
                    } catch (...) {
                        throw Failure(std::format(Wcs::c_requiresProperty, L"payment.sum")); // NOLINT(*-exception-baseclass)
                    }
                } else {
                    throw Failure(std::format(Wcs::c_requiresProperty, L"payment.sum")); // NOLINT(*-exception-baseclass)
                }
                if (!Json::handleKey(json, "type", this->m_paymentType, s_paymentTypeCastMap, path)) {
                    throw Failure(std::format(Wcs::c_requiresProperty, L"payment.type")); // NOLINT(*-exception-baseclass)
                }
                return true;
            }
        );
        if (!payment) {
            throw Failure(std::format(Wcs::c_requiresProperty, L"payment")); // NOLINT(*-exception-baseclass)
        }
    }

    void Device::subRegisterReceipt(
        const ReceiptType type,
        const Call::ReceiptDetails & details,
        Call::Result & result
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

        /** Открытие печатного чека **/
        subSetOperator(details);
        if (details.m_customerDataIsPresent) {
            subSetCustomer(details);
        }
        m_kkm.setParam(Atol::LIBFPTR_PARAM_RECEIPT_TYPE, static_cast<unsigned>(type));
        m_kkm.setParam(Atol::LIBFPTR_PARAM_RECEIPT_ELECTRONICALLY, details.m_electronically); // Открытие электрочека
        if (m_kkm.openReceipt() < 0) {
            return result.fail(*this); // throw Failure(*this); // NOLINT(*-exception-baseclass)
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

        tsLogDebug(Wcs::c_subRegisterItems, m_logPrefix, m_serialNumber);

        /** Регистрация позиций **/
        for (auto & item : details.m_items) {
            m_kkm.setParam(Atol::LIBFPTR_PARAM_COMMODITY_NAME, item.m_commodityName);
            m_kkm.setParam(Atol::LIBFPTR_PARAM_PRICE, item.m_price);
            m_kkm.setParam(Atol::LIBFPTR_PARAM_QUANTITY, item.m_quantity);
            m_kkm.setParam(Atol::LIBFPTR_PARAM_MEASUREMENT_UNIT, static_cast<int>(item.m_unit));
            m_kkm.setParam(Atol::LIBFPTR_PARAM_TAX_TYPE, static_cast<int>(item.m_tax));
            if (m_kkm.registration() < 0) {
                return result.fail(*this); // throw Failure(*this); // NOLINT(*-exception-baseclass)
            }
        }

        tsLogDebug(Wcs::c_subRegisterPayment, m_logPrefix, m_serialNumber);

        /** Оплата чека **/
        if (details.m_paymentSum > 0) {
            m_kkm.setParam(Atol::LIBFPTR_PARAM_PAYMENT_TYPE, static_cast<int>(details.m_paymentType));
            m_kkm.setParam(Atol::LIBFPTR_PARAM_PAYMENT_SUM, details.m_paymentSum);
            if (m_kkm.payment() < 0) {
                return result.fail(*this); // throw Failure(*this); // NOLINT(*-exception-baseclass)
            }
            // result.m_remainder = m_kkm.getParamDouble(Atol::LIBFPTR_PARAM_REMAINDER); // Неоплаченный остаток чека
            // result.m_change = m_kkm.getParamDouble(Atol::LIBFPTR_PARAM_CHANGE); // Сдача по чеку
        }

        tsLogDebug(Wcs::c_subRegisterReceiptAndPrint, m_logPrefix, m_serialNumber);

        /** Закрытие чека **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_PAYMENT_TYPE, static_cast<int>(details.m_paymentType));
        if (m_kkm.closeReceipt() < 0) {
            return result.fail(*this); // throw Failure(*this); // NOLINT(*-exception-baseclass)
        }

        m_needToCancelReceipt = false;
        subCheckDocumentClosed(result);
    }

    void Device::registerSell(const Call::ReceiptDetails & details, Call::Result & result) {
        tsLogDebug(Wcs::c_sellMethod, m_logPrefix, m_serialNumber);
        subRegisterReceipt(ReceiptType::Sell, details, result);
    }

    void Device::registerSellReturn(const Call::ReceiptDetails & details, Call::Result & result) {
        tsLogDebug(Wcs::c_sellReturnMethod, m_logPrefix, m_serialNumber);
        subRegisterReceipt(ReceiptType::SellReturn, details, result);
    }

    Device::Call::CloseDetails::CloseDetails(const Nln::Json & details)
    : OperatorDetails(details) {
        Json::handleKey(details, "closeShift", m_closeShift);
        Json::handleKey(details, "cashOut", m_cashOut);
    }

    Device::Call::CloseDetails::CloseDetails(
        const std::wstring_view name,
        const std::wstring_view inn,
        const bool closeShift,
        const bool cashOut
    ) : OperatorDetails(name, inn), m_closeShift(closeShift), m_cashOut(cashOut) {}

    void Device::subCloseShift(const Call::OperatorDetails & details, Call::Result & result) {
        /** Запрос состояния смены **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DATA_TYPE, Atol::LIBFPTR_DT_SHIFT_STATE);
        if (m_kkm.queryData() < 0) {
            return result.fail(*this); // throw Failure(*this); // NOLINT(*-exception-baseclass)
        }
        if (static_cast<ShiftState>(m_kkm.getParamInt(Atol::LIBFPTR_PARAM_SHIFT_STATE)) == ShiftState::Closed) {
            tsLogDebug(Wcs::c_subCloseShiftNoNeed, m_logPrefix, m_serialNumber);
        } else {
            subSetOperator(details);
            tsLogDebug(Wcs::c_subCloseShift, m_logPrefix, m_serialNumber);
            // tsLogDebug(Wcs::c_subPrint, m_logPrefix, m_serialNumber);
            /** Закрытие смены **/
            m_kkm.setParam(Atol::LIBFPTR_PARAM_REPORT_TYPE, Atol::LIBFPTR_RT_CLOSE_SHIFT);
            if (m_kkm.report() < 0) {
                return result.fail(*this); // throw Failure(*this); // NOLINT(*-exception-baseclass)
            }
            subCheckDocumentClosed(result);
        }
    }

    void Device::subCashOut(const Call::OperatorDetails & details, Call::Result & result) {
        /** Запрос суммы наличных в денежном ящике **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DATA_TYPE, Atol::LIBFPTR_DT_CASH_SUM);
        if (m_kkm.queryData() < 0) {
            return result.fail(*this); // throw Failure(*this); // NOLINT(*-exception-baseclass)
        }
        double cashSum { m_kkm.getParamDouble(Atol::LIBFPTR_PARAM_SUM) };
        if (cashSum > 0) {
            subSetOperator(details);
            tsLogDebug(Wcs::c_subCashOut, m_logPrefix, m_serialNumber);
            /** Выплата **/
            m_kkm.setParam(Atol::LIBFPTR_PARAM_SUM, cashSum);
            m_kkm.setParam(Atol::LIBFPTR_PARAM_DOCUMENT_ELECTRONICALLY, details.m_electronically);
            if (m_kkm.cashOutcome() < 0) {
                return result.fail(*this); // throw Failure(*this); // NOLINT(*-exception-baseclass)
            }
        } else {
            tsLogDebug(Wcs::c_subCashOutNoNeed, m_logPrefix, m_serialNumber);
        }
    }

    void Device::closeShift(const Call::CloseDetails & details, Call::Result & result) {
        tsLogDebug(Wcs::c_closeShiftMethod, m_logPrefix, m_serialNumber);

        if (details.m_cashOut) {
            subCashOut(details, result);
        }

        subCloseShift(details, result);
    }

    void Device::reportX(const Call::CloseDetails & details, Call::Result & result) {
        tsLogDebug(Wcs::c_reportXMethod, m_logPrefix, m_serialNumber);

        if (details.m_cashOut) {
            subCashOut(details, result);
        }
        if (details.m_closeShift) {
            subCloseShift(details, result);
        }

        tsLogDebug(Wcs::c_subPrint, m_logPrefix, m_serialNumber);

        /** Печать X-отчета **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_REPORT_TYPE, Atol::LIBFPTR_RT_X);
        if (m_kkm.report() < 0) {
            result.fail(m_kkm.errorDescription());
        }
    }

    void Device::resetState(const Call::CloseDetails & details, Call::Result & result) {
        tsLogDebug(Wcs::c_resetStateMethod, m_logPrefix, m_serialNumber);

        /** Запрос состояния чека **/
        m_kkm.setParam(Atol::LIBFPTR_PARAM_DATA_TYPE, Atol::LIBFPTR_DT_RECEIPT_STATE);
        if (m_kkm.queryData() < 0) {
            return result.fail(*this); // throw Failure(*this); // NOLINT(*-exception-baseclass)
        }
        if (static_cast<ReceiptType>(m_kkm.getParamInt(Atol::LIBFPTR_PARAM_RECEIPT_TYPE)) == ReceiptType::Closed) {
            tsLogDebug(Wcs::c_subCancelReceiptNoNeed, m_logPrefix, m_serialNumber);
        } else {
            tsLogDebug(Wcs::c_subCancelReceipt, m_logPrefix, m_serialNumber);
            /** Отмена чека **/
            if (m_kkm.cancelReceipt() < 0) {
                return result.fail(*this); // throw Failure(*this); // NOLINT(*-exception-baseclass)
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
