// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <string>

namespace Service {
    namespace Worker {
        void run();
    }

    namespace Control {
        void start();
        void stop();
        void install(const std::wstring &);
        void uninstall();
    }
}
