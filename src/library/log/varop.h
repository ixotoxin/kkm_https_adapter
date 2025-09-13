// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <lib/json.h>
#include <ostream>

namespace Log {
    void setVars(const Nln::Json &);
    std::wostream & vars(std::wostream &);
}
