// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "variables.h"
#include "strings.h"
#include "core.h"
#include <lib/text.h>

#define LOG_DEBUG_NTS(x, ...) Log::Nts::write(Log::Level::Debug, x __VA_OPT__(,) __VA_ARGS__)
#define LOG_INFO_NTS(x, ...) Log::Nts::write(Log::Level::Info, x __VA_OPT__(,) __VA_ARGS__)
#define LOG_WARNING_NTS(x, ...) Log::Nts::write(Log::Level::Warning, x __VA_OPT__(,) __VA_ARGS__)
#define LOG_ERROR_NTS(x, ...) Log::Nts::write(Log::Level::Error, x __VA_OPT__(,) __VA_ARGS__)

#define LOG_DEBUG_TS(x, ...) Log::Ts::write(Log::Level::Debug, x __VA_OPT__(,) __VA_ARGS__)
#define LOG_INFO_TS(x, ...) Log::Ts::write(Log::Level::Info, x __VA_OPT__(,) __VA_ARGS__)
#define LOG_WARNING_TS(x, ...) Log::Ts::write(Log::Level::Warning, x __VA_OPT__(,) __VA_ARGS__)
#define LOG_ERROR_TS(x, ...) Log::Ts::write(Log::Level::Error, x __VA_OPT__(,) __VA_ARGS__)

namespace Log {
    namespace Nts {
        [[maybe_unused]]
        inline void write(const Level level, const std::wstring_view message) noexcept {
            if (Console::ready(level)) {
                Console::write(level, message);
            }
            if (File::ready(level)) {
                File::write(level, message);
            }
            if (EventLog::ready(level)) {
                EventLog::write(level, std::wstring { message });
            }
        }

        [[maybe_unused]]
        inline void write(const Level level, const std::string_view message) noexcept try {
            const bool consoleReady { Console::ready(level) };
            const bool fileReady { File::ready(level) };
            const bool eventLogReady { EventLog::ready(level) };
            if (consoleReady || fileReady || eventLogReady) {
                const std::wstring message2 { Text::convert(message) };
                if (consoleReady) {
                    Console::write(level, message2);
                }
                if (fileReady) {
                    File::write(level, message2);
                }
                if (eventLogReady) {
                    EventLog::write(level, message2);
                }
            }
        } catch (...) {
            fallbackLog();
        }

        template<Meta::View Fmt, typename ... Args>
        [[maybe_unused]]
        void write(const Level level, const Fmt fmt, const auto & arg1, const Args & ... args) noexcept try {
            const bool consoleReady { Console::ready(level) };
            const bool fileReady { File::ready(level) };
            const bool eventLogReady { EventLog::ready(level) };
            if (consoleReady || fileReady || eventLogReady) {
                if constexpr (Meta::isWide<Fmt>) {
                    const std::wstring message { std::vformat(fmt, std::make_wformat_args(arg1, args...)) };
                    if (consoleReady) {
                        Console::write(level, message);
                    }
                    if (fileReady) {
                        File::write(level, message);
                    }
                    if (eventLogReady) {
                        EventLog::write(level, message);
                    }
                } else {
                    const std::wstring message { Text::convert(std::vformat(fmt, std::make_format_args(arg1, args...))) };
                    if (consoleReady) {
                        Console::write(level, message);
                    }
                    if (fileReady) {
                        File::write(level, message);
                    }
                    if (eventLogReady) {
                        EventLog::write(level, message);
                    }
                }
            }
        } catch (...) {
            fallbackLog();
        }

        template<Meta::Char Fmt, typename ... Args>
        [[maybe_unused]]
        void write(const Level level, const Fmt * fmt, const auto & arg1, const Args & ... args) noexcept {
            write<typename Meta::TextTrait<Fmt>::View>(level, fmt, arg1, args...);
        }

        template<Meta::String Fmt, typename ... Args>
        [[maybe_unused]]
        void write(const Level level, const Fmt & fmt, const auto & arg1, const Args & ... args) noexcept {
            write<typename Meta::TextTrait<Fmt>::View>(level, fmt, arg1, args...);
        }

        [[maybe_unused]]
        inline void write(const Level level, const Basic::Failure & e) noexcept {
            const bool consoleReady { Console::ready(level) };
            const bool fileReady { File::ready(level) };
            const bool eventLogReady { EventLog::ready(level) };
            if (consoleReady || fileReady || eventLogReady) {
                const std::wstring message { e.explain(s_appendLocation) };
                if (consoleReady) {
                    Console::write(level, message);
                }
                if (fileReady) {
                    File::write(level, message);
                }
                if (eventLogReady) {
                    EventLog::write(level, message);
                }
            }
        }

        [[maybe_unused]]
        inline void write(const Level level, const std::exception & e) noexcept try {
            const bool consoleReady { Console::ready(level) };
            const bool fileReady { File::ready(level) };
            const bool eventLogReady { EventLog::ready(level) };
            if (consoleReady || fileReady || eventLogReady) {
                const std::wstring message { Text::convert(e.what()) };
                if (consoleReady) {
                    Console::write(level, message);
                }
                if (fileReady) {
                    File::write(level, message);
                }
                if (eventLogReady) {
                    EventLog::write(level, message);
                }
            }
        } catch (...) {
            fallbackLog();
        }

        [[maybe_unused]]
        inline void write(const Level level, const std::error_code & e) noexcept try {
            const bool consoleReady { Console::ready(level) };
            const bool fileReady { File::ready(level) };
            const bool eventLogReady { EventLog::ready(level) };
            if (consoleReady || fileReady || eventLogReady) {
                const std::wstring message { Text::convert(e.message()) };
                if (consoleReady) {
                    Console::write(level, message);
                }
                if (fileReady) {
                    File::write(level, message);
                }
                if (eventLogReady) {
                    EventLog::write(level, message);
                }
            }
        } catch (...) {
            fallbackLog();
        }

        template<class T>
        requires requires (const T & t) { { t() } -> Meta::String; }
        [[maybe_unused]]
        void write(const Level level, const T & func) noexcept try {
            const bool consoleReady { Console::ready(level) };
            const bool fileReady { File::ready(level) };
            const bool eventLogReady { EventLog::ready(level) };
            if (consoleReady || fileReady || eventLogReady) {
                if constexpr (Meta::isWide<std::remove_cvref_t<decltype(func())>>) {
                    const std::wstring message { func() };
                    if (consoleReady) {
                        Console::write(level, message);
                    }
                    if (fileReady) {
                        File::write(level, message);
                    }
                    if (eventLogReady) {
                        EventLog::write(level, message);
                    }
                } else {
                    const std::wstring message { Text::convert(func()) };
                    if (consoleReady) {
                        Console::write(level, message);
                    }
                    if (fileReady) {
                        File::write(level, message);
                    }
                    if (eventLogReady) {
                        EventLog::write(level, message);
                    }
                }
            }
        } catch (...) {
            fallbackLog();
        }
    }

    namespace Ts {
        [[maybe_unused]]
        inline void write(const Level level, const std::wstring_view message) noexcept try {
            std::scoped_lock logLock(s_logMutex);
            Nts::write(level, message);
        } catch (...) {
            fallbackLog();
        }

        [[maybe_unused]]
        inline void write(const Level level, const std::string_view message) noexcept try {
            std::scoped_lock logLock(s_logMutex);
            Nts::write(level, message);
        } catch (...) {
            fallbackLog();
        }

        template<Meta::View Fmt, typename ... Args>
        [[maybe_unused]]
        void write(const Level level, const Fmt fmt, const auto & arg1, const Args & ... args) noexcept try {
            std::scoped_lock logLock(s_logMutex);
            Nts::write(level, fmt, arg1, args...);
        } catch (...) {
            fallbackLog();
        }

        template<Meta::Char Fmt, typename ... Args>
        [[maybe_unused]]
        void write(const Level level, const Fmt * fmt, const auto & arg1, const Args & ... args) noexcept try {
            std::scoped_lock logLock(s_logMutex);
            Nts::write<typename Meta::TextTrait<Fmt>::View>(level, fmt, arg1, args...);
        } catch (...) {
            fallbackLog();
        }

        template<Meta::String Fmt, typename ... Args>
        [[maybe_unused]]
        void write(const Level level, const Fmt & fmt, const auto & arg1, const Args & ... args) noexcept try {
            std::scoped_lock logLock(s_logMutex);
            Nts::write<typename Meta::TextTrait<Fmt>::View>(level, fmt, arg1, args...);
        } catch (...) {
            fallbackLog();
        }

        [[maybe_unused]]
        inline void write(const Level level, const Basic::Failure & e) noexcept try {
            std::scoped_lock logLock(s_logMutex);
            Nts::write(level, e);
        } catch (...) {
            fallbackLog();
        }

        [[maybe_unused]]
        inline void write(const Level level, const std::exception & e) noexcept try {
            std::scoped_lock logLock(s_logMutex);
            Nts::write(level, e);
        } catch (...) {
            fallbackLog();
        }

        [[maybe_unused]]
        inline void write(const Level level, const std::error_code & e) noexcept try {
            std::scoped_lock logLock(s_logMutex);
            Nts::write(level, e);
        } catch (...) {
            fallbackLog();
        }

        template<class T>
        requires requires (T t) { { t() } -> Meta::String; }
        [[maybe_unused]]
        void write(const Level level, const T & func) noexcept try {
            std::scoped_lock logLock(s_logMutex);
            Nts::write(level, func);
        } catch (...) {
            fallbackLog();
        }
    }
}
