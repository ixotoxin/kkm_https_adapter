// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "types.h"
#include "variables.h"
#include <lib/utils.h>
#include <string>

#define LOG_DEBUG_CLI(x) \
    do { \
        if (Log::Console::ready(Log::Level::Debug)) Log::Console::write(Log::Level::Debug, x); \
    } while (false)

#define LOG_INFO_CLI(x) \
    do { \
        if (Log::Console::ready(Log::Level::Info)) Log::Console::write(Log::Level::Info, x); \
    } while (false)

#define LOG_WARNING_CLI(x) do { Log::Console::write(Log::Level::Warning, x); } while (false)
#define LOG_ERROR_CLI(x) do { Log::Console::write(Log::Level::Error, x); } while (false)

namespace Log {
    namespace Console {
        [[nodiscard, maybe_unused]] bool ready(Level) noexcept;
        [[maybe_unused]] void write(Level, std::wstring_view) noexcept;

        [[maybe_unused]]
        inline Level levelDown(Level level) {
            auto prevLevel = static_cast<Level>(s_level);
            if (Meta::toUnderlying(level) < s_level) {
                s_level = Meta::toUnderlying(level);
            }
            return prevLevel;
        }

        [[maybe_unused]]
        inline Level levelUp(Level level) {
            auto prevLevel = static_cast<Level>(s_level);
            if (Meta::toUnderlying(level) > s_level) {
                s_level = Meta::toUnderlying(level);
            }
            return prevLevel;
        }

        [[nodiscard, maybe_unused]]
        inline auto postLevelDownRestorer(Level level) {
            return [prevLevel = levelDown(level)] { levelUp(prevLevel); };
        }

        [[nodiscard, maybe_unused]]
        inline auto postLevelUpRestorer(Level level) {
            return [prevLevel = levelUp(level)] { levelDown(prevLevel); };
        }

        class ScopeLevelDown : protected Deferred::Exec<decltype(postLevelDownRestorer(Level::Debug))> {
        public:
            ScopeLevelDown() = delete;
            ScopeLevelDown(const ScopeLevelDown &) = delete;
            ScopeLevelDown(ScopeLevelDown &&) = delete;
            [[maybe_unused]] explicit ScopeLevelDown(Level level) : Exec(postLevelDownRestorer(level)) {}
            ~ScopeLevelDown() = default;

            ScopeLevelDown & operator=(const ScopeLevelDown &) = delete;
            ScopeLevelDown & operator=(ScopeLevelDown &&) = delete;

            using Exec::perform;
            using Exec::cancel;
        };

        class ScopeLevelUp : protected Deferred::Exec<decltype(postLevelUpRestorer(Level::Debug))> {
        public:
            ScopeLevelUp() = delete;
            ScopeLevelUp(const ScopeLevelUp &) = delete;
            ScopeLevelUp(ScopeLevelUp &&) = delete;
            [[maybe_unused]] explicit ScopeLevelUp(Level level) : Exec(postLevelUpRestorer(level)) {}
            ~ScopeLevelUp() = default;

            ScopeLevelUp & operator=(const ScopeLevelUp &) = delete;
            ScopeLevelUp & operator=(ScopeLevelUp &&) = delete;

            using Exec::perform;
            using Exec::cancel;
        };
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
