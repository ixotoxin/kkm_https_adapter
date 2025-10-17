// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "asio.h"
#include "server_strings.h"
#include "http_request.h"
#include <lib/except.h>
#include <lib/text.h>

namespace Server {
    class Failure final : public Basic::Failure {
    public:
        Failure() = delete;
        Failure(const Failure &) = default;
        Failure(Failure &&) noexcept = default;

        explicit Failure(
            const std::string_view operation,
            const Asio::Error error,
            SrcLoc::Point && location = SrcLoc::Point::current()
        ) : Basic::Failure(
                Text::concat(operation, ": ", error.message()),
                std::forward<SrcLoc::Point>(location)
            ) {}

        explicit Failure(
            const Http::Request::IdType id,
            const std::string_view operation,
            const Asio::Error error,
            SrcLoc::Point && location = SrcLoc::Point::current()
        ) : Basic::Failure(
                std::format(Mbs::c_prefixedOperation, id, operation, error.message()),
                std::forward<SrcLoc::Point>(location)
            ) {}

        ~Failure() override = default;

        Failure & operator=(const Failure &) = default;
        Failure & operator=(Failure &&) noexcept = default;
    };
}
