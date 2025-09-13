// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <lib/datetime.h>

namespace Server::KkmOp {
    constexpr const DateTime::OffsetValue c_reportCacheLifeTime { 5 }; // Секунды
    constexpr const DateTime::OffsetValue c_receiptCacheLifeTime { 345'600 }; // Секунды
}
