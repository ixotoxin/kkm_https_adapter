// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "service_defaults.h"
#include "service_strings.h"
#include "service_core.h"
#include "server_defaults.h"
#include "server_core.h"
#include <lib/except.h>
#include <lib/utils.h>
#include <log/write.h>
#include <optional>
#include <winsvc.h>

namespace Service {
    using Basic::Failure;

    static_assert(c_waitHint >= c_sleepQuantum.count());
    static_assert(c_waitHint > Server::c_controlTimeout.count());
    static_assert(c_controlTimeout >= c_sleepQuantum.count());
    static_assert(c_controlTimeout > Server::c_controlTimeout.count());

    namespace Worker {
        static ::SERVICE_STATUS s_status {};
        static ::SERVICE_STATUS_HANDLE s_statusHandle { nullptr };
        static ::DWORD s_checkPoint { 0 };

        void setStatus(::DWORD state, ::DWORD win32ExitCode = NO_ERROR) noexcept {
            s_status.dwCurrentState = state;
            s_status.dwWin32ExitCode = win32ExitCode;

            if (state == SERVICE_RUNNING || state == SERVICE_STOPPED) {
                s_status.dwCheckPoint = 0;
                s_status.dwWaitHint = 0;
            } else {
                s_status.dwCheckPoint = ++s_checkPoint;
                s_status.dwWaitHint = c_waitHint;
            }

            ::SetServiceStatus(s_statusHandle, &s_status);
        }

        void start() noexcept {
            try {
                setStatus(SERVICE_START_PENDING);
                Server::start();
                setStatus(SERVICE_RUNNING);
                return; /** Не удаляй, смотри дальше. **/
            } catch (const Failure & e) {
                tsLogError(e);
            } catch (const std::exception & e) {
                tsLogError(e);
            } catch (...) {
                tsLogError(Wcs::c_startingFailed);
            }

            setStatus(SERVICE_STOPPED, NO_ERROR);
        }

        void stop() noexcept {
            ::DWORD originalState { s_status.dwCurrentState };

            try {
                setStatus(SERVICE_STOP_PENDING);
                Server::stop();
                setStatus(SERVICE_STOPPED);
                return; /** Не удаляй, смотри дальше. **/
            } catch (const Failure & e) {
                tsLogError(e);
            } catch (const std::exception & e) {
                tsLogError(e);
            } catch (...) {
                tsLogError(Wcs::c_startingFailed);
            }

            setStatus(originalState);
        }

        void WINAPI handler(::DWORD ctrl) noexcept {
            if (ctrl == SERVICE_CONTROL_STOP) {
                stop();
            }
        }

        void WINAPI main(::DWORD, ::PWSTR *) noexcept {
            s_statusHandle = ::RegisterServiceCtrlHandlerW(c_systemName, handler);
            if (s_statusHandle) {
                start();
            } else {
                tsLogError(System::explainError());
            }
        }

        void run() {
            s_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
            s_status.dwCurrentState = SERVICE_START_PENDING;
            s_status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
            s_status.dwWin32ExitCode = NO_ERROR;
            s_status.dwServiceSpecificExitCode = 0;
            s_status.dwCheckPoint = 0;
            s_status.dwWaitHint = 0;

            ::SERVICE_TABLE_ENTRYW serviceTable[] {
                { const_cast<::LPWSTR>(c_systemName), main },
                { nullptr, nullptr }
            };

            if (!::StartServiceCtrlDispatcherW(serviceTable)) {
                throw Failure(System::explainError(L"StartServiceCtrlDispatcherW(...)")); // NOLINT(*-exception-baseclass)
            }
        }
    }

    namespace Control {
        inline void queryStatus(::SC_HANDLE & service, ::SERVICE_STATUS_PROCESS & status) {
            ::DWORD bytesNeeded;

            auto result
                = ::QueryServiceStatusEx(
                    service, SC_STATUS_PROCESS_INFO, (::LPBYTE) &status, sizeof(::SERVICE_STATUS_PROCESS), &bytesNeeded
                );

            if (!result) {
                throw Failure(System::explainError(L"QueryServiceStatusEx(...)")); // NOLINT(*-exception-baseclass)
            }
        }

        inline ::DWORD waitNewState(
            ::SC_HANDLE & service,
            ::SERVICE_STATUS_PROCESS & status,
            ::DWORD value,
            std::wstring_view message
        ) {
            auto checkPoint = status.dwCheckPoint;
            auto initialTicks = ::GetTickCount();
            auto ticks = initialTicks;

            do {
                cliMsgDebug(message);
                ::Sleep(c_sleepQuantum);
                queryStatus(service, status);
                auto currTicks = ::GetTickCount();
                if (status.dwCheckPoint > checkPoint) {
                    checkPoint = status.dwCheckPoint;
                    ticks = currTicks;
                } else if (currTicks - ticks > status.dwWaitHint || currTicks - initialTicks > c_controlTimeout) {
                    break;
                }
            } while (status.dwCurrentState == value);

            return status.dwCurrentState;
        }

        inline ::DWORD waitNewState(
            ::SC_HANDLE & service,
            ::DWORD value,
            std::wstring_view message
        ) {
            ::SERVICE_STATUS_PROCESS status {};

            queryStatus(service, status);

            if (status.dwCurrentState == value) {
                return waitNewState(service, status, value, message);
            }

            return status.dwCurrentState;
        }

        void start() {
            ::SC_HANDLE manager { nullptr };
            ::SC_HANDLE service { nullptr };

            Deferred::Exec closeHandlers {
                [& manager, & service] {
                    if (service) {
                        ::CloseServiceHandle(service);
                    }
                    if (manager) {
                        ::CloseServiceHandle(manager);
                    }
                }
            };

            constexpr auto scmPermission = SC_MANAGER_ALL_ACCESS;
            manager = ::OpenSCManagerW(nullptr, nullptr, scmPermission);
            if (!manager) {
                throw Failure(System::explainError(L"OpenSCManagerW(...)")); // NOLINT(*-exception-baseclass)
            }

            constexpr auto svcPermission = SERVICE_ALL_ACCESS;
            service = ::OpenServiceW(manager, c_systemName, svcPermission);
            if (!service) {
                throw Failure(System::explainError(L"OpenServiceW(...)")); // NOLINT(*-exception-baseclass)
            }

            auto state = waitNewState(service, SERVICE_STOP_PENDING, Wcs::c_stopping);
            if (state == SERVICE_STOP_PENDING) {
                throw Failure(Wcs::c_stoppingTimeout); // NOLINT(*-exception-baseclass)
            }
            if (state != SERVICE_STOPPED) {
                cliMsgWarning(Wcs::c_alreadyStarted);
                return;
            }

            if (!::StartServiceW(service, 0, nullptr)) {
                throw Failure(System::explainError(L"StartServiceW(...)")); // NOLINT(*-exception-baseclass)
            }

            state = waitNewState(service, SERVICE_START_PENDING, Wcs::c_starting);
            if (state != SERVICE_RUNNING) {
                throw Failure(Wcs::c_startingFailed); // NOLINT(*-exception-baseclass)
            }

            cliMsgInfo(Wcs::c_started);
        }

        inline void stop(::SC_HANDLE & service, bool logAlreadyStopped = true) {
            ::SERVICE_STATUS_PROCESS status {};

            queryStatus(service, status);

            if (status.dwCurrentState == SERVICE_STOPPED) {
                if (logAlreadyStopped) {
                    cliMsgWarning(Wcs::c_alreadyStopped);
                }
                return;
            }

            if (status.dwCurrentState != SERVICE_STOP_PENDING) {
                // ISSUE: Возможно стоит реализовать остановку зависимых служб, вдруг что-нибудь захочет позависеть.
                //  Тогда нам сюда:
                //      https://learn.microsoft.com/en-us/windows/win32/services/service-control-program-tasks
                //      https://learn.microsoft.com/en-us/windows/win32/services/svccontrol-cpp
                if (!::ControlService(service, SERVICE_CONTROL_STOP, reinterpret_cast<::LPSERVICE_STATUS>(&status))) {
                    throw Failure(System::explainError(L"ControlService(...)")); // NOLINT(*-exception-baseclass)
                }
            }

            if (waitNewState(service, status, SERVICE_STOP_PENDING, Wcs::c_stopping) != SERVICE_STOPPED) {
                throw Failure(Wcs::c_stoppingTimeout); // NOLINT(*-exception-baseclass)
            }

            cliMsgInfo(Wcs::c_stopped);
        }

        void stop() {
            ::SC_HANDLE manager { nullptr };
            ::SC_HANDLE service { nullptr };

            Deferred::Exec closeHandlers {
                [& manager, & service] {
                    if (service) {
                        ::CloseServiceHandle(service);
                    }
                    if (manager) {
                        ::CloseServiceHandle(manager);
                    }
                }
            };

            constexpr auto scmPermission = SC_MANAGER_ALL_ACCESS;
            manager = ::OpenSCManagerW(nullptr, nullptr, scmPermission);
            if (!manager) {
                throw Failure(System::explainError(L"OpenSCManagerW(...)")); // NOLINT(*-exception-baseclass)
            }

            constexpr auto svcPermission = SERVICE_STOP | SERVICE_QUERY_STATUS | SERVICE_ENUMERATE_DEPENDENTS;
            service = ::OpenServiceW(manager, c_systemName, svcPermission);
            if (!service) {
                throw Failure(System::explainError(L"OpenServiceW(...)")); // NOLINT(*-exception-baseclass)
            }

            stop(service);
        }

        void install(const std::wstring & command) {
            ::SC_HANDLE manager { nullptr };
            ::SC_HANDLE service { nullptr };

            Deferred::Exec closeHandlers {
                [& manager, & service] {
                    if (service) {
                        ::CloseServiceHandle(service);
                    }
                    if (manager) {
                        ::CloseServiceHandle(manager);
                    }
                }
            };

            constexpr auto scmPermission = SC_MANAGER_CONNECT | SC_MANAGER_CREATE_SERVICE;
            manager = ::OpenSCManagerW(nullptr, nullptr, scmPermission);
            if (!manager) {
                throw Failure(System::explainError(L"OpenSCManagerW(...)")); // NOLINT(*-exception-baseclass)
            }

            service
                = ::CreateServiceW(
                    manager,                    // SCManager database
                    c_systemName,               // Name of service
                    c_displayName,              // Name to display
                    SERVICE_QUERY_STATUS,       // Desired access
                    SERVICE_WIN32_OWN_PROCESS,  // Service type
                    c_startType,                // Service start type
                    SERVICE_ERROR_NORMAL,       // Error control type
                    command.c_str(),            // Service's binary / command
                    nullptr,                    // No load ordering group
                    nullptr,                    // No tag identifier
                    c_dependencies,             // Dependencies
                    c_account,                  // Service running account
                    c_password                  // Password of the account
                );

            if (!service) {
                throw Failure(System::explainError(L"CreateServiceW(...)")); // NOLINT(*-exception-baseclass)
            }

            cliMsgInfo(Wcs::c_installed);
        }

        void uninstall() {
            ::SC_HANDLE manager { nullptr };
            ::SC_HANDLE service { nullptr };

            Deferred::Exec closeHandlers {
                [& manager, & service] {
                    if (service) {
                        ::CloseServiceHandle(service);
                    }
                    if (manager) {
                        ::CloseServiceHandle(manager);
                    }
                }
            };

            constexpr auto scmPermission = SC_MANAGER_CONNECT;
            manager = ::OpenSCManagerW(nullptr, nullptr, scmPermission);
            if (!manager) {
                throw Failure(System::explainError(L"OpenSCManagerW(...)")); // NOLINT(*-exception-baseclass)
            }

            constexpr auto svcPermission = SERVICE_STOP | SERVICE_QUERY_STATUS | DELETE;
            service = ::OpenServiceW(manager, c_systemName, svcPermission);
            if (!service) {
                throw Failure(System::explainError(L"OpenServiceW(...)")); // NOLINT(*-exception-baseclass)
            }

            stop(service, false);

            if (!::DeleteService(service)) {
                throw Failure(System::explainError(L"DeleteService(...)")); // NOLINT(*-exception-baseclass)
            }

            cliMsgInfo(Wcs::c_uninstalled);
        }
    }
}
