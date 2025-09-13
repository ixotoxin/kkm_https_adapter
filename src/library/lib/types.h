// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

consteval size_t operator""_size_t(const unsigned long long value) noexcept { return value; }
