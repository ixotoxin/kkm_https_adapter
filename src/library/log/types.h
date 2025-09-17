// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <lib/meta.h>

namespace Log {
    enum class Level { Debug = 0, Info = 1, Warning = 2, Error = 3 };

    constexpr const std::underlying_type_t<Level> c_levelDebug [[maybe_unused]] { Meta::toUnderlying(Level::Debug) };
    constexpr const std::underlying_type_t<Level> c_levelInfo [[maybe_unused]] { Meta::toUnderlying(Level::Info) };
    constexpr const std::underlying_type_t<Level> c_levelWarning [[maybe_unused]] { Meta::toUnderlying(Level::Warning) };
    constexpr const std::underlying_type_t<Level> c_levelError [[maybe_unused]] { Meta::toUnderlying(Level::Error) };
    constexpr const std::underlying_type_t<Level> c_levelNone [[maybe_unused]] { c_levelError + 1 };
}
