// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "core.h"
#include "strings.h"
#include <lib/meta.h>
#include <lib/winapi.h>
#include <lib/datetime.h>
#include <lib/text.h>
#include <cassert>
#include <iostream>
#include <fstream>
#include <filesystem>

namespace Log {
    static bool isForegroundProcess { true };

    namespace Console {
        [[nodiscard, maybe_unused]]
        bool ready(const Level level) noexcept {
            return isForegroundProcess && Meta::toUnderlying(level) >= s_level;
        }

        [[maybe_unused]]
        void write(const Level level, const std::wstring_view message) noexcept try {
            assert(Wcs::c_levelLabels.contains(level));
            assert(ready(level));
            std::wostream & output { level >= Level::Warning ? std::wcerr : std::wcout };
            if (s_outputTimestamp) {
                output << DateTime::iso << L": ";
            }
            if (s_outputLevel) {
                output << Wcs::c_levelLabels.at(level) << L": ";
            }
            output << message << std::endl;
        } catch (...) {
            fallbackLog();
        }
    }

    namespace File {
        static std::wofstream s_file {};
        static ::WORD s_currentMonth { 0 };

        [[maybe_unused]]
        bool open() noexcept try {
            ::SYSTEMTIME localTime;
            ::GetLocalTime(&localTime);
            if (s_file.is_open()) {
                if (localTime.wMonth == s_currentMonth && s_file.good()) {
                    return true;
                }
                s_file.close();
            }
            if (s_directory.empty()) {
                return false;
            }
            std::filesystem::path filePath { s_directory };
            if (!std::filesystem::is_directory(filePath)) {
                std::filesystem::create_directories(filePath);
                if (!std::filesystem::is_directory(filePath)) {
                    fallbackLog();
                    return false;
                }
            }
            s_currentMonth = localTime.wMonth;
#ifdef EXTERNAL_LOG_VARIABLES
            filePath /= std::vformat(c_logFileFormat, std::make_wformat_args(localTime.wYear, localTime.wMonth));
#else
            filePath /= std::format(c_logFileFormat, localTime.wYear, localTime.wMonth);
#endif
            s_file.open(filePath, std::ios::out | std::ios::app);
            s_file.imbue(std::locale(".utf-8"));
            if (!s_file.good()) {
                s_file.close();
            }
            return s_file.is_open() && s_file.good();
        } catch (...) {
            fallbackLog();
            return false;
        }

        [[maybe_unused]]
        void close() noexcept try {
            if (s_file.is_open()) {
                s_file.close();
            }
        } catch (...) {
            fallbackLog();
        }

        [[nodiscard, maybe_unused]]
        bool ready(const Level level) noexcept {
            if (Meta::toUnderlying(level) < (isForegroundProcess ? s_fgLevel : s_bgLevel)) {
                return false;
            }
            return open();
        }

        [[maybe_unused]]
        void write(const Level level, const std::wstring_view message) noexcept try {
            assert(Wcs::c_levelLabels.contains(level));
            assert(ready(level));
            s_file << DateTime::iso << L": " << Wcs::c_levelLabels.at(level) << L": " << message << std::endl;
        } catch (...) {
            fallbackLog();
        }
    }

    namespace EventLog {
        static const std::unordered_map<Level, ::WORD> c_types {
            { Level::Debug, static_cast<::WORD>(EVENTLOG_INFORMATION_TYPE) },
            { Level::Info, static_cast<::WORD>(EVENTLOG_INFORMATION_TYPE) },
            { Level::Warning, static_cast<::WORD>(EVENTLOG_WARNING_TYPE) },
            { Level::Error, static_cast<::WORD>(EVENTLOG_ERROR_TYPE) }
        };

        static HANDLE s_sourceHandle { nullptr };

        [[maybe_unused]]
        bool open() noexcept {
            if (!s_sourceHandle) {
                s_sourceHandle = ::RegisterEventSourceW(nullptr, c_eventSource);
            }
            return static_cast<bool>(s_sourceHandle);
        }

        [[maybe_unused]]
        void close() noexcept {
            if (s_sourceHandle) {
                ::DeregisterEventSource(s_sourceHandle);
                s_sourceHandle = nullptr;
            }
        }

        [[nodiscard, maybe_unused]]
        bool ready(const Level level) noexcept {
            if (Meta::toUnderlying(level) < (isForegroundProcess ? s_fgLevel : s_bgLevel)) {
                return false;
            }
            return open();
        }

        [[maybe_unused]]
        void write(const Level level, const std::wstring & message) noexcept try {
            assert(c_types.contains(level));
            assert(ready(level));

            const wchar_t * strings[2] {
                c_eventSource,
                message.c_str()
            };

            ::ReportEventW(
                s_sourceHandle,     // Event log handle
                c_types.at(level),  // Event type
                c_eventCategory,    // Event category
                c_eventId,          // Event identifier
                nullptr,            // No security identifier
                2,                  // Size of strings array
                0,                  // No binary data
                strings,            // Array of strings
                nullptr             // No binary data
            );
        } catch (...) {
            fallbackLog();
        }
    }

    void fallbackLog() noexcept try {
        if (isForegroundProcess) {
            std::wclog << Wcs::c_loggingError << std::endl;
        }
    } catch (...) {}

    void reconfig() noexcept try {
        File::close();
        EventLog::close();
    } catch (...) {}

    [[maybe_unused]]
    void asForegroundProcess() noexcept {
        isForegroundProcess = true;
    }

    [[maybe_unused]]
    void asBackgroundProcess() noexcept {
        isForegroundProcess = false;
    }

    [[maybe_unused]]
    std::wstring levelLabel(LevelUnderlying level) {
        assert(level >= c_levelDebug && level <= c_levelNone);
        if (level == c_levelNone) {
            return L"none";
        }
        if (Wcs::c_levelLabels.contains(static_cast<Level>(level))) {
            return Text::lowered(Wcs::c_levelLabels.at(static_cast<Level>(level)));
        }
        return Basic::Wcs::c_fallbackErrorMessage;
    }
}

namespace Config {
    [[maybe_unused]]
    void initLogger() {
        std::atexit([] {
            ::Log::File::close();
            ::Log::EventLog::close();
        });
    }
}
