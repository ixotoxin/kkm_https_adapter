// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "library/except.h"
#include "library/text.h"
#include <iostream>
#include <mutex>
#include <unordered_map>
#include "defaults.h"

#define ntsLogDebug(x, ...) Log::Nts::write(Log::Level::Debug, x __VA_OPT__(,) __VA_ARGS__)
#define ntsLogInfo(x, ...) Log::Nts::write(Log::Level::Info, x __VA_OPT__(,) __VA_ARGS__)
#define ntsLogWarning(x, ...) Log::Nts::write(Log::Level::Warning, x __VA_OPT__(,) __VA_ARGS__)
#define ntsLogError(x, ...) Log::Nts::write(Log::Level::Error, x __VA_OPT__(,) __VA_ARGS__)

#define tsLogDebug(x, ...) Log::Ts::write(Log::Level::Debug, x __VA_OPT__(,) __VA_ARGS__)
#define tsLogInfo(x, ...) Log::Ts::write(Log::Level::Info, x __VA_OPT__(,) __VA_ARGS__)
#define tsLogWarning(x, ...) Log::Ts::write(Log::Level::Warning, x __VA_OPT__(,) __VA_ARGS__)
#define tsLogError(x, ...) Log::Ts::write(Log::Level::Error, x __VA_OPT__(,) __VA_ARGS__)

#define cliMsgDebug(x) Log::Console::write(Log::Level::Debug, x)
#define cliMsgInfo(x) Log::Console::write(Log::Level::Info, x)
#define cliMsgWarning(x) Log::Console::write(Log::Level::Warning, x)
#define cliMsgError(x) Log::Console::write(Log::Level::Error, x)

namespace Log {
    namespace Wcs {
        constexpr const wchar_t * c_loggingError { L"Logging error" };
    }

    enum class Level : int { Debug, Info, Warning, Error };

    constexpr int c_levelDebug [[maybe_unused]] { static_cast<int>(Level::Debug) };
    constexpr int c_levelInfo [[maybe_unused]] { static_cast<int>(Level::Info) };
    constexpr int c_levelWarning [[maybe_unused]] { static_cast<int>(Level::Warning) };
    constexpr int c_levelError [[maybe_unused]] { static_cast<int>(Level::Error) };
    constexpr int c_levelNone [[maybe_unused]] { c_levelError + 1 };

    namespace Console {
        inline int s_level { c_levelDebug };
        inline bool s_outputTimestamp { c_outputTimestamp };
        inline bool s_outputLevel { c_outputLevel };

        [[nodiscard, maybe_unused]] bool ready(Level) noexcept;
        [[maybe_unused]] void write(Level, std::wstring_view) noexcept;
    }

    namespace File {
        inline int s_level { c_levelDebug };
        inline std::wstring s_directory { c_directory };

        [[maybe_unused]] bool open() noexcept;
        [[maybe_unused]] void close() noexcept;
        [[nodiscard, maybe_unused]] bool ready(Level) noexcept;
        [[maybe_unused]] void write(Level, std::wstring_view) noexcept;
    }

    namespace EventLog {
        inline int s_level { c_levelInfo };

        [[maybe_unused]] void open() noexcept;
        [[maybe_unused]] void close() noexcept;
        [[nodiscard, maybe_unused]] bool ready(Level) noexcept;
        [[maybe_unused]] void write(Level, const std::wstring &) noexcept;
    }

    namespace Nts {
        [[maybe_unused]]
        inline void write(Level level, const std::wstring_view message) noexcept {
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
        inline void write(Level level, const std::string_view message) noexcept try {
            bool consoleReady { Console::ready(level) };
            bool fileReady { File::ready(level) };
            bool eventLogReady { EventLog::ready(level) };
            if (consoleReady || fileReady || eventLogReady) {
                std::wstring message2 { Text::convert(message) };
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
            std::wclog << Wcs::c_loggingError << std::endl;
        }

        template<Meta::View Fmt, typename ... Args>
        [[maybe_unused]]
        inline void write(Level level, const Fmt fmt, const auto & arg1, const Args & ... args) noexcept try {
            bool consoleReady { Console::ready(level) };
            bool fileReady { File::ready(level) };
            bool eventLogReady { EventLog::ready(level) };
            if (consoleReady || fileReady || eventLogReady) {
                if constexpr (Meta::isWide<Fmt>) {
                    std::wstring message { std::vformat(fmt, std::make_wformat_args(arg1, args...)) };
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
                    std::wstring message { Text::convert(std::vformat(fmt, std::make_format_args(arg1, args...))) };
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
            std::wclog << Wcs::c_loggingError << std::endl;
        }

        template<Meta::Char Fmt, typename ... Args>
        [[maybe_unused]]
        inline void write(Level level, const Fmt * fmt, const auto & arg1, const Args & ... args) noexcept {
            write<typename Meta::TextTrait<Fmt>::View>(level, fmt, arg1, args...);
        }

        template<Meta::String Fmt, typename ... Args>
        [[maybe_unused]]
        inline void write(Level level, const Fmt & fmt, const auto & arg1, const Args & ... args) noexcept {
            write<typename Meta::TextTrait<Fmt>::View>(level, fmt, arg1, args...);
        }

        [[maybe_unused]]
        inline void write(Level level, const Basic::Failure & e) noexcept {
            bool consoleReady { Console::ready(level) };
            bool fileReady { File::ready(level) };
            bool eventLogReady { EventLog::ready(level) };
            if (consoleReady || fileReady || eventLogReady) {
                std::wstring message { e.explain() };
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
        inline void write(Level level, const std::exception & e) noexcept try {
            bool consoleReady { Console::ready(level) };
            bool fileReady { File::ready(level) };
            bool eventLogReady { EventLog::ready(level) };
            if (consoleReady || fileReady || eventLogReady) {
                std::wstring message { Text::convert(e.what()) };
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
            std::wclog << Wcs::c_loggingError << std::endl;
        }

        [[maybe_unused]]
        inline void write(Level level, const std::error_code & e) noexcept try {
            bool consoleReady { Console::ready(level) };
            bool fileReady { File::ready(level) };
            bool eventLogReady { EventLog::ready(level) };
            if (consoleReady || fileReady || eventLogReady) {
                std::wstring message { Text::convert(e.message()) };
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
            std::wclog << Wcs::c_loggingError << std::endl;
        }

        template<class T>
        requires requires (const T & t) { { t() } -> Meta::String; }
        [[maybe_unused]]
        inline void write(Level level, const T & func) noexcept try {
            bool consoleReady { Console::ready(level) };
            bool fileReady { File::ready(level) };
            bool eventLogReady { EventLog::ready(level) };
            if (consoleReady || fileReady || eventLogReady) {
                if constexpr (Meta::isWide<decltype(func())>) {
                    std::wstring message { func() };
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
                    std::wstring message { Text::convert(func()) };
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
            std::wclog << Wcs::c_loggingError << std::endl;
        }

        template<std::integral T>
        [[maybe_unused]]
        void dump(
            const std::wstring & message,
            const T * data,
            size_t dataSize,
            size_t rowLength = 16 / sizeof(T),
            Level level = Level::Debug
        ) noexcept try {
            bool consoleReady { Console::ready(level) };
            bool fileReady { File::ready(level) };
            if (!consoleReady && !fileReady) {
                return;
            }
            std::wstring buffer { L"      0 [ 0x0000 ] " };
            size_t count = 0, index = 0;
            rowLength = std::clamp(rowLength, static_cast<size_t>(8 / sizeof(T)), static_cast<size_t>(64 / sizeof(T)));
            std::wstring caps { L"  index [ offset ] hex " };
            caps.append(std::to_wstring(8 * sizeof(T)));
            caps.append(L"-bit data");
            if (consoleReady) {
                Console::write(level, message);
                Console::write(level, caps);
            }
            if (fileReady) {
                File::write(level, message);
                File::write(level, caps);
            }
            while (dataSize) {
                if (count >= rowLength) {
                    if (consoleReady) {
                        Console::write(level, buffer);
                    }
                    if (fileReady) {
                        File::write(level, buffer);
                    }
                    buffer = std::format(L"  {:5d} [ {:#06x} ] ", index, sizeof(T) * index);
                    count = 0;
                }
                buffer.append(std::format(L"{:0{}x} ", *(reinterpret_cast<const T *>(data)), 2 * sizeof(T)));
                ++count;
                ++index;
                ++data;
                --dataSize;
            }
            if (count) {
                if (consoleReady) {
                    Console::write(level, buffer);
                }
                if (fileReady) {
                    File::write(level, buffer);
                }
            }
        } catch (...) {
            std::wclog << Wcs::c_loggingError << std::endl;
        }
    }

    namespace Ts {
        inline std::mutex s_logMutex;

        [[maybe_unused]]
        inline void write(Level level, const std::wstring_view message) noexcept try {
            std::scoped_lock logLock(s_logMutex);
            Nts::write(level, message);
        } catch (...) {
            std::wclog << Wcs::c_loggingError << std::endl;
        }

        [[maybe_unused]]
        inline void write(Level level, const std::string_view message) noexcept try {
            std::scoped_lock logLock(s_logMutex);
            Nts::write(level, message);
        } catch (...) {
            std::wclog << Wcs::c_loggingError << std::endl;
        }

        template<Meta::View Fmt, typename ... Args>
        [[maybe_unused]]
        inline void write(Level level, const Fmt fmt, const auto & arg1, const Args & ... args) noexcept try {
            std::scoped_lock logLock(s_logMutex);
            Nts::write(level, fmt, arg1, args...);
        } catch (...) {
            std::wclog << Wcs::c_loggingError << std::endl;
        }

        template<Meta::Char Fmt, typename ... Args>
        [[maybe_unused]]
        inline void write(Level level, const Fmt * fmt, const auto & arg1, const Args & ... args) noexcept try {
            std::scoped_lock logLock(s_logMutex);
            Nts::write<typename Meta::TextTrait<Fmt>::View>(level, fmt, arg1, args...);
        } catch (...) {
            std::wclog << Wcs::c_loggingError << std::endl;
        }

        template<Meta::String Fmt, typename ... Args>
        [[maybe_unused]]
        inline void write(Level level, const Fmt & fmt, const auto & arg1, const Args & ... args) noexcept try {
            std::scoped_lock logLock(s_logMutex);
            Nts::write<typename Meta::TextTrait<Fmt>::View>(level, fmt, arg1, args...);
        } catch (...) {
            std::wclog << Wcs::c_loggingError << std::endl;
        }

        [[maybe_unused]]
        inline void write(Level level, const Basic::Failure & e) noexcept try {
            std::scoped_lock logLock(s_logMutex);
            Nts::write(level, e);
        } catch (...) {
            std::wclog << Wcs::c_loggingError << std::endl;
        }

        [[maybe_unused]]
        inline void write(Level level, const std::exception & e) noexcept try {
            std::scoped_lock logLock(s_logMutex);
            Nts::write(level, e);
        } catch (...) {
            std::wclog << Wcs::c_loggingError << std::endl;
        }

        [[maybe_unused]]
        inline void write(Level level, const std::error_code & e) noexcept try {
            std::scoped_lock logLock(s_logMutex);
            Nts::write(level, e);
        } catch (...) {
            std::wclog << Wcs::c_loggingError << std::endl;
        }

        template<class T>
        requires requires (T t) { { t() } -> Meta::String; }
        [[maybe_unused]]
        inline void write(Level level, const T & func) noexcept try {
            std::scoped_lock logLock(s_logMutex);
            Nts::write(level, func);
        } catch (...) {
            std::wclog << Wcs::c_loggingError << std::endl;
        }

        template<typename ... Args>
        [[maybe_unused]]
        inline void dump(const Args & ... args) noexcept try {
            std::scoped_lock logLock(s_logMutex);
            Nts::dump(args...);
        } catch (...) {
            std::wclog << Wcs::c_loggingError << std::endl;
        }
    }

    static const std::unordered_map<std::wstring, int> s_levelCastMap {
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
        { L"non", c_levelNone },
        { L"none", c_levelNone },
        { std::to_wstring(c_levelNone), c_levelNone }
    };

    [[nodiscard, maybe_unused]] std::wstring_view label(int);
}
