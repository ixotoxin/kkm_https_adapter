// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "http.h"

namespace Http {
    class ConfigHandler : public RequestHandler {
    public:
        ConfigHandler() = default;
        ConfigHandler(const ConfigHandler &) = default;
        ConfigHandler(ConfigHandler &&) = default;
        ~ConfigHandler() override = default;

        ConfigHandler & operator=(const ConfigHandler &) = default;
        ConfigHandler & operator=(ConfigHandler &&) = default;

        [[nodiscard]] bool asyncReady() const noexcept override;
        // [[nodiscard]] bool isCaching() const noexcept override;
        void operator()(Request &) const noexcept override;
    };
}
