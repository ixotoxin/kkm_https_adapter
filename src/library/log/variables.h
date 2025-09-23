// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "macro.h"
#include "defaults.h"
#include "types.h"
#include <lib/json.h>
#include <string>
#include <mutex>

namespace Log {
    namespace Console {
        LOG_VAR(LevelUnderlying, s_level, c_levelDebug);
        LOG_VAR(bool, s_outputTimestamp, c_outputTimestamp);
        LOG_VAR(bool, s_outputLevel, c_outputLevel);
    }

    namespace File {
        LOG_VAR(LevelUnderlying, s_fgLevel, c_levelNone);
        LOG_VAR(LevelUnderlying, s_bgLevel, c_levelInfo);
        LOG_VAR(std::filesystem::path, s_directory, c_directory);
    }

    namespace EventLog {
        LOG_VAR(LevelUnderlying, s_fgLevel, c_levelNone);
        LOG_VAR(LevelUnderlying, s_bgLevel, c_levelInfo);
    }

    namespace Ts {
        inline std::mutex s_logMutex;
    }

#ifdef EXTERNAL_LOG_VARIABLES
    extern bool s_appendLocation;
#else
#   ifdef DEBUG
    inline bool s_appendLocation { true };
#   else
    inline bool s_appendLocation { false };
#   endif
#endif
}
