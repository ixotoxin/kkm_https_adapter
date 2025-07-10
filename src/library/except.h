// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "basic.h"
#include <system_error>
#include "wconv.h"
#include "slapp.h"

namespace Basic {
    namespace Wcs {
        using Basic::Wcs::c_invalidVariable;
    }

    class Failure {
        std::wstring m_message {};

    protected:
        std::source_location m_location;

    public:
        Failure() = delete;
        Failure(const Failure &) = default;
        Failure(Failure &&) = default;

        [[maybe_unused]]
        explicit Failure(
            const std::wstring_view message,
            std::source_location && location = std::source_location::current()
        ) : m_message(message.begin(), message.end()), m_location(std::forward<std::source_location>(location)) {}

        [[maybe_unused]]
        explicit Failure(
            const std::string_view message,
            std::source_location && location = std::source_location::current()
        ) : m_message(Text::convert(message)), m_location(std::forward<std::source_location>(location)) {}

        [[maybe_unused]]
        explicit Failure(
            const std::exception & e,
            std::source_location && location = std::source_location::current()
        ) : m_message(Text::convert(e.what())), m_location(std::forward<std::source_location>(location)) {}

        [[maybe_unused]]
        explicit Failure(
            const std::error_code & e,
            std::source_location && location = std::source_location::current()
        ) : m_message(Text::convert(e.message())), m_location(std::forward<std::source_location>(location)) {}

        virtual ~Failure() = default;

        Failure & operator=(const Failure &) = default;
        Failure & operator=(Failure &&) = default;

        [[nodiscard, maybe_unused]]
        const std::wstring & what() const noexcept {
            return m_message;
        }

        [[nodiscard, maybe_unused]]
        const std::source_location & where() const noexcept {
            return m_location;
        }

        [[nodiscard, maybe_unused]]
        virtual std::wstring explain() const noexcept {
            if (Log::s_appendLocation) {
                return what() + m_location;
            } else {
                return what();
            }
        }
    };

    class DataError : public Basic::Failure {
        std::wstring m_variable;

    public:
        DataError() = delete;
        DataError(const DataError &) = default;
        DataError(DataError &&) = default;

        [[maybe_unused]]
        explicit DataError(
            Basic::Failure && e,
            const std::wstring_view variable = {}
        ) : Failure(std::forward<Basic::Failure>(e)), m_variable(variable) {}

        [[maybe_unused]]
        explicit DataError(
            const std::wstring_view message,
            const std::wstring_view variable = {},
            std::source_location && location = std::source_location::current()
        ) : Failure(message, std::forward<std::source_location>(location)), m_variable(variable) {}

        [[maybe_unused]]
        explicit DataError(
            const std::string_view message,
            const std::wstring_view variable = {},
            std::source_location && location = std::source_location::current()
        ) : Failure(message, std::forward<std::source_location>(location)), m_variable(variable) {}

        [[maybe_unused]]
        explicit DataError(
            const std::exception & e,
            const std::wstring_view variable = {},
            std::source_location && location = std::source_location::current()
        ) : Failure(e, std::forward<std::source_location>(location)), m_variable(variable) {}

        [[maybe_unused]]
        explicit DataError(
            const std::error_code & e,
            const std::wstring_view variable = {},
            std::source_location && location = std::source_location::current()
        ) : Failure(e, std::forward<std::source_location>(location)), m_variable(variable) {}

        ~DataError() override = default;

        DataError & operator=(const DataError &) = default;
        DataError & operator=(DataError &&) = default;

        [[maybe_unused]]
        void variable(const std::wstring_view variable, bool override = false) noexcept {
            if (override || (m_variable.empty() && !variable.empty())) {
                m_variable.assign(variable);
            }
        }

        [[nodiscard, maybe_unused]]
        const std::wstring & variable() const noexcept {
            return m_variable;
        }

        [[nodiscard, maybe_unused]]
        std::wstring explain() const noexcept override {
            auto message { m_variable.empty() ? what() : std::format(Wcs::c_invalidVariable, m_variable, what()) };
            if (Log::s_appendLocation) {
                message.append(L" (в файле ");
                Basic::append(message, m_location);
                message.append(L")");
            }
            return message;
        }
    };
}
