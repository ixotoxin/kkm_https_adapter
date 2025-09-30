// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "connparams.h"
#include "defaults.h"
#include "variables.h"
#include "strings.h"
#include <lib/json.h>
#include <fstream>

namespace Kkm {
    using namespace std::string_literals;

    ConnParams::ConnParams(const Container & params)
    : m_params { params } {}

    ConnParams::ConnParams(Container && params)
    : m_params { std::forward<Container>(params) } {}

    ConnParams::operator std::wstring() const {
        std::wstring params;
        Text::joinTo(params, m_params, c_connParamsSeparator);
        return params;
    }

    void ConnParams::apply(Atol::Fptr & kkm) const {
        if (m_params[0] == L"com"s) {
            applyCom(kkm);
        } else if (m_params[0] == L"usb"s) {
            applyUsb(kkm);
        } else if (m_params[0] == L"tcpip"s || m_params[0] == L"ip"s) {
            applyTcpIp(kkm);
        } else if (m_params[0] == L"bluetooth"s || m_params[0] == L"bt"s) {
            applyBluetooth(kkm);
        } else {
            throw Failure(Wcs::c_invalidConnParams); // NOLINT(*-exception-baseclass)
        }
    }

    void ConnParams::applyCommon(Atol::Fptr & kkm) const { // NOLINT(*-convert-member-functions-to-static)
        kkm.setSingleSetting(Atol::LIBFPTR_SETTING_MODEL, std::to_wstring(Atol::LIBFPTR_MODEL_ATOL_AUTO));
        if (s_timeZoneConfigured) {
            kkm.setSingleSetting(Atol::LIBFPTR_SETTING_TIME_ZONE, std::to_wstring(Meta::toUnderlying(s_timeZone)));
        }
        kkm.setSingleSetting(Atol::LIBFPTR_SETTING_OFD_CHANNEL, std::to_wstring(Atol::LIBFPTR_OFD_CHANNEL_AUTO));
        // ISSUE: Из документации не ясно, что передавать в качестве значения параметра.
        //  Но нам этот параметр не очень нужен, потому как при формировании чека единицы у нас
        //  всегда передаются и проблем с ФФД 1.2+ не должно возникнуть.
        // kkm.setSingleSetting(Atol::LIBFPTR_SETTING_AUTO_MEASUREMENT_UNIT, ???);
    }

    void ConnParams::applyCom(Atol::Fptr & kkm) const {
        if (m_params.size() < 2) {
            throw Failure(Wcs::c_invalidConnParams); // NOLINT(*-exception-baseclass)
        }
        std::wstring port { L"COM" };
        port.append(m_params[1]);
        std::wstring baudRate {};
        if (m_params.size() < 3) {
            baudRate.assign(s_defaultBaudRate);
        } else if (std::ranges::find(Wcs::c_allowedBaudRate, m_params[2]) != Wcs::c_allowedBaudRate.end()) {
            baudRate.assign(m_params[2]);
        } else {
            throw Failure(Wcs::c_invalidConnParams); // NOLINT(*-exception-baseclass)
        }
        kkm.setSingleSetting(Atol::LIBFPTR_SETTING_PORT, std::to_wstring(Atol::LIBFPTR_PORT_COM));
        kkm.setSingleSetting(Atol::LIBFPTR_SETTING_COM_FILE, port);
        kkm.setSingleSetting(Atol::LIBFPTR_SETTING_BAUDRATE, baudRate);
        applyCommon(kkm);
        if (kkm.applySingleSettings() < 0) {
            throw Failure(kkm); // NOLINT(*-exception-baseclass)
        }
    }

    void ConnParams::applyUsb(Atol::Fptr &) const { // NOLINT(*-convert-member-functions-to-static)
        // TODO: Реализовать.
        // if (m_params.size() < 2) {
        //     throw Failure(Wcs::c_invalidConnParams); // NOLINT(*-exception-baseclass)
        // }
        // kkm.setSingleSetting(Atol::LIBFPTR_SETTING_PORT, std::to_wstring(Atol::LIBFPTR_PORT_USB));
        // ...
        throw Failure(Wcs::c_notImplemented); // NOLINT(*-exception-baseclass)
    }

    void ConnParams::applyTcpIp(Atol::Fptr &) const { // NOLINT(*-convert-member-functions-to-static)
        // TODO: Реализовать.
        // if (m_params.size() < 2) {
        //     throw Failure(Wcs::c_invalidConnParams); // NOLINT(*-exception-baseclass)
        // }
        // kkm.setSingleSetting(Atol::LIBFPTR_SETTING_PORT, std::to_wstring(Atol::LIBFPTR_PORT_TCPIP));
        // ...
        throw Failure(Wcs::c_notImplemented); // NOLINT(*-exception-baseclass)
    }

    void ConnParams::applyBluetooth(Atol::Fptr &) const { // NOLINT(*-convert-member-functions-to-static)
        // TODO: Реализовать.
        // if (m_params.size() < 2) {
        //     throw Failure(Wcs::c_invalidConnParams); // NOLINT(*-exception-baseclass)
        // }
        // kkm.setSingleSetting(Atol::LIBFPTR_SETTING_PORT, std::to_wstring(Atol::LIBFPTR_PORT_BLUETOOTH));
        // ...
        throw Failure(Wcs::c_notImplemented); // NOLINT(*-exception-baseclass)
    }

    NewConnParams::NewConnParams(const std::wstring_view params)
    : ConnParams {} {
        Text::splitTo(m_params, params, c_connParamsSeparator);
        if (m_params.size() < 2) {
            throw Failure(Wcs::c_invalidConnParams); // NOLINT(*-exception-baseclass)
        }
        Text::lower(m_params[0]);
    }

    void NewConnParams::save(const std::wstring & serialNumber) {
        if (serialNumber.empty()) {
            throw Failure(KKM_WFMT(Wcs::c_savingError, L"-")); // NOLINT(*-exception-baseclass)
        }
        std::filesystem::path filePath { s_dbDirectory };
        if (!std::filesystem::is_directory(filePath)) {
            std::filesystem::create_directories(filePath);
            if (!std::filesystem::is_directory(filePath)) {
                throw Failure(KKM_WFMT(Wcs::c_savingError, serialNumber)); // NOLINT(*-exception-baseclass)
            }
        }
        filePath /= serialNumber + L".json";
        Nln::Json json = Text::convert(m_params);
        std::ofstream file { filePath };
        file << json.dump();
        file.close();
        if (!std::filesystem::is_regular_file(filePath)) {
            throw Failure(KKM_WFMT(Wcs::c_savingError, serialNumber)); // NOLINT(*-exception-baseclass)
        }
    }

    KnownConnParams::KnownConnParams(const NewConnParams & params, const std::wstring_view serialNumber)
    : ConnParams { params.m_params }, m_serialNumber { serialNumber } {}

    KnownConnParams::KnownConnParams(NewConnParams && params, const std::wstring_view serialNumber)
    : ConnParams { std::forward<Container>(params.m_params) }, m_serialNumber { serialNumber } {}

    void KnownConnParams::load(const std::filesystem::path & path) {
        assert(m_params.empty());
        std::ifstream file { path };
        if (!file.is_open() || !file.good()) {
            throw Failure(LIB_WFMT(Basic::Wcs::c_couldntReadFile, path.c_str())); // NOLINT(*-exception-baseclass)
        }
        Nln::Json json(Nln::Json::parse(std::ifstream(path)));
        Json::handle(json, m_params);
    }

    KnownConnParams::KnownConnParams(std::wstring serial)
    : ConnParams {}, m_serialNumber { std::move(serial) } {
        load(filePath(m_serialNumber));
    }

    KnownConnParams::KnownConnParams(const std::filesystem::path & path)
    : ConnParams {}, m_serialNumber { serialNumber(path) } {
        load(path);
    }

    [[nodiscard]]
    const std::wstring & KnownConnParams::serialNumber() const {
        return m_serialNumber;
    }

    [[nodiscard]]
    std::wstring KnownConnParams::serialNumber(const std::filesystem::path & filePath) {
        auto fileName = filePath.filename();
        fileName.replace_extension(L"");
        return fileName.wstring();
    }

    [[nodiscard]]
    std::filesystem::path KnownConnParams::filePath(const std::wstring & serialNumber) {
        std::filesystem::path path { s_dbDirectory };
        path /= serialNumber + L".json"s;
        if (!std::filesystem::is_regular_file(path)) {
            throw Failure(KKM_WFMT(Wcs::c_loadingError, serialNumber)); // NOLINT(*-exception-baseclass)
        }
        return path;
    }
}
