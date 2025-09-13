// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

namespace Log {
    enum class Level : int { Debug, Info, Warning, Error };

    constexpr const int c_levelDebug [[maybe_unused]] { static_cast<int>(Level::Debug) };
    constexpr const int c_levelInfo [[maybe_unused]] { static_cast<int>(Level::Info) };
    constexpr const int c_levelWarning [[maybe_unused]] { static_cast<int>(Level::Warning) };
    constexpr const int c_levelError [[maybe_unused]] { static_cast<int>(Level::Error) };
    constexpr const int c_levelNone [[maybe_unused]] { c_levelError + 1 };
}
