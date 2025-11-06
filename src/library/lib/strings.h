// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "macro.h"
#include "meta.h"
#include <string_view>

namespace Basic {
    namespace Wcs {
        LIB_WCSTR(c_ok, L"OK");
        LIB_WCSTR(c_somethingWrong, L"Что-то пошло не так!");
        LIB_WCSTR(c_fallbackErrorMessage, L"[ error ]");
        LIB_WSTR(c_source, L"источник");
        LIB_WSTR(c_directoryNotFound, L"Директория '{}' не найдена");
        LIB_WSTR(c_fileNotFound, L"Файл '{}' не найден");
        LIB_WSTR(c_couldntReadFile, L"Не удалось прочитать файл '{}'");
        LIB_WSTR(c_couldntWriteFile, L"Не удалось записать файл '{}'");
    }

    namespace Mbs {
        LIB_CSTR(c_ok, "OK");
        LIB_CSTR(c_somethingWrong, "Что-то пошло не так!");
        LIB_CSTR(c_fallbackErrorMessage, "[ error ]");
        LIB_STR(c_source, "источник");
    }
}

namespace Meta {
    template<>
    struct BoolLabels<std::wstring_view, DaNet> {
        static constexpr std::wstring_view c_true [[maybe_unused]] { L"Да" };
        static constexpr std::wstring_view c_false [[maybe_unused]] { L"Нет" };
    };

    template<>
    struct BoolLabels<std::string_view, DaNet> {
        static constexpr std::string_view c_true [[maybe_unused]] { "Да" };
        static constexpr std::string_view c_false [[maybe_unused]] { "Нет" };
    };

    template<>
    struct BoolLabels<std::wstring_view, YesNo> {
        static constexpr std::wstring_view c_true [[maybe_unused]] { L"yes" };
        static constexpr std::wstring_view c_false [[maybe_unused]] { L"no" };
    };

    template<>
    struct BoolLabels<std::string_view, YesNo> {
        static constexpr std::string_view c_true [[maybe_unused]] { "yes" };
        static constexpr std::string_view c_false [[maybe_unused]] { "no" };
    };

    template<>
    struct BoolLabels<std::wstring_view, EnaDis> {
        static constexpr std::wstring_view c_true [[maybe_unused]] { L"enable" };
        static constexpr std::wstring_view c_false [[maybe_unused]] { L"disable" };
    };

    template<>
    struct BoolLabels<std::string_view, EnaDis> {
        static constexpr std::string_view c_true [[maybe_unused]] { "enable" };
        static constexpr std::string_view c_false [[maybe_unused]] { "disable" };
    };

    template<>
    struct BoolLabels<std::wstring_view, TrueFalse> {
        static constexpr std::wstring_view c_true [[maybe_unused]] { L"true" };
        static constexpr std::wstring_view c_false [[maybe_unused]] { L"false" };
    };

    template<>
    struct BoolLabels<std::string_view, TrueFalse> {
        static constexpr std::string_view c_true [[maybe_unused]] { "true" };
        static constexpr std::string_view c_false [[maybe_unused]] { "false" };
    };
}
