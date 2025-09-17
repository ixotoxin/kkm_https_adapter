// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#   error Requires WIN32_LEAN_AND_MEAN definition
#endif

#ifndef NOMINMAX
#   error Requires NOMINMAX definition
#endif

#include <windows.h>

#define WIN_MB2WC(A, B, C, D) \
    ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, A, B, C, D)

#define WIN_MB2WC_ESTIMATED(A, B) \
    ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, A, B, nullptr, 0)

#define WIN_WC2MB(A, B, C, D) \
    ::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS | WC_NO_BEST_FIT_CHARS, A, B, C, D, nullptr, nullptr)

#define WIN_WC2MB_ESTIMATED(A, B) \
    ::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS | WC_NO_BEST_FIT_CHARS, A, B, nullptr, 0, nullptr, nullptr)
