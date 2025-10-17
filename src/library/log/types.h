// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <lib/meta.h>

namespace Log {
    enum class Level { Debug = 0, Info = 1, Warning = 2, Error = 3 };

    using LevelUnderlying = std::underlying_type_t<Level>;

    constexpr LevelUnderlying c_levelDebug [[maybe_unused]] { Meta::toUnderlying(Level::Debug) };
    constexpr LevelUnderlying c_levelInfo [[maybe_unused]] { Meta::toUnderlying(Level::Info) };
    constexpr LevelUnderlying c_levelWarning [[maybe_unused]] { Meta::toUnderlying(Level::Warning) };
    constexpr LevelUnderlying c_levelError [[maybe_unused]] { Meta::toUnderlying(Level::Error) };
    constexpr LevelUnderlying c_levelNone [[maybe_unused]] { c_levelError + 1 };
}
