// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "service.h"
#include "library/utils.h"
#include "library/except.h"
#include <filesystem>
#include <optional>
#include <winsvc.h>
#include "messages.h"
#include "log.h"
#include "server.h"

namespace Service {
    using Basic::Failure;

    static ::SERVICE_STATUS s_status {};
    static ::SERVICE_STATUS_HANDLE s_statusHandle { nullptr };
    static DWORD s_checkPoint { 1 };

    void setServiceStatus(DWORD state, DWORD win32ExitCode = NO_ERROR) {
        s_status.dwCurrentState = state;
        s_status.dwWin32ExitCode = win32ExitCode;
        s_status.dwWaitHint = 0;
        s_status.dwCheckPoint = state == SERVICE_RUNNING || state == SERVICE_STOPPED ? 0 : s_checkPoint++;
        ::SetServiceStatus(s_statusHandle, &s_status);
    }

    void startWorker() {
        std::optional<DWORD> error = NO_ERROR;

        try {
            setServiceStatus(SERVICE_START_PENDING);
            Server::start();
            setServiceStatus(SERVICE_RUNNING);
            error = std::nullopt;

        // TODO: Исправить перехват исключений
        } catch (const Failure & e) {
            tsLogError(e);
        } catch (const std::exception & e) {
            tsLogError(e);
        } catch (...) {
            tsLogError(Wcs::c_startingFailed);
        }

        if (error.has_value()) {
            setServiceStatus(SERVICE_STOPPED, *error);
        }
    }

    void stopWorker() {
        std::optional<DWORD> originalState = s_status.dwCurrentState;

        try {
            setServiceStatus(SERVICE_STOP_PENDING);
            Server::stop();
            setServiceStatus(SERVICE_STOPPED);
            originalState = std::nullopt;

        // TODO: Исправить перехват исключений
        } catch (const Failure & e) {
            tsLogError(e);
        } catch (const std::exception & e) {
            tsLogError(e);
        } catch (...) {
            tsLogError(Wcs::c_startingFailed);
        }

        if (originalState.has_value()) {
            setServiceStatus(*originalState);
        }
    }

    void WINAPI controlHandler(DWORD ctrl) {
        if (ctrl == SERVICE_CONTROL_STOP) {
            stopWorker();
        }
    }

    void WINAPI mainFunc(DWORD, PWSTR *) {
        s_statusHandle = ::RegisterServiceCtrlHandlerW(c_systemName, controlHandler);
        if (!s_statusHandle) {
            throw Failure(System::explainError()); // NOLINT(*-exception-baseclass)
        }
        startWorker();
    }

    void run() {
        s_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
        s_status.dwCurrentState = SERVICE_START_PENDING;
        s_status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
        s_status.dwWin32ExitCode = NO_ERROR;
        s_status.dwServiceSpecificExitCode = 0;
        s_status.dwCheckPoint = 0;
        s_status.dwWaitHint = 0;
        ::SERVICE_TABLE_ENTRYW serviceTable[] = { { const_cast<LPWSTR>(c_systemName), mainFunc }, { nullptr, nullptr } };

        if (!::StartServiceCtrlDispatcherW(serviceTable)) {
            throw Failure(System::explainError(L"StartServiceCtrlDispatcherW(...)")); // NOLINT(*-exception-baseclass)
        }
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

        manager = ::OpenSCManagerW(nullptr, nullptr, SC_MANAGER_CONNECT | SC_MANAGER_CREATE_SERVICE);
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

        ntsLogInfo(Wcs::c_installed);
    }

    void uninstall() {
        ::SC_HANDLE manager { nullptr };
        ::SC_HANDLE service { nullptr };
        ::SERVICE_STATUS status {};

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

        manager = ::OpenSCManagerW(nullptr, nullptr, SC_MANAGER_CONNECT);
        if (!manager) {
            throw Failure(System::explainError(L"OpenSCManagerW(...)")); // NOLINT(*-exception-baseclass)
        }

        service = ::OpenServiceW(manager, c_systemName, SERVICE_STOP | SERVICE_QUERY_STATUS | DELETE);
        if (!service) {
            throw Failure(System::explainError(L"OpenServiceW(...)")); // NOLINT(*-exception-baseclass)
        }

        if (::ControlService(service, SERVICE_CONTROL_STOP, &status)) {
            bool queryStatus;
            do {
                Log::Console::write(Log::Level::Debug, Wcs::c_stopping);
                ::Sleep(1000);
                queryStatus = ::QueryServiceStatus(service, &status);
            } while (queryStatus && status.dwCurrentState == SERVICE_STOP_PENDING);
            if (status.dwCurrentState == SERVICE_STOPPED) {
                ntsLogInfo(Wcs::c_stopped);
            } else {
                throw Failure(Wcs::c_stoppingFailed); // NOLINT(*-exception-baseclass)
            }
        }

        if (!::DeleteService(service)) {
            throw Failure(System::explainError(L"DeleteService(...)")); // NOLINT(*-exception-baseclass)
        }

        ntsLogInfo(Wcs::c_uninstalled);
    }

    inline void queryServiceStatus(SC_HANDLE & service, SERVICE_STATUS_PROCESS & status) {
        DWORD bytesNeeded;

        auto result
            = ::QueryServiceStatusEx(
                service, SC_STATUS_PROCESS_INFO, (LPBYTE) &status, sizeof(SERVICE_STATUS_PROCESS), &bytesNeeded
            );

        if (!result) {
            throw Failure(System::explainError(L"QueryServiceStatusEx(...)")); // NOLINT(*-exception-baseclass)
        }
    }

    void start() {
        ::SC_HANDLE manager { nullptr };
        ::SC_HANDLE service { nullptr };
        ::SERVICE_STATUS_PROCESS status {};
        DWORD tickCount;
        DWORD oldCheckPoint;
        DWORD waitTime;

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

        manager = ::OpenSCManagerW(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);
        if (!manager) {
            throw Failure(System::explainError(L"OpenSCManagerW(...)")); // NOLINT(*-exception-baseclass)
        }

        service = ::OpenServiceW(manager, c_systemName, SERVICE_ALL_ACCESS);
        if (!service) {
            throw Failure(System::explainError(L"OpenServiceW(...)")); // NOLINT(*-exception-baseclass)
        }

        queryServiceStatus(service, status);
        if(status.dwCurrentState != SERVICE_STOPPED && status.dwCurrentState != SERVICE_STOP_PENDING) {
            throw Failure(Wcs::c_alreadyStarted); // NOLINT(*-exception-baseclass)
        }

        tickCount = ::GetTickCount();
        oldCheckPoint = status.dwCheckPoint;

        while (status.dwCurrentState == SERVICE_STOP_PENDING) {
            Log::Console::write(Log::Level::Debug, Wcs::c_stopping);
            waitTime = std::clamp(status.dwWaitHint, 10'000ul, 100'000ul);
            ::Sleep(waitTime / 10);
            queryServiceStatus(service, status);
            if (status.dwCheckPoint > oldCheckPoint) {
                tickCount = ::GetTickCount();
                oldCheckPoint = status.dwCheckPoint;
            } else if(::GetTickCount() - tickCount > status.dwWaitHint) {
                throw Failure(Wcs::c_stoppingTimeout); // NOLINT(*-exception-baseclass)
            }
        }

        if (!::StartServiceW(service, 0, nullptr)) {
            throw Failure(System::explainError(L"StartServiceW(...)")); // NOLINT(*-exception-baseclass)
        }

        queryServiceStatus(service, status);
        tickCount = ::GetTickCount();
        oldCheckPoint = status.dwCheckPoint;

        while (status.dwCurrentState == SERVICE_START_PENDING) {
            Log::Console::write(Log::Level::Debug, Wcs::c_starting);
            waitTime = std::clamp(status.dwWaitHint, 10'000ul, 100'000ul);
            ::Sleep(waitTime / 10);
            queryServiceStatus(service, status);
            if (status.dwCheckPoint > oldCheckPoint) {
                tickCount = ::GetTickCount();
                oldCheckPoint = status.dwCheckPoint;
            } else if(::GetTickCount() - tickCount > status.dwWaitHint) {
                break;
            }
        }

        if (status.dwCurrentState == SERVICE_RUNNING) {
            ntsLogInfo(Wcs::c_started);
        } else {
            throw Failure(Wcs::c_startingFailed); // NOLINT(*-exception-baseclass)
        }
    }

    void stop() {
        ::SC_HANDLE manager { nullptr };
        ::SC_HANDLE service { nullptr };
        ::SERVICE_STATUS_PROCESS status {};
        DWORD startTime { ::GetTickCount() };
        DWORD waitTime;

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

        manager = ::OpenSCManagerW(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);
        if (!manager) {
            throw Failure(System::explainError(L"OpenSCManagerW(...)")); // NOLINT(*-exception-baseclass)
        }

        service
            = ::OpenServiceW(manager, c_systemName, SERVICE_STOP | SERVICE_QUERY_STATUS | SERVICE_ENUMERATE_DEPENDENTS);

        if (!service) {
            throw Failure(System::explainError(L"OpenServiceW(...)")); // NOLINT(*-exception-baseclass)
        }

        queryServiceStatus(service, status);
        if (status.dwCurrentState == SERVICE_STOPPED) {
            throw Failure(Wcs::c_alreadyStopped); // NOLINT(*-exception-baseclass)
        }

        if (status.dwCurrentState == SERVICE_STOP_PENDING) {
            do {
                Log::Console::write(Log::Level::Debug, Wcs::c_stopping);
                waitTime = std::clamp(status.dwWaitHint, 10'000ul, 100'000ul);
                ::Sleep(waitTime / 10);
                queryServiceStatus(service, status);
                if (status.dwCurrentState == SERVICE_STOPPED) {
                    break;
                }
                if (::GetTickCount() - startTime > c_stoppingTimeout) {
                    throw Failure(Wcs::c_stoppingTimeout); // NOLINT(*-exception-baseclass)
                }
            } while (status.dwCurrentState == SERVICE_STOP_PENDING);
        } else {
            // ISSUE: Возможно стоит реализовать остановку зависимых служб, вдруг что-нибудь захочет позависеть.
            //  Тогда нам сюда
            //  https://learn.microsoft.com/en-us/windows/win32/services/service-control-program-tasks
            //  https://learn.microsoft.com/en-us/windows/win32/services/svccontrol-cpp

            if (!::ControlService(service, SERVICE_CONTROL_STOP, (LPSERVICE_STATUS) &status)) {
                throw Failure(System::explainError(L"ControlService(...)")); // NOLINT(*-exception-baseclass)
            }

            while (status.dwCurrentState != SERVICE_STOPPED) {
                Log::Console::write(Log::Level::Debug, Wcs::c_stopping);
                ::Sleep(status.dwWaitHint );
                queryServiceStatus(service, status);
                if (status.dwCurrentState == SERVICE_STOPPED) {
                    break;
                }
                if (::GetTickCount() - startTime > c_stoppingTimeout) {
                    throw Failure(Wcs::c_stoppingTimeout); // NOLINT(*-exception-baseclass)
                }
            }
        }

        // if (status.dwCurrentState == SERVICE_STOPPED) {
            ntsLogInfo(Wcs::c_stopped);
        // } else {
        //     throw Failure(Wcs::c_stoppingFailed); // NOLINT(*-exception-baseclass)
        // }
    }
}
