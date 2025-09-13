// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "types.h"
#include <string>

namespace Log {
    namespace Console {
        [[nodiscard, maybe_unused]] bool ready(Level) noexcept;
        [[maybe_unused]] void write(Level, std::wstring_view) noexcept;
    }

    namespace File {
        [[maybe_unused]] bool open() noexcept;
        [[maybe_unused]] void close() noexcept;
        [[nodiscard, maybe_unused]] bool ready(Level) noexcept;
        [[maybe_unused]] void write(Level, std::wstring_view) noexcept;
    }

    namespace EventLog {
        [[maybe_unused]] void open() noexcept;
        [[maybe_unused]] void close() noexcept;
        [[nodiscard, maybe_unused]] bool ready(Level) noexcept;
        [[maybe_unused]] void write(Level, const std::wstring &) noexcept;
    }

    std::wstring levelLabel(int);
    void completeConsoleConfig();
}
