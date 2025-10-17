// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "strings.h"
#include "variables.h"
#include "core.h"
#include <algorithm>
#include <format>

namespace Log {
    namespace Nts {
        template<std::integral T>
        [[maybe_unused]]
        void dump(
            const std::wstring & message,
            const T * data,
            size_t dataSize,
            size_t rowLength = 16 / sizeof(T),
            const Level level = Level::Debug
        ) noexcept try {
            const bool consoleReady { Console::ready(level) };
            const bool fileReady { File::ready(level) };
            if (!consoleReady && !fileReady) {
                return;
            }
            std::wstring buffer { L"      0 [ 0x0000 ] " };
            size_t count = 0, index = 0;
            rowLength = std::clamp(rowLength, 8 / sizeof(T), 64 / sizeof(T));
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
                buffer.append(std::format(L"{:0{}x} ", *data, 2 * sizeof(T)));
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
            fallbackLog();
        }
    }

    namespace Ts {
        template<std::integral T>
        [[maybe_unused]]
        void dump(
            const std::wstring & message,
            const T * data,
            size_t dataSize,
            size_t rowLength = 16 / sizeof(T),
            const Level level = Level::Debug
        ) noexcept try {
            std::scoped_lock logLock(s_logMutex);
            Nts::dump(message, data, dataSize, rowLength, level);
        } catch (...) {
            fallbackLog();
        }
    }
}
