// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "log.h"
#include "library/datetime.h"
#include <cassert>
#include <fstream>
#include <filesystem>

namespace Log {
    static const std::unordered_map<Level, std::wstring_view> s_levelLabels {
        { Level::Debug, L"DBG" },
        { Level::Info, L"INF" },
        { Level::Warning, L"WRN" },
        { Level::Error, L"ERR" }
    };

    namespace Console {
        [[nodiscard, maybe_unused]]
        bool ready(Level level) noexcept {
            return static_cast<int>(level) >= s_level;
        }

        [[maybe_unused]]
        void write(Level level, const std::wstring_view message) noexcept try {
            assert(ready(level));
            std::wostream & output { level >= Level::Warning ? std::wcerr : std::wcout };
            if (s_outputTimestamp) {
                output << DateTime::iso;
            }
            if (s_outputLevel) {
                output << s_levelLabels.at(level) << L": ";
            }
            output << message << std::endl;
        } catch (...) {
            std::wclog << Wcs::c_loggingError << std::endl;
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
                } else {
                    s_file.close();
                }
            }
            if (s_directory.empty()) {
                return false;
            }
            std::filesystem::path filePath { s_directory };
            if (!std::filesystem::is_directory(filePath)) {
                std::filesystem::create_directories(filePath);
                if (!std::filesystem::is_directory(filePath)) {
                    std::wclog << Wcs::c_loggingError << std::endl;
                    return false;
                }
            }
            s_currentMonth = localTime.wMonth;
            filePath /= std::format(L"kkmha-{:04d}-{:02d}.log", localTime.wYear, localTime.wMonth);
            s_file.open(filePath, std::ios::out | std::ios::app);
            s_file.imbue(std::locale(".utf-8"));
            if (!s_file.good()) {
                s_file.close();
            }
            return s_file.is_open() && s_file.good();
        } catch (...) {
            std::wclog << Wcs::c_loggingError << std::endl;
            return false;
        }

        [[maybe_unused]]
        void close() noexcept try {
            if (s_file.is_open()) {
                s_file.close();
            }
        } catch (...) {
            std::wclog << Wcs::c_loggingError << std::endl;
        }

        [[nodiscard, maybe_unused]]
        bool ready(Level level) noexcept {
            return static_cast<int>(level) >= s_level && open();
        }

        [[maybe_unused]]
        void write(Level level, const std::wstring_view message) noexcept try {
            assert(ready(level));
            s_file << DateTime::iso << s_levelLabels.at(level) << L": " << message << std::endl;
        } catch (...) {
            std::wclog << Wcs::c_loggingError << std::endl;
        }
    }

    namespace EventLog {
        static const std::unordered_map<Level, ::WORD> s_types {
            { Level::Debug, static_cast<::WORD>(EVENTLOG_INFORMATION_TYPE) },
            { Level::Info, static_cast<::WORD>(EVENTLOG_INFORMATION_TYPE) },
            { Level::Warning, static_cast<::WORD>(EVENTLOG_WARNING_TYPE) },
            { Level::Error, static_cast<::WORD>(EVENTLOG_ERROR_TYPE) }
        };

        static HANDLE s_sourceHandle { nullptr };

        [[maybe_unused]]
        void open() noexcept try {
            if (!s_sourceHandle) {
                s_sourceHandle = ::RegisterEventSourceW(nullptr, c_eventSource);
            }
        } catch (...) {
            std::wclog << Wcs::c_loggingError << std::endl;
        }

        [[maybe_unused]]
        void close() noexcept try {
            if (s_sourceHandle) {
                ::DeregisterEventSource(s_sourceHandle);
                s_sourceHandle = nullptr;
            }
        } catch (...) {
            std::wclog << Wcs::c_loggingError << std::endl;
        }

        [[nodiscard, maybe_unused]]
        bool ready(Level level) noexcept {
            return static_cast<int>(level) >= s_level && s_sourceHandle;
        }

        [[maybe_unused]]
        void write(Level level, const std::wstring & message) noexcept try {
            assert(ready(level));
            const wchar_t * strings[2] {
                c_eventSource,
                message.c_str()
            };
            ::ReportEventW(
                s_sourceHandle,     // Event log handle
                s_types.at(level),  // Event type
                c_eventCategory,    // Event category
                c_eventId,          // Event identifier
                nullptr,            // No security identifier
                2,                  // Size of strings array
                0,                  // No binary data
                strings,            // Array of strings
                nullptr             // No binary data
            );
        } catch (...) {
            std::wclog << Wcs::c_loggingError << std::endl;
        }
    }

    [[nodiscard, maybe_unused]]
    std::wstring_view label(int level) {
        assert(level >= c_levelDebug && level <= c_levelNone);
        if (level == c_levelNone) {
            return L"NONE";
        }
        if (s_levelLabels.contains(static_cast<Level>(level))) {
            return s_levelLabels.at(static_cast<Level>(level));
        }
        return L"[error]";
    }
}
