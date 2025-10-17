// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "meta.h"
#include "strings.h"
#include "except.h"
#include <functional>
#include <filesystem>

namespace Path {
    using namespace std::string_view_literals;

    using Basic::Failure;
    using Basic::DataError;

    using Filter = std::function<std::filesystem::path(const std::filesystem::path &)>;

    [[nodiscard, maybe_unused]]
    inline bool isGood(const std::wstring_view path) {
        return
            path.find_first_of(
                L"\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f"
                L"\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f"
                L"<>:\"|?*"sv
            ) == std::wstring_view::npos
            && path.find(L"\\."sv) == std::wstring_view::npos
            && path.find(L"/."sv) == std::wstring_view::npos
            && path.find(L".\\"sv) == std::wstring_view::npos
            && path.find(L"./"sv) == std::wstring_view::npos
            && path.find(L"\\ "sv) == std::wstring_view::npos
            && path.find(L"/ "sv) == std::wstring_view::npos
            && path.find(L" \\"sv) == std::wstring_view::npos
            && path.find(L" /"sv) == std::wstring_view::npos
            && path.find(L".."sv) == std::wstring_view::npos
            && path.find_last_not_of(L" ."sv) == path.size() - 1;
    }

    [[nodiscard, maybe_unused]]
    inline bool isGood(const std::string_view path) {
        return
            path.find_first_of(
                "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f"
                "\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f"
                "<>:\"|?*"sv
            ) == std::string_view::npos
            && path.find("\\."sv) == std::string_view::npos
            && path.find("/."sv) == std::string_view::npos
            && path.find(".\\"sv) == std::string_view::npos
            && path.find("./"sv) == std::string_view::npos
            && path.find("\\ "sv) == std::string_view::npos
            && path.find("/ "sv) == std::string_view::npos
            && path.find(" \\"sv) == std::string_view::npos
            && path.find(" /"sv) == std::string_view::npos
            && path.find(".."sv) == std::string_view::npos
            && path.find_last_not_of(" ."sv) == path.size() - 1;
    }

    [[nodiscard, maybe_unused]]
    inline bool isGoodFileName(const std::wstring_view path) {
        return
            path.find_first_of(
                L"\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f"
                L"\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f"
                L"<>:\"/\\|?*"sv
            ) == std::string_view::npos
            && !path.empty()
            && !path.starts_with(L"."sv)
            && !path.ends_with(L"."sv)
            && !path.starts_with(L" "sv)
            && !path.ends_with(L" "sv);
    }

    [[nodiscard, maybe_unused]]
    inline bool isGoodFileName(const std::string_view path) {
        return
            path.find_first_of(
                "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f"
                "\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f"
                "<>:\"/\\|?*"sv
            ) == std::string_view::npos
            && !path.empty()
            && !path.starts_with("."sv)
            && !path.ends_with("."sv)
            && !path.starts_with(" "sv)
            && !path.ends_with(" "sv);
    }

    [[nodiscard, maybe_unused]]
    inline auto noEmpty() {
        return
            [] (const std::filesystem::path & value) -> std::filesystem::path {
                if (value.empty()) {
                    throw DataError(Basic::Wcs::c_rangeError); // NOLINT(*-exception-baseclass)
                }
                return value;
            };
    }

    template<Meta::Filter<std::filesystem::path> F>
    [[nodiscard, maybe_unused]]
    auto noEmpty(F && subFilter0) {
        return
            [subFilter = std::forward<F>(subFilter0)]
            (const std::filesystem::path & value) -> std::filesystem::path {
                std::filesystem::path filtered { subFilter(value) };
                if (filtered.empty()) {
                    throw DataError(Basic::Wcs::c_rangeError); // NOLINT(*-exception-baseclass)
                }
                return filtered;
            };
    }

    [[nodiscard, maybe_unused]]
    inline auto good() {
        return
            [] (const std::filesystem::path & value) -> std::filesystem::path {
                if (isGood(value.native())) {
                    return value;
                }
                throw DataError(Basic::Wcs::c_rangeError); // NOLINT(*-exception-baseclass)
            };
    }

    template<Meta::Filter<std::filesystem::path> F>
    [[nodiscard, maybe_unused]]
    auto good(F && subFilter0) {
        return
            [subFilter = std::forward<F>(subFilter0)]
            (const std::filesystem::path & value) -> std::filesystem::path {
                std::filesystem::path filtered { subFilter(value) };
                if (isGood(filtered.native())) {
                    return filtered;
                }
                throw DataError(Basic::Wcs::c_rangeError); // NOLINT(*-exception-baseclass)
            };
    }

    template<Meta::String T>
    [[nodiscard, maybe_unused]]
    auto good() {
        return
            [] (const T & value) -> T {
                if (isGood(value)) {
                    return value;
                }
                throw DataError(Basic::Wcs::c_rangeError); // NOLINT(*-exception-baseclass)
            };
    }

    template<Meta::String T, Meta::Filter<T> F>
    [[nodiscard, maybe_unused]]
    auto good(F && subFilter0) {
        return
            [subFilter = std::forward<F>(subFilter0)]
            (const T & value) -> T {
                T filtered { subFilter(value) };
                if (isGood(filtered)) {
                    return filtered;
                }
                throw DataError(Basic::Wcs::c_rangeError); // NOLINT(*-exception-baseclass)
            };
    }

    [[nodiscard, maybe_unused]]
    inline auto goodFileName() {
        return
            [] (const std::filesystem::path & value) -> std::filesystem::path {
                if (isGoodFileName(value.native())) {
                    return value;
                }
                throw DataError(Basic::Wcs::c_rangeError); // NOLINT(*-exception-baseclass)
            };
    }

    template<Meta::Filter<std::filesystem::path> F>
    [[nodiscard, maybe_unused]]
    auto goodFileName(F && subFilter0) {
        return
            [subFilter = std::forward<F>(subFilter0)]
            (const std::filesystem::path & value) -> std::filesystem::path {
                std::filesystem::path filtered { subFilter(value) };
                if (isGoodFileName(filtered.native())) {
                    return filtered;
                }
                throw DataError(Basic::Wcs::c_rangeError); // NOLINT(*-exception-baseclass)
            };
    }

    template<Meta::String T>
    [[nodiscard, maybe_unused]]
    auto goodFileName() {
        return
            [] (const T & value) -> T {
                if (isGoodFileName(value)) {
                    return value;
                }
                throw DataError(Basic::Wcs::c_rangeError); // NOLINT(*-exception-baseclass)
            };
    }

    template<Meta::String T, Meta::Filter<T> F>
    [[nodiscard, maybe_unused]]
    auto goodFileName(F && subFilter0) {
        return
            [subFilter = std::forward<F>(subFilter0)]
            (const T & value) -> T {
                T filtered { subFilter(value) };
                if (isGoodFileName(filtered)) {
                    return filtered;
                }
                throw DataError(Basic::Wcs::c_rangeError); // NOLINT(*-exception-baseclass)
            };
    }

    [[nodiscard, maybe_unused]]
    inline auto absolute() {
        return
            [] (const std::filesystem::path & value) -> std::filesystem::path {
                return std::filesystem::absolute(value);
            };
    }

    template<Meta::Filter<std::filesystem::path> F>
    [[nodiscard, maybe_unused]]
    auto absolute(F && subFilter0) {
        return
            [subFilter = std::forward<F>(subFilter0)]
            (const std::filesystem::path & value) -> std::filesystem::path {
                return std::filesystem::absolute(subFilter(value));
            };
    }

    [[nodiscard, maybe_unused]]
    inline auto absolute(const std::filesystem::path & dir) {
        return
            [dir] (const std::filesystem::path & value) -> std::filesystem::path {
                const std::filesystem::path current { std::filesystem::current_path() };
                std::filesystem::current_path(dir);
                std::filesystem::path result { std::filesystem::absolute(value) };
                std::filesystem::current_path(current);
                return result;
            };
    }

    template<Meta::Filter<std::filesystem::path> F>
    [[nodiscard, maybe_unused]]
    auto absolute(const std::filesystem::path & dir, F && subFilter0) {
        return
            [dir, subFilter = std::forward<F>(subFilter0)]
            (const std::filesystem::path & value) -> std::filesystem::path {
                const std::filesystem::path current { std::filesystem::current_path() };
                std::filesystem::current_path(dir);
                std::filesystem::path result { std::filesystem::absolute(subFilter(value)) };
                std::filesystem::current_path(current);
                return result;
            };
    }

    [[nodiscard, maybe_unused]]
    inline auto existsDir() {
        return
            [] (const std::filesystem::path & value) -> std::filesystem::path {
                if (!std::filesystem::is_directory(value)) {
                    throw Failure(LIB_WFMT(Basic::Wcs::c_directoryNotFound, value.native())); // NOLINT(*-exception-baseclass)
                }
                return value;
            };
    }

    template<Meta::Filter<std::filesystem::path> F>
    [[nodiscard, maybe_unused]]
    auto existsDir(F && subFilter0) {
        return
            [subFilter = std::forward<F>(subFilter0)]
            (const std::filesystem::path & value) -> std::filesystem::path {
                std::filesystem::path filtered { subFilter(value) };
                if (!std::filesystem::is_directory(filtered)) {
                    throw Failure(LIB_WFMT(Basic::Wcs::c_directoryNotFound, filtered.native())); // NOLINT(*-exception-baseclass)
                }
                return filtered;
            };
    }

    [[nodiscard, maybe_unused]]
    inline auto existsFile() {
        return
            [] (const std::filesystem::path & value) -> std::filesystem::path {
                if (!std::filesystem::is_regular_file(value)) {
                    throw Failure(LIB_WFMT(Basic::Wcs::c_fileNotFound, value.native())); // NOLINT(*-exception-baseclass)
                }
                return value;
            };
    }

    template<Meta::Filter<std::filesystem::path> F>
    [[nodiscard, maybe_unused]]
    auto existsFile(F && subFilter0) {
        return
            [subFilter = std::forward<F>(subFilter0)]
            (const std::filesystem::path & value) -> std::filesystem::path {
                std::filesystem::path filtered { subFilter(value) };
                if (!std::filesystem::is_regular_file(filtered)) {
                    throw Failure(LIB_WFMT(Basic::Wcs::c_fileNotFound, filtered.native())); // NOLINT(*-exception-baseclass)
                }
                return filtered;
            };
    }

    [[nodiscard, maybe_unused]]
    inline auto testDir(const bool & test) {
        return
            [& test] (const std::filesystem::path & value) -> std::filesystem::path {
                if (test && !std::filesystem::is_directory(value)) {
                    throw Failure(LIB_WFMT(Basic::Wcs::c_directoryNotFound, value.native())); // NOLINT(*-exception-baseclass)
                }
                return value;
            };
    }

    template<Meta::Filter<std::filesystem::path> F>
    [[nodiscard, maybe_unused]]
    auto testDir(const bool & test, F && subFilter0) {
        return
            [& test, subFilter = std::forward<F>(subFilter0)]
            (const std::filesystem::path & value) -> std::filesystem::path {
                std::filesystem::path filtered { subFilter(value) };
                if (test && !std::filesystem::is_directory(filtered)) {
                    throw Failure(LIB_WFMT(Basic::Wcs::c_directoryNotFound, filtered.native())); // NOLINT(*-exception-baseclass)
                }
                return filtered;
            };
    }

    [[nodiscard, maybe_unused]]
    inline auto testFile(const bool & test) {
        return
            [& test] (const std::filesystem::path & value) -> std::filesystem::path {
                if (test && !std::filesystem::is_regular_file(value)) {
                    throw Failure(LIB_WFMT(Basic::Wcs::c_fileNotFound, value.native())); // NOLINT(*-exception-baseclass)
                }
                return value;
            };
    }

    template<Meta::Filter<std::filesystem::path> F>
    [[nodiscard, maybe_unused]]
    auto testFile(const bool & test, F && subFilter0) {
        return
            [& test, subFilter = std::forward<F>(subFilter0)]
            (const std::filesystem::path & value) -> std::filesystem::path {
                std::filesystem::path filtered { subFilter(value) };
                if (test && !std::filesystem::is_regular_file(filtered)) {
                    throw Failure(LIB_WFMT(Basic::Wcs::c_fileNotFound, filtered.native())); // NOLINT(*-exception-baseclass)
                }
                return filtered;
            };
    }

    [[nodiscard, maybe_unused]]
    inline auto touchDir() {
        return
            [] (const std::filesystem::path & value) -> std::filesystem::path {
                if (!std::filesystem::is_directory(value)) {
                    std::filesystem::create_directories(value);
                    if (!std::filesystem::is_directory(value)) {
                        throw Failure(LIB_WFMT(Basic::Wcs::c_directoryNotFound, value.native())); // NOLINT(*-exception-baseclass)
                    }
                }
                return value;
            };
    }

    template<Meta::Filter<std::filesystem::path> F>
    [[nodiscard, maybe_unused]]
    auto touchDir(F && subFilter0) {
        return
            [subFilter = std::forward<F>(subFilter0)]
            (const std::filesystem::path & value) -> std::filesystem::path {
                std::filesystem::path filtered { subFilter(value) };
                if (!std::filesystem::is_directory(filtered)) {
                    std::filesystem::create_directories(filtered);
                    if (!std::filesystem::is_directory(filtered)) {
                        throw Failure(LIB_WFMT(Basic::Wcs::c_directoryNotFound, filtered.native())); // NOLINT(*-exception-baseclass)
                    }
                }
                return filtered;
            };
    }

    namespace Wcs {
        [[nodiscard, maybe_unused]]
        inline auto good() {
            return ::Path::good<std::wstring>();
        }

        template<Meta::Filter<std::wstring> F>
        [[nodiscard, maybe_unused]]
        auto good(F && subFilter) {
            return ::Path::good<std::wstring>(std::forward<F>(subFilter));
        }

        [[nodiscard, maybe_unused]]
        inline auto goodFileName() {
            return ::Path::goodFileName<std::wstring>();
        }

        template<Meta::Filter<std::wstring> F>
        [[nodiscard, maybe_unused]]
        auto goodFileName(F && subFilter) {
            return ::Path::goodFileName<std::wstring>(std::forward<F>(subFilter));
        }
    }

    namespace Mbs {
        [[nodiscard, maybe_unused]]
        inline auto good() {
            return ::Path::good<std::string>();
        }

        template<Meta::Filter<std::string> F>
        [[nodiscard, maybe_unused]]
        auto good(F && subFilter) {
            return ::Path::good<std::string>(std::forward<F>(subFilter));
        }

        [[nodiscard, maybe_unused]]
        inline auto goodFileName() {
            return ::Path::goodFileName<std::string>();
        }

        template<Meta::Filter<std::string> F>
        [[nodiscard, maybe_unused]]
        auto goodFileName(F && subFilter) {
            return ::Path::goodFileName<std::string>(std::forward<F>(subFilter));
        }
    }
}
