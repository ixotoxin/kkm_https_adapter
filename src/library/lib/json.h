// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "macro.h"
#include "except.h"
#include "numeric.h"
#include "datetime.h"
#include "text.h"
#include <ostream>
#include <nlohmann/json.hpp>

namespace Nln {
    using Json = nlohmann::json;
    using Exception = Json::exception;
    constexpr auto EmptyJsonObject = Json::value_t::object;
}

inline std::wostream & operator<<(std::wostream & stream, const Nln::Json & json) {
    stream << Text::convert(json.dump(4));
    return stream;
}

namespace Json {
    namespace Mbs {
        LIB_CSTR(c_successKey, "!success");
        LIB_CSTR(c_messageKey, "!message");
    }

    using Basic::Failure;
    using Basic::DataError;
    using Handler = std::function<bool(const Nln::Json &, const std::wstring &)>;

    template<Meta::Bool T>
    [[nodiscard, maybe_unused]]
    inline T cast(const Nln::Json & json) try {
        if (json.is_boolean()) {
            return json.get<T>();
        } else if (json.is_string()) {
            return Text::cast<T>(static_cast<std::string>(json));
        } else if (json.is_number()) {
            return static_cast<T>(json.get<int64_t>());
        }
        throw DataError(Basic::Wcs::c_invalidValue); // NOLINT(*-exception-baseclass)
    } catch (const Nln::Exception & e) {
        throw DataError(e); // NOLINT(*-exception-baseclass)
    }

    template<Meta::Numeric T>
    [[nodiscard, maybe_unused]]
    inline T cast(const Nln::Json & json) try {
        if (json.is_number/*_integer*/()) {
            return json.get<T>();
        } else if (json.is_string()) {
            return Text::cast<T>(static_cast<std::string>(json));
        } else if (json.is_boolean()) {
            return static_cast<T>(json.get<bool>());
        }
        throw DataError(Basic::Wcs::c_invalidValue); // NOLINT(*-exception-baseclass)
    } catch (const Nln::Exception & e) {
        throw DataError(e); // NOLINT(*-exception-baseclass)
    }

    template<Meta::fromTemplate<std::chrono::duration> T>
    [[nodiscard, maybe_unused]]
    inline T cast(const Nln::Json & json) try {
        if (json.is_number/*_integer*/()) {
            return T(json.get<typename T::rep>());
        } else if (json.is_string()) {
            return T(Text::cast<typename T::rep>(static_cast<std::string>(json)));
        }
        throw DataError(Basic::Wcs::c_invalidValue); // NOLINT(*-exception-baseclass)
    } catch (const Nln::Exception & e) {
        throw DataError(e); // NOLINT(*-exception-baseclass)
    }

    template<Meta::FloatingPoint T>
    [[nodiscard, maybe_unused]]
    inline T cast(const Nln::Json & json) try {
        if (json.is_number()) {
            return json.get<T>();
        } else if (json.is_string()) {
            return Text::cast<T>(static_cast<std::string>(json));
        } else if (json.is_boolean()) {
            return static_cast<T>(json.get<bool>());
        }
        throw DataError(Basic::Wcs::c_invalidValue); // NOLINT(*-exception-baseclass)
    } catch (const Nln::Exception & e) {
        throw DataError(e); // NOLINT(*-exception-baseclass)
    }

    template<Meta::String T>
    [[nodiscard, maybe_unused]]
    inline T cast(const Nln::Json & json) try {
        if (json.is_string()) {
            if constexpr (Meta::isWide<T>) {
                return Text::convert(json.get<std::string>());
            } else {
                return json.get<std::string>();
            }
        } else if (json.is_number_integer()) {
            return Numeric::cast<T>(json.get<int64_t>());
        } else if (json.is_number_float()) {
            return Numeric::cast<T>(json.get<long double>());
        } else if (json.is_boolean()) {
            auto value = Text::yesNo<typename Meta::TextTrait<T>::Wideness>(json.get<bool>());
            // auto value = Numeric::boolCast<typename Meta::TextTrait<T>::View, Meta::YesNo>(json.get<bool>());
            return { value.begin(), value.end() };
        }
        throw DataError(Basic::Wcs::c_invalidValue); // NOLINT(*-exception-baseclass)
    } catch (const Nln::Exception & e) {
        throw DataError(e); // NOLINT(*-exception-baseclass)
    }

    template<std::same_as<std::filesystem::path> T>
    [[nodiscard, maybe_unused]]
    inline T cast(const Nln::Json & json) try {
        if (json.is_string()) {
            return T(Text::convert(static_cast<std::string>(json)));
        }
        throw DataError(Basic::Wcs::c_invalidValue); // NOLINT(*-exception-baseclass)
    } catch (const Nln::Exception & e) {
        throw DataError(e); // NOLINT(*-exception-baseclass)
    }

    [[maybe_unused]]
    inline bool handle(
        const Nln::Json & json,
        const Handler & handler,
        const std::wstring & jsonPath = {}
    ) try {
        if (json.is_null()) {
            return false;
        }
        return handler(json, jsonPath);
    } catch (DataError & e) {
        e.variable(jsonPath);
        throw;
    } catch (const Nln::Exception & e) {
        throw DataError(e, jsonPath); // NOLINT(*-exception-baseclass)
    }

    [[maybe_unused]]
    inline bool handleKey(
        const Nln::Json & json,
        const std::string_view key,
        const Handler & handler,
        const std::wstring & baseJsonPath = {}
    ) {
        assert(json.is_object());
        std::wstring jsonPath { baseJsonPath };
        Text::joinTo(jsonPath, Text::convert(key), L".");
        try {
            if (json.is_object()) {
                if (json.contains(key)) {
                    return handle(json[key], handler, jsonPath);
                }
                return false;
            } else if (json.is_null()) {
                return false;
            }
        } catch (DataError & e) {
            e.variable(jsonPath);
            throw;
        } catch (const Nln::Exception & e) {
            throw DataError(e, jsonPath); // NOLINT(*-exception-baseclass)
        }
        throw DataError(Basic::Wcs::c_invalidValue, jsonPath); // NOLINT(*-exception-baseclass)
    }

    template<typename T>
    requires (!Meta::BackSideGrowingRange<T>)
    [[maybe_unused]]
    inline bool handle(
        const Nln::Json & json,
        T & variable,
        const std::wstring & jsonPath = {}
    ) try {
        if (json.is_null()) {
            return false;
        }
        variable = cast<T>(json);
        return true;
    } catch (DataError & e) {
        e.variable(jsonPath);
        throw;
    } catch (const Nln::Exception & e) {
        throw DataError(e, jsonPath); // NOLINT(*-exception-baseclass)
    }

    template<typename T>
    requires (!Meta::BackSideGrowingRange<T>)
    [[maybe_unused]]
    inline bool handleKey(
        const Nln::Json & json,
        const std::string_view key,
        T & variable,
        const std::wstring & baseJsonPath = {}
    ) {
        assert(json.is_object());
        std::wstring jsonPath { baseJsonPath };
        Text::joinTo(jsonPath, Text::convert(key), L".");
        try {
            if (json.is_object()) {
                if (json.contains(key)) {
                    return handle<T>(json[key], variable, jsonPath);
                }
                return false;
            } else if (json.is_null()) {
                return false;
            }
        } catch (DataError & e) {
            e.variable(jsonPath);
            throw;
        } catch (const Nln::Exception & e) {
            throw DataError(e, jsonPath); // NOLINT(*-exception-baseclass)
        }
        throw DataError(Basic::Wcs::c_invalidValue, jsonPath); // NOLINT(*-exception-baseclass)
    }

    template<typename T>
    [[maybe_unused]]
    inline bool handle(
        const Nln::Json & json,
        T & variable,
        const Meta::Filter<T> auto & filter,
        const std::wstring & jsonPath = {}
    ) try {
        if (json.is_null()) {
            return false;
        }
        variable = filter(cast<T>(json));
        return true;
    } catch (DataError & e) {
        e.variable(jsonPath);
        throw;
    } catch (const Nln::Exception & e) {
        throw DataError(e, jsonPath); // NOLINT(*-exception-baseclass)
    }

    template<typename T>
    [[maybe_unused]]
    inline bool handleKey(
        const Nln::Json & json,
        const std::string_view key,
        T & variable,
        const Meta::Filter<T> auto & filter,
        const std::wstring & baseJsonPath = {}
    ) {
        assert(json.is_object());
        std::wstring jsonPath { baseJsonPath };
        Text::joinTo(jsonPath, Text::convert(key), L".");
        try {
            if (json.is_object()) {
                if (json.contains(key)) {
                    return handle<T>(json[key], variable, filter, jsonPath);
                }
                return false;
            } else if (json.is_null()) {
                return false;
            }
        } catch (DataError & e) {
            e.variable(jsonPath);
            throw;
        } catch (const Nln::Exception & e) {
            throw DataError(e, jsonPath); // NOLINT(*-exception-baseclass)
        }
        throw DataError(Basic::Wcs::c_invalidValue, jsonPath); // NOLINT(*-exception-baseclass)
    }

    template<typename T, Meta::EnumCastMap<T> U>
    [[maybe_unused]]
    inline bool handle(
        const Nln::Json & json,
        T & variable,
        const U & castMap,
        const std::wstring & jsonPath = {}
    ) try {
        if (json.is_null()) {
            return false;
        }
        auto text = cast<typename U::key_type>(json);
        Text::trim(text);
        Text::lower(text);
        auto it = castMap.find(text);
        if (it == castMap.end()) {
            throw DataError(Basic::Wcs::c_rangeError, jsonPath); // NOLINT(*-exception-baseclass)
        }
        variable = it->second;
        return true;
    } catch (DataError & e) {
        e.variable(jsonPath);
        throw;
    } catch (const Nln::Exception & e) {
        throw DataError(e, jsonPath); // NOLINT(*-exception-baseclass)
    }

    template<typename T>
    [[maybe_unused]]
    inline bool handleKey(
        const Nln::Json & json,
        const std::string_view key,
        T & variable,
        const Meta::EnumCastMap<T> auto & castMap,
        const std::wstring & baseJsonPath = {}
    ) {
        assert(json.is_object());
        std::wstring jsonPath { baseJsonPath };
        Text::joinTo(jsonPath, Text::convert(key), L".");
        try {
            if (json.is_object()) {
                if (json.contains(key)) {
                    return handle<T>(json[key], variable, castMap, jsonPath);
                }
                return false;
            } else if (json.is_null()) {
                return false;
            }
        } catch (DataError & e) {
            e.variable(jsonPath);
            throw;
        } catch (const Nln::Exception & e) {
            throw DataError(e, jsonPath); // NOLINT(*-exception-baseclass)
        }
        throw DataError(Basic::Wcs::c_invalidValue, jsonPath); // NOLINT(*-exception-baseclass)
    }

    template<typename T>
    [[maybe_unused]]
    inline bool handle(
        const Nln::Json & json,
        T & variable,
        const Meta::EnumCastMap<T> auto & castMap,
        const Meta::Filter<T> auto & filter,
        const std::wstring & jsonPath = {}
    ) try {
        if (json.is_null()) {
            return false;
        }
        T value {};
        if (handle(json, value, castMap, jsonPath)) {
            variable = filter(value);
            return true;
        }
        return false;
    } catch (DataError & e) {
        e.variable(jsonPath);
        throw;
    } catch (const Nln::Exception & e) {
        throw DataError(e, jsonPath); // NOLINT(*-exception-baseclass)
    }

    template<typename T>
    [[maybe_unused]]
    inline bool handleKey(
        const Nln::Json & json,
        const std::string_view key,
        T & variable,
        const Meta::EnumCastMap<T> auto & castMap,
        const Meta::Filter<T> auto & filter,
        const std::wstring & baseJsonPath = {}
    ) {
        assert(json.is_object());
        std::wstring jsonPath { baseJsonPath };
        Text::joinTo(jsonPath, Text::convert(key), L".");
        try {
            if (json.is_object()) {
                if (json.contains(key)) {
                    return handle<T>(json[key], variable, castMap, filter, jsonPath);
                }
                return false;
            } else if (json.is_null()) {
                return false;
            }
        } catch (DataError & e) {
            e.variable(jsonPath);
            throw;
        } catch (const Nln::Exception & e) {
            throw DataError(e, jsonPath); // NOLINT(*-exception-baseclass)
        }
        throw DataError(Basic::Wcs::c_invalidValue, jsonPath); // NOLINT(*-exception-baseclass)
    }

    template<typename T, Meta::EnumDomain<T> U>
    [[maybe_unused]]
    inline bool handle(
        const Nln::Json & json,
        T & variable,
        const U & domain,
        const std::wstring & jsonPath = {}
    ) try {
        if (json.is_null()) {
            return false;
        }
        auto value = cast<T>(json);
        if constexpr (Meta::String<T>) {
            Text::trim(value);
            Text::lower(value);
        }
        if (std::ranges::find(domain, value) == domain.end()) {
            throw DataError(Basic::Wcs::c_rangeError, jsonPath); // NOLINT(*-exception-baseclass)
        }
        variable = value;
        return true;
    } catch (DataError & e) {
        e.variable(jsonPath);
        throw;
    } catch (const Nln::Exception & e) {
        throw DataError(e, jsonPath); // NOLINT(*-exception-baseclass)
    }

    template<typename T>
    [[maybe_unused]]
    inline bool handleKey(
        const Nln::Json & json,
        const std::string_view key,
        T & variable,
        const Meta::EnumDomain<T> auto & domain,
        const std::wstring & baseJsonPath = {}
    ) {
        assert(json.is_object());
        std::wstring jsonPath { baseJsonPath };
        Text::joinTo(jsonPath, Text::convert(key), L".");
        try {
            if (json.is_object()) {
                if (json.contains(key)) {
                    return handle<T>(json[key], variable, domain, jsonPath);
                }
                return false;
            } else if (json.is_null()) {
                return false;
            }
        } catch (DataError & e) {
            e.variable(jsonPath);
            throw;
        } catch (const Nln::Exception & e) {
            throw DataError(e, jsonPath); // NOLINT(*-exception-baseclass)
        }
        throw DataError(Basic::Wcs::c_invalidValue, jsonPath); // NOLINT(*-exception-baseclass)
    }

    template<typename T>
    [[maybe_unused]]
    inline bool handle(
        const Nln::Json & json,
        T & variable,
        const Meta::EnumDomain<T> auto & domain,
        const Meta::Filter<T> auto & filter,
        const std::wstring & jsonPath = {}
    ) try {
        if (json.is_null()) {
            return false;
        }
        T value {};
        if (handle(json, value, domain, jsonPath)) {
            variable = filter(value);
            return true;
        }
        return false;
    } catch (DataError & e) {
        e.variable(jsonPath);
        throw;
    } catch (const Nln::Exception & e) {
        throw DataError(e, jsonPath); // NOLINT(*-exception-baseclass)
    }

    template<typename T>
    [[maybe_unused]]
    inline bool handleKey(
        const Nln::Json & json,
        const std::string_view key,
        T & variable,
        const Meta::EnumDomain<T> auto & domain,
        const Meta::Filter<T> auto & filter,
        const std::wstring & baseJsonPath = {}
    ) {
        assert(json.is_object());
        std::wstring jsonPath { baseJsonPath };
        Text::joinTo(jsonPath, Text::convert(key), L".");
        try {
            if (json.is_object()) {
                if (json.contains(key)) {
                    return handle<T>(json[key], variable, domain, filter, jsonPath);
                }
                return false;
            } else if (json.is_null()) {
                return false;
            }
        } catch (DataError & e) {
            e.variable(jsonPath);
            throw;
        } catch (const Nln::Exception & e) {
            throw DataError(e, jsonPath); // NOLINT(*-exception-baseclass)
        }
        throw DataError(Basic::Wcs::c_invalidValue, jsonPath); // NOLINT(*-exception-baseclass)
    }

    template<Meta::BackSideGrowingRange T>
    [[maybe_unused]]
    inline bool handle(
        const Nln::Json & json,
        T & variable,
        const std::wstring & jsonPath = {}
    ) try {
        if (json.is_array()) {
            for (auto & j : json) {
                variable.push_back(cast<typename T::value_type>(j));
            }
            return true;
        } else if (json.is_null()) {
            return false;
        }
        throw DataError(Basic::Wcs::c_invalidValue, jsonPath); // NOLINT(*-exception-baseclass)
    } catch (DataError & e) {
        e.variable(jsonPath);
        throw;
    } catch (const Nln::Exception & e) {
        throw DataError(e, jsonPath); // NOLINT(*-exception-baseclass)
    }

    [[maybe_unused]]
    inline bool handleKey(
        const Nln::Json & json,
        const std::string_view key,
        Meta::BackSideGrowingRange auto & variable,
        const std::wstring & baseJsonPath = {}
    ) {
        assert(json.is_object());
        std::wstring jsonPath { baseJsonPath };
        Text::joinTo(jsonPath, Text::convert(key), L".");
        try {
            if (json.is_object()) {
                if (json.contains(key)) {
                    return handle(json[key], variable, jsonPath);
                }
                return false;
            } else if (json.is_null()) {
                return false;
            }
        } catch (DataError & e) {
            e.variable(jsonPath);
            throw;
        } catch (const Nln::Exception & e) {
            throw DataError(e, jsonPath); // NOLINT(*-exception-baseclass)
        }
        throw DataError(Basic::Wcs::c_invalidValue, jsonPath); // NOLINT(*-exception-baseclass)
    }

    template<Meta::BackSideGrowingRange T>
    [[maybe_unused]]
    inline bool handle(
        const Nln::Json & json,
        T & variable,
        const Meta::Filter<typename T::value_type> auto & filter,
        const std::wstring & jsonPath = {}
    ) try {
        if (json.is_array()) {
            for (auto & j : json) {
                variable.push_back(filter(cast<typename T::value_type>(j)));
            }
            return true;
        } else if (json.is_null()) {
            return false;
        }
        throw DataError(Basic::Wcs::c_invalidValue, jsonPath); // NOLINT(*-exception-baseclass)
    } catch (DataError & e) {
        e.variable(jsonPath);
        throw;
    } catch (const Nln::Exception & e) {
        throw DataError(e, jsonPath); // NOLINT(*-exception-baseclass)
    }

    template<Meta::BackSideGrowingRange T>
    [[maybe_unused]]
    inline bool handle(
        const Nln::Json & json,
        const std::string_view key,
        T & variable,
        const Meta::Filter<typename T::value_type> auto & filter,
        const std::wstring & baseJsonPath = {}
    ) {
        std::wstring jsonPath { baseJsonPath };
        Text::joinTo(jsonPath, Text::convert(key), L".");
        try {
            if (json.is_object()) {
                if (json.contains(key)) {
                    return handle<T>(json[key], variable, filter, jsonPath);
                }
                return false;
            } else if (json.is_null()) {
                return false;
            }
        } catch (DataError & e) {
            e.variable(jsonPath);
            throw;
        } catch (const Nln::Exception & e) {
            throw DataError(e, jsonPath); // NOLINT(*-exception-baseclass)
        }
        throw DataError(Basic::Wcs::c_invalidValue, jsonPath); // NOLINT(*-exception-baseclass)
    }
}
