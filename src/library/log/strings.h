// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "types.h"
#include <string>
#include <unordered_map>

namespace Log::Wcs {
    constexpr const wchar_t * c_loggingError { L"[ logging error ]" };

    inline const std::unordered_map<Level, std::wstring_view> c_levelLabels {
        { Level::Debug, L"DBG" },
        { Level::Info, L"INF" },
        { Level::Warning, L"WRN" },
        { Level::Error, L"ERR" }
    };

    inline const std::unordered_map<std::wstring, int> c_levelCastMap {
        { L"dbg", c_levelDebug },
        { L"debug", c_levelDebug },
        { std::to_wstring(c_levelDebug), c_levelDebug },
        { L"inf", c_levelInfo },
        { L"info", c_levelInfo },
        { std::to_wstring(c_levelInfo), c_levelInfo },
        { L"wrn", c_levelWarning },
        { L"warning", c_levelWarning },
        { std::to_wstring(c_levelWarning), c_levelWarning },
        { L"err", c_levelError },
        { L"error", c_levelError },
        { std::to_wstring(c_levelError), c_levelError },
        { L"no", c_levelNone },
        { L"non", c_levelNone },
        { L"none", c_levelNone },
        { std::to_wstring(c_levelNone), c_levelNone }
    };
}
