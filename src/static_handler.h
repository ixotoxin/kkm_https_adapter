// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "http.h"
#include "defaults.h"

namespace Http {
    class StaticHandler : public Http::RequestHandler {
    public:
        StaticHandler() = default;
        StaticHandler(const StaticHandler &) = default;
        StaticHandler(StaticHandler &&) = default;
        ~StaticHandler() override = default;

        StaticHandler &operator=(const StaticHandler &) = default;
        StaticHandler &operator=(StaticHandler &&) = default;

        [[nodiscard]] bool asyncReady() const noexcept override;
        [[nodiscard]] bool isCaching() const noexcept override;
        void operator()(Request &) const noexcept override;
    };

    inline bool s_enableStatic { c_enableStatic };
    inline std::wstring s_staticDirectory { c_staticDirectory };
    inline std::wstring s_mimeMapFile { c_mimeMapFile };
    inline Nln::Json s_mimeMap(Nln::EmptyJsonObject);
    inline bool s_enableUnknownType { c_enableUnknownType };
}
