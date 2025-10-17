// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "types.h"
#include <filesystem>
#include <string>
#include <vector>

namespace Kkm {
    class ConnParams {
    public:
        using Container = std::vector<std::wstring>;

        ConnParams(const ConnParams &) = default;
        ConnParams(ConnParams &&) noexcept = default;
        ~ConnParams() = default;

        ConnParams & operator=(const ConnParams &) = default;
        ConnParams & operator=(ConnParams &&) noexcept = default;
        explicit operator std::wstring() const;

        void apply(Atol::Fptr &) const;

    protected:
        Container m_params {};

        ConnParams() = default;
        explicit ConnParams(const Container &);
        explicit ConnParams(Container &&);

        void applyCommon(Atol::Fptr &) const;
        void applyCom(Atol::Fptr &) const;
        void applyUsb(Atol::Fptr &) const;
        void applyTcpIp(Atol::Fptr &) const;
        void applyBluetooth(Atol::Fptr &) const;
    };

    class NewConnParams : public ConnParams {
        friend class KnownConnParams;

    public:
        NewConnParams() = delete;
        NewConnParams(const NewConnParams &) = default;
        NewConnParams(NewConnParams &&) noexcept = default;
        explicit NewConnParams(std::wstring_view);
        ~NewConnParams() = default;

        NewConnParams & operator=(const NewConnParams &) = default;
        NewConnParams & operator=(NewConnParams &&) noexcept = default;

        void save(const std::wstring &) const;
    };

    class KnownConnParams : public ConnParams {
    public:
        KnownConnParams() = delete;
        KnownConnParams(const KnownConnParams &) = default;
        KnownConnParams(KnownConnParams &&) noexcept = default;
        KnownConnParams(const NewConnParams &, std::wstring_view);
        KnownConnParams(NewConnParams &&, std::wstring_view);
        explicit KnownConnParams(std::wstring);
        explicit KnownConnParams(const std::filesystem::path &);
        ~KnownConnParams() = default;

        KnownConnParams & operator=(const KnownConnParams &) = default;
        KnownConnParams & operator=(KnownConnParams &&) noexcept = default;

        [[nodiscard]] const std::wstring & serialNumber() const;

    protected:
        std::wstring m_serialNumber {};

        void load(const std::filesystem::path &);

        [[nodiscard]] static std::wstring serialNumber(const std::filesystem::path &);
        [[nodiscard]] static std::filesystem::path filePath(const std::wstring &);
        [[nodiscard]] static std::wstring filterSerialNumber(std::wstring);
        [[nodiscard]] static std::filesystem::path filterFilePath(std::filesystem::path);
    };
}
