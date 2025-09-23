// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "types.h"
#include <string>

#define cliMsgDebug(x) \
    do { \
        if (Log::Console::ready(Log::Level::Debug)) Log::Console::write(Log::Level::Debug, x); \
    } while (false)

#define cliMsgInfo(x) \
    do { \
        if (Log::Console::ready(Log::Level::Info)) Log::Console::write(Log::Level::Info, x); \
    } while (false)

#define cliMsgWarning(x) do { Log::Console::write(Log::Level::Warning, x); } while (false)
#define cliMsgError(x) do { Log::Console::write(Log::Level::Error, x); } while (false)

namespace Log {
    namespace Console {
        [[nodiscard, maybe_unused]] bool ready(Level) noexcept;
        [[maybe_unused]] void write(Level, std::wstring_view) noexcept;
    }

    namespace File {
        [[nodiscard, maybe_unused]] bool ready(Level) noexcept;
        [[maybe_unused]] void write(Level, std::wstring_view) noexcept;
    }

    namespace EventLog {
        [[nodiscard, maybe_unused]] bool ready(Level) noexcept;
        [[maybe_unused]] void write(Level, const std::wstring &) noexcept;
    }

    void fallbackLog() noexcept;
    void reconfig() noexcept;
    [[maybe_unused]] void asForegroundProcess() noexcept;
    [[maybe_unused]] void asBackgroundProcess() noexcept;
    [[maybe_unused]] std::wstring levelLabel(int);
}
