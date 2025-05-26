// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "basic.h"
#include <nlohmann/json.hpp>
#include "numeric.h"
#include "text.h"

namespace Nln {
    using Json = nlohmann::json;
    constexpr auto EmptyJsonObject = nlohmann::json::value_t::object;
}

namespace Json {
    namespace Wcs {
        using Basic::Wcs::c_invalidValue;
        using Basic::Wcs::c_rangeError;
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
        throw DataError(Wcs::c_invalidValue); // NOLINT(*-exception-baseclass)
    } catch (DataError &) {
        throw;
    } catch (Failure & e) {
        throw DataError(std::move(e)); // NOLINT(*-exception-baseclass)
    } catch (const std::exception & e) {
        throw DataError(e); // NOLINT(*-exception-baseclass)
    } catch (...) {
        throw DataError(Wcs::c_invalidValue); // NOLINT(*-exception-baseclass)
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
        throw DataError(Wcs::c_invalidValue); // NOLINT(*-exception-baseclass)
    } catch (DataError &) {
        throw;
    } catch (Failure & e) {
        throw DataError(std::move(e)); // NOLINT(*-exception-baseclass)
    } catch (const std::exception & e) {
        throw DataError(e); // NOLINT(*-exception-baseclass)
    } catch (...) {
        throw DataError(Wcs::c_invalidValue); // NOLINT(*-exception-baseclass)
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
        throw DataError(Wcs::c_invalidValue); // NOLINT(*-exception-baseclass)
    } catch (DataError &) {
        throw;
    } catch (Failure & e) {
        throw DataError(std::move(e)); // NOLINT(*-exception-baseclass)
    } catch (const std::exception & e) {
        throw DataError(e); // NOLINT(*-exception-baseclass)
    } catch (...) {
        throw DataError(Wcs::c_invalidValue); // NOLINT(*-exception-baseclass)
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
        throw DataError(Wcs::c_invalidValue); // NOLINT(*-exception-baseclass)
    } catch (DataError &) {
        throw;
    } catch (Failure & e) {
        throw DataError(std::move(e)); // NOLINT(*-exception-baseclass)
    } catch (const std::exception & e) {
        throw DataError(e); // NOLINT(*-exception-baseclass)
    } catch (...) {
        throw DataError(Wcs::c_invalidValue); // NOLINT(*-exception-baseclass)
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
    } catch (Failure & e) {
        throw DataError(std::move(e), jsonPath); // NOLINT(*-exception-baseclass)
    } catch (const std::exception & e) {
        throw DataError(e, jsonPath); // NOLINT(*-exception-baseclass)
    } catch (...) {
        throw DataError(Wcs::c_invalidValue, jsonPath); // NOLINT(*-exception-baseclass)
    }

    [[maybe_unused]]
    inline bool handleKey(
        const Nln::Json & json,
        const std::string_view key,
        const Handler & handler,
        const std::wstring & baseJsonPath = {}
    ) {
        assert(json.is_object());
        auto jsonPath { baseJsonPath };
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
        } catch (Failure & e) {
            throw DataError(std::move(e), jsonPath); // NOLINT(*-exception-baseclass)
        } catch (const std::exception & e) {
            throw DataError(e, jsonPath); // NOLINT(*-exception-baseclass)
        } catch (...) {}
        throw DataError(Wcs::c_invalidValue, jsonPath); // NOLINT(*-exception-baseclass)
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
    } catch (Failure & e) {
        throw DataError(std::move(e), jsonPath); // NOLINT(*-exception-baseclass)
    } catch (const std::exception & e) {
        throw DataError(e, jsonPath); // NOLINT(*-exception-baseclass)
    } catch (...) {
        throw DataError(Wcs::c_invalidValue, jsonPath); // NOLINT(*-exception-baseclass)
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
        auto jsonPath { baseJsonPath };
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
        } catch (Failure & e) {
            throw DataError(std::move(e), jsonPath); // NOLINT(*-exception-baseclass)
        } catch (const std::exception & e) {
            throw DataError(e, jsonPath); // NOLINT(*-exception-baseclass)
        } catch (...) {}
        throw DataError(Wcs::c_invalidValue, jsonPath); // NOLINT(*-exception-baseclass)
    }

    template<typename T>
    [[maybe_unused]]
    inline bool handle(
        const Nln::Json & json,
        T & variable,
        const Meta::Filter<T> & filter,
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
    } catch (Failure & e) {
        throw DataError(std::move(e), jsonPath); // NOLINT(*-exception-baseclass)
    } catch (const std::exception & e) {
        throw DataError(e, jsonPath); // NOLINT(*-exception-baseclass)
    } catch (...) {
        throw DataError(Wcs::c_invalidValue, jsonPath); // NOLINT(*-exception-baseclass)
    }

    template<typename T>
    [[maybe_unused]]
    inline bool handleKey(
        const Nln::Json & json,
        const std::string_view key,
        T & variable,
        const Meta::Filter<T> & filter,
        const std::wstring & baseJsonPath = {}
    ) {
        assert(json.is_object());
        auto jsonPath { baseJsonPath };
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
        } catch (Failure & e) {
            throw DataError(std::move(e), jsonPath); // NOLINT(*-exception-baseclass)
        } catch (const std::exception & e) {
            throw DataError(e, jsonPath); // NOLINT(*-exception-baseclass)
        } catch (...) {}
        throw DataError(Wcs::c_invalidValue, jsonPath); // NOLINT(*-exception-baseclass)
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
            throw DataError(Wcs::c_rangeError, jsonPath); // NOLINT(*-exception-baseclass)
        }
        variable = it->second;
        return true;
    } catch (DataError & e) {
        e.variable(jsonPath);
        throw;
    } catch (Failure & e) {
        throw DataError(std::move(e), jsonPath); // NOLINT(*-exception-baseclass)
    } catch (const std::exception & e) {
        throw DataError(e, jsonPath); // NOLINT(*-exception-baseclass)
    } catch (...) {
        throw DataError(Wcs::c_invalidValue, jsonPath); // NOLINT(*-exception-baseclass)
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
        auto jsonPath { baseJsonPath };
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
        } catch (Failure & e) {
            throw DataError(std::move(e), jsonPath); // NOLINT(*-exception-baseclass)
        } catch (const std::exception & e) {
            throw DataError(e, jsonPath); // NOLINT(*-exception-baseclass)
        } catch (...) {}
        throw DataError(Wcs::c_invalidValue, jsonPath); // NOLINT(*-exception-baseclass)
    }

    template<typename T>
    [[maybe_unused]]
    inline bool handle(
        const Nln::Json & json,
        T & variable,
        const Meta::EnumCastMap<T> auto & castMap,
        const Meta::Filter<T> & filter,
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
    } catch (Failure & e) {
        throw DataError(std::move(e), jsonPath); // NOLINT(*-exception-baseclass)
    } catch (const std::exception & e) {
        throw DataError(e, jsonPath); // NOLINT(*-exception-baseclass)
    } catch (...) {
        throw DataError(Wcs::c_invalidValue, jsonPath); // NOLINT(*-exception-baseclass)
    }

    template<typename T>
    [[maybe_unused]]
    inline bool handleKey(
        const Nln::Json & json,
        const std::string_view key,
        T & variable,
        const Meta::EnumCastMap<T> auto & castMap,
        const Meta::Filter<T> & filter,
        const std::wstring & baseJsonPath = {}
    ) {
        assert(json.is_object());
        auto jsonPath { baseJsonPath };
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
        } catch (Failure & e) {
            throw DataError(std::move(e), jsonPath); // NOLINT(*-exception-baseclass)
        } catch (const std::exception & e) {
            throw DataError(e, jsonPath); // NOLINT(*-exception-baseclass)
        } catch (...) {}
        throw DataError(Wcs::c_invalidValue, jsonPath); // NOLINT(*-exception-baseclass)
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
            throw DataError(Wcs::c_rangeError, jsonPath);
        }
        variable = value;
        return true;
    } catch (DataError & e) {
        e.variable(jsonPath);
        throw;
    } catch (Failure & e) {
        throw DataError(std::move(e), jsonPath); // NOLINT(*-exception-baseclass)
    } catch (const std::exception & e) {
        throw DataError(e, jsonPath); // NOLINT(*-exception-baseclass)
    } catch (...) {
        throw DataError(Wcs::c_invalidValue, jsonPath); // NOLINT(*-exception-baseclass)
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
        auto jsonPath { baseJsonPath };
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
        } catch (Failure & e) {
            throw DataError(std::move(e), jsonPath); // NOLINT(*-exception-baseclass)
        } catch (const std::exception & e) {
            throw DataError(e, jsonPath); // NOLINT(*-exception-baseclass)
        } catch (...) {}
        throw DataError(Wcs::c_invalidValue, jsonPath); // NOLINT(*-exception-baseclass)
    }

    template<typename T>
    [[maybe_unused]]
    inline bool handle(
        const Nln::Json & json,
        T & variable,
        const Meta::EnumDomain<T> auto & domain,
        const Meta::Filter<T> & filter,
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
    } catch (Failure & e) {
        throw DataError(std::move(e), jsonPath); // NOLINT(*-exception-baseclass)
    } catch (const std::exception & e) {
        throw DataError(e, jsonPath); // NOLINT(*-exception-baseclass)
    } catch (...) {
        throw DataError(Wcs::c_invalidValue, jsonPath); // NOLINT(*-exception-baseclass)
    }

    template<typename T>
    [[maybe_unused]]
    inline bool handleKey(
        const Nln::Json & json,
        const std::string_view key,
        T & variable,
        const Meta::EnumDomain<T> auto & domain,
        const Meta::Filter<T> & filter,
        const std::wstring & baseJsonPath = {}
    ) {
        assert(json.is_object());
        auto jsonPath { baseJsonPath };
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
        } catch (Failure & e) {
            throw DataError(std::move(e), jsonPath); // NOLINT(*-exception-baseclass)
        } catch (const std::exception & e) {
            throw DataError(e, jsonPath); // NOLINT(*-exception-baseclass)
        } catch (...) {}
        throw DataError(Wcs::c_invalidValue, jsonPath); // NOLINT(*-exception-baseclass)
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
        throw DataError(Wcs::c_invalidValue, jsonPath); // NOLINT(*-exception-baseclass)
    } catch (DataError & e) {
        e.variable(jsonPath);
        throw;
    } catch (Failure & e) {
        throw DataError(std::move(e), jsonPath); // NOLINT(*-exception-baseclass)
    } catch (const std::exception & e) {
        throw DataError(e, jsonPath); // NOLINT(*-exception-baseclass)
    } catch (...) {
        throw DataError(Wcs::c_invalidValue, jsonPath); // NOLINT(*-exception-baseclass)
    }

    [[maybe_unused]]
    inline bool handleKey(
        const Nln::Json & json,
        const std::string_view key,
        Meta::BackSideGrowingRange auto & variable,
        const std::wstring & baseJsonPath = {}
    ) {
        assert(json.is_object());
        auto jsonPath { baseJsonPath };
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
        } catch (Failure & e) {
            throw DataError(std::move(e), jsonPath); // NOLINT(*-exception-baseclass)
        } catch (const std::exception & e) {
            throw DataError(e, jsonPath); // NOLINT(*-exception-baseclass)
        } catch (...) {}
        throw DataError(Wcs::c_invalidValue, jsonPath); // NOLINT(*-exception-baseclass)
    }

    template<Meta::BackSideGrowingRange T>
    [[maybe_unused]]
    inline bool handle(
        const Nln::Json & json,
        T & variable,
        const Meta::Filter<typename T::value_type> & filter,
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
        throw DataError(Wcs::c_invalidValue, jsonPath); // NOLINT(*-exception-baseclass)
    } catch (DataError & e) {
        e.variable(jsonPath);
        throw;
    } catch (Failure & e) {
        throw DataError(std::move(e), jsonPath); // NOLINT(*-exception-baseclass)
    } catch (const std::exception & e) {
        throw DataError(e, jsonPath); // NOLINT(*-exception-baseclass)
    } catch (...) {
        throw DataError(Wcs::c_invalidValue, jsonPath); // NOLINT(*-exception-baseclass)
    }

    template<Meta::BackSideGrowingRange T>
    [[maybe_unused]]
    inline bool handle(
        const Nln::Json & json,
        const std::string_view key,
        T & variable,
        const Meta::Filter<typename T::value_type> & filter,
        const std::wstring & baseJsonPath = {}
    ) {
        auto jsonPath { baseJsonPath };
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
        } catch (Failure & e) {
            throw DataError(std::move(e), jsonPath); // NOLINT(*-exception-baseclass)
        } catch (const std::exception & e) {
            throw DataError(e, jsonPath); // NOLINT(*-exception-baseclass)
        } catch (...) {}
        throw DataError(Wcs::c_invalidValue, jsonPath); // NOLINT(*-exception-baseclass)
    }
}
