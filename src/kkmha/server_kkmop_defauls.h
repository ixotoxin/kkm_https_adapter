// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <lib/datetime.h>

namespace Server::KkmOp {
    using namespace std::chrono_literals;

    constexpr const DateTime::Offset c_reportCacheLifeTime { 5s }; // Секунды
    constexpr const DateTime::Offset c_receiptCacheLifeTime { 345'600s }; // Секунды
}
