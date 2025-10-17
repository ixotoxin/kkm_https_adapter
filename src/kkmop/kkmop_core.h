// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "kkmop_strings.h"
#include <log/write.h>
#include <kkm/variables.h>
#include <kkm/strings.h>
#include <kkm/device.h>
#include <kkm/callhelpers.h>
#include <cstdlib>
#include <optional>

namespace KkmOperator {
    using namespace Kkm;

    [[nodiscard]]
    inline int learn(const int connParamCount, wchar_t ** connParamItems) {
        assert(connParamCount > 0);
        Log::Console::ScopeLevelDown scopeLevel { Log::Level::Info };

        for (int i = 0, n = 1; i < connParamCount; ++i, ++n) {
            try {
                NewConnParams connParams { std::wstring { connParamItems[i] } };
                Device kkm { connParams, std::format(Wcs::c_commandPrefix, n) };
                std::wstring serialNumber { kkm.serialNumber() };
                LOG_DEBUG_NTS(Wcs::c_getKkmInfo, n, serialNumber);
                connParams.save(serialNumber);
                kkm.printHello();
                LOG_INFO_NTS(Wcs::c_connParamsSaved, n, serialNumber);
            } catch (const Failure & e) {
                LOG_WARNING_NTS(std::format(Wcs::c_prefixedText, n, e.explain(Log::s_appendLocation)));
            }
        }

        return EXIT_SUCCESS;
    }

    template<class R, class K>
    requires std::derived_from<std::remove_cvref_t<K>, Device>
    [[maybe_unused]]
    void callMethod(K && kkm, UndetailedMethod<R> method) {
        Log::Console::ScopeLevelDown scopeLevel { Log::Level::Info };
        R result {};
        (kkm.*method)(result);
        if (result.m_success) {
            LOG_INFO_CLI(L"Done");
        } else {
            throw Basic::Failure(result.m_message); // NOLINT(*-exception-baseclass)
        }
    }

    template<class R, class D, class K>
    requires std::derived_from<std::remove_cvref_t<K>, Device>
    [[maybe_unused]]
    void callMethod(K && kkm, DetailedMethod<R, D> method, D && details) {
        Log::Console::ScopeLevelDown scopeLevel { Log::Level::Info };
        R result {};
        (kkm.*method)(details, result);
        if (result.m_success) {
            LOG_INFO_CLI(L"Done");
        } else {
            throw Basic::Failure(result.m_message); // NOLINT(*-exception-baseclass)
        }
    }

    [[nodiscard]]
    inline std::optional<int> exec(const std::wstring & operation, wchar_t * serialNumber) {
        std::wstring serial { serialNumber };

        if (operation == L"status") {
            Device kkm { KnownConnParams { serial } };

            StatusResult statusResult {};
            kkm.getStatus(statusResult);
            if (!statusResult.m_success) {
                throw Basic::Failure(statusResult.m_message); // NOLINT(*-exception-baseclass)
            }

            ShiftStateResult shiftStateResult {};
            kkm.getShiftState(shiftStateResult);
            if (!shiftStateResult.m_success) {
                throw Basic::Failure(shiftStateResult.m_message); // NOLINT(*-exception-baseclass)
            }

            CashStatResult cashStatResult {};
            kkm.getCashStat(cashStatResult);
            if (!cashStatResult.m_success) {
                throw Basic::Failure(cashStatResult.m_message); // NOLINT(*-exception-baseclass)
            }

            FndtOfdExchangeStatusResult fndtOfdExchangeStatusResult {};
            kkm.getFndtOfdExchangeStatus(fndtOfdExchangeStatusResult);
            if (!fndtOfdExchangeStatusResult.m_success) {
                throw Basic::Failure(fndtOfdExchangeStatusResult.m_message); // NOLINT(*-exception-baseclass)
            }

            FndtErrorsResult fndtErrorsResult {};
            kkm.getFndtErrors(fndtErrorsResult);
            if (!fndtErrorsResult.m_success) {
                throw Basic::Failure(fndtErrorsResult.m_message); // NOLINT(*-exception-baseclass)
            }

            FfdVersionResult ffdVersionResult {};
            kkm.getFfdVersion(ffdVersionResult);
            if (!ffdVersionResult.m_success) {
                throw Basic::Failure(ffdVersionResult.m_message); // NOLINT(*-exception-baseclass)
            }

            FwVersionResult fwVersionResult {};
            kkm.getFwVersion(fwVersionResult);
            if (!fwVersionResult.m_success) {
                throw Basic::Failure(fwVersionResult.m_message); // NOLINT(*-exception-baseclass)
            }

            {
                Log::Console::ScopeLevelDown scopeLevel { Log::Level::Info };
                // LOG_INFO_CLI(std::format(Wcs::c_fmtModel, wcsSafeGet(Mbs::c_models, statusResult.m_model)));
                LOG_INFO_CLI(std::format(Wcs::c_fmtModel, statusResult.m_modelName));
                LOG_INFO_CLI(std::format(Wcs::c_fmtSerialNumber, statusResult.m_serialNumber));
                LOG_INFO_CLI(std::format(Wcs::c_fmtBlocked, Text::Wcs::daNet(statusResult.m_blocked)));
                LOG_INFO_CLI(std::format(Wcs::c_fmtLogicalNumber, statusResult.m_logicalNumber));
                LOG_INFO_CLI(std::format(Wcs::c_fmtDateTime, DateTime::cast<std::wstring>(statusResult.m_dateTime)));
                LOG_INFO_CLI(std::format(Wcs::c_fmtFnPresent, Text::Wcs::daNet(statusResult.m_fnPresent)));
                LOG_INFO_CLI(std::format(Wcs::c_fmtFnFiscal, Text::Wcs::daNet(statusResult.m_fnFiscal)));
                LOG_INFO_CLI(std::format(Wcs::c_fmtInvalidFn, Text::Wcs::daNet(statusResult.m_invalidFn)));
                LOG_INFO_CLI(std::format(Wcs::c_fmtFiscal, Text::Wcs::daNet(statusResult.m_fiscal)));
                LOG_INFO_CLI(
                    std::format(
                        Wcs::c_fmtCashDrawerOpened,
                        Text::Wcs::daNet(statusResult.m_cashDrawerOpened)
                        )
                    );
                LOG_INFO_CLI(std::format(Wcs::c_fmtCoverOpened, Text::Wcs::daNet(statusResult.m_coverOpened)));
                LOG_INFO_CLI(
                    std::format(Wcs::c_fmtReceiptPaperPresent, Text::Wcs::daNet(statusResult.m_receiptPaperPresent))
                );
                LOG_INFO_CLI(std::format(Wcs::c_fmtPaperNearEnd, Text::Wcs::daNet(statusResult.m_paperNearEnd)));
                LOG_INFO_CLI(std::format(Wcs::c_fmtCutError, Text::Wcs::daNet(statusResult.m_cutError)));
                LOG_INFO_CLI(std::format(Wcs::c_fmtPrinterOverheat, Text::Wcs::daNet(statusResult.m_printerOverheat)));
                LOG_INFO_CLI(std::format(Wcs::c_fmtReceiptLineLength, statusResult.m_receiptLineLength));
                LOG_INFO_CLI(std::format(Wcs::c_fmtReceiptLineLengthPix, statusResult.m_receiptLineLengthPix));
                LOG_INFO_CLI(
                    std::format(
                        Wcs::c_fmtShiftState,
                        wcsSafeGet(Mbs::c_shiftStateLabels, shiftStateResult.m_shiftState)
                    )
                );
                LOG_INFO_CLI(
                    std::format(
                        Wcs::c_fmtShiftExpiration,
                        DateTime::cast<std::wstring>(shiftStateResult.m_expirationDateTime)
                    )
                );
                LOG_INFO_CLI(
                    std::format(Wcs::c_fmtReceiptType, wcsSafeGet(Mbs::c_receiptTypeLabels, statusResult.m_receiptType))
                );
                LOG_INFO_CLI(
                    std::format(
                        Wcs::c_fmtDocumentType,
                        wcsSafeGet(Mbs::c_documentTypeLabels, statusResult.m_documentType)
                    )
                );
                LOG_INFO_CLI(std::format(Wcs::c_fmtCashInCount, cashStatResult.m_cashInCount));
                LOG_INFO_CLI(std::format(Wcs::c_fmtCashInSum, cashStatResult.m_cashInSum));
                LOG_INFO_CLI(std::format(Wcs::c_fmtCashOutCount, cashStatResult.m_cashOutCount));
                LOG_INFO_CLI(std::format(Wcs::c_fmtCashOutSum, cashStatResult.m_cashOutSum));
                LOG_INFO_CLI(std::format(Wcs::c_fmtSellCashSum, cashStatResult.m_sellCashSum));
                LOG_INFO_CLI(std::format(Wcs::c_fmtSellReturnCashSum, cashStatResult.m_sellReturnCashSum));
                LOG_INFO_CLI(std::format(Wcs::c_fmtCashSum, cashStatResult.m_cashSum));
                LOG_INFO_CLI(Wcs::c_ofdExchangeStatus);
                LOG_INFO_CLI(
                    std::format(
                        Wcs::c_fmtOfdExSBit0,
                        Text::Wcs::daNet(fndtOfdExchangeStatusResult.m_exchangeStatus & 0b0000'0001)
                    )
                );
                LOG_INFO_CLI(
                    std::format(
                        Wcs::c_fmtOfdExSBit1,
                        Text::Wcs::daNet(fndtOfdExchangeStatusResult.m_exchangeStatus & 0b0000'0010)
                    )
                );
                LOG_INFO_CLI(
                    std::format(
                        Wcs::c_fmtOfdExSBit2,
                        Text::Wcs::daNet(fndtOfdExchangeStatusResult.m_exchangeStatus & 0b0000'0100)
                    )
                );
                LOG_INFO_CLI(
                    std::format(
                        Wcs::c_fmtOfdExSBit3,
                        Text::Wcs::daNet(fndtOfdExchangeStatusResult.m_exchangeStatus & 0b0000'1000)
                    )
                );
                LOG_INFO_CLI(
                    std::format(
                        Wcs::c_fmtOfdExSBit4,
                        Text::Wcs::daNet(fndtOfdExchangeStatusResult.m_exchangeStatus & 0b0001'0000)
                    )
                );
                LOG_INFO_CLI(
                    std::format(
                        Wcs::c_fmtOfdExSBit5,
                        Text::Wcs::daNet(fndtOfdExchangeStatusResult.m_exchangeStatus & 0b0010'0000)
                    )
                );
                LOG_INFO_CLI(std::format(Wcs::c_fmtUnsentCount, fndtOfdExchangeStatusResult.m_unsentCount));
                LOG_INFO_CLI(std::format(Wcs::c_fmtFirstUnsentNumber, fndtOfdExchangeStatusResult.m_firstUnsentNumber));
                LOG_INFO_CLI(
                    std::format(
                        Wcs::c_fmtFirstUnsentDateTime,
                        DateTime::cast<std::wstring>(fndtOfdExchangeStatusResult.m_firstUnsentDateTime)
                    )
                );
                LOG_INFO_CLI(
                    std::format(
                        Wcs::c_fmtOkpDateTime,
                        DateTime::cast<std::wstring>(fndtOfdExchangeStatusResult.m_okpDateTime)
                    )
                );
                LOG_INFO_CLI(
                    std::format(
                        Wcs::c_fmtOfdMessageRead,
                        Text::Wcs::daNet(fndtOfdExchangeStatusResult.m_ofdMessageRead)
                    )
                );
                LOG_INFO_CLI(
                    std::format(
                        Wcs::c_fmtLastSentDateTime,
                        DateTime::cast<std::wstring>(fndtOfdExchangeStatusResult.m_lastSentDateTime)
                    )
                );
                LOG_INFO_CLI(
                    std::format(
                        Wcs::c_fmtSuccessDateTime,
                        DateTime::cast<std::wstring>(fndtErrorsResult.m_successDateTime)
                    )
                );
                LOG_INFO_CLI(std::format(Wcs::c_fmtNetworkError, fndtErrorsResult.m_networkError));
                LOG_INFO_CLI(std::format(Wcs::c_fmtNetworkErrorText, fndtErrorsResult.m_networkErrorText));
                LOG_INFO_CLI(std::format(Wcs::c_fmtOfdError, fndtErrorsResult.m_ofdError));
                LOG_INFO_CLI(std::format(Wcs::c_fmtOfdErrorText, fndtErrorsResult.m_ofdErrorText));
                LOG_INFO_CLI(std::format(Wcs::c_fmtFnError, fndtErrorsResult.m_fnError));
                LOG_INFO_CLI(std::format(Wcs::c_fmtFnErrorText, fndtErrorsResult.m_fnErrorText));
                LOG_INFO_CLI(std::format(Wcs::c_fmtDocumentNumber, fndtErrorsResult.m_documentNumber));
                LOG_INFO_CLI(std::format(Wcs::c_fmtCommandCode, fndtErrorsResult.m_commandCode));
                LOG_INFO_CLI(
                    std::format(Wcs::c_fmtDataForSendIsEmpty, Text::Wcs::daNet(fndtErrorsResult.m_dataForSendIsEmpty))
                );
                LOG_INFO_CLI(
                    std::format(
                        Wcs::c_fmtDeviceFfdVersion,
                        wcsSafeGet(Mbs::c_ffdVersions, ffdVersionResult.m_deviceFfdVersion)
                    )
                );
                LOG_INFO_CLI(
                    std::format(
                        Wcs::c_fmtDevMinFfdVersion,
                        wcsSafeGet(Mbs::c_ffdVersions, ffdVersionResult.m_devMinFfdVersion)
                    )
                );
                LOG_INFO_CLI(
                    std::format(
                        Wcs::c_fmtDevMaxFfdVersion,
                        wcsSafeGet(Mbs::c_ffdVersions, ffdVersionResult.m_devMaxFfdVersion)
                    )
                );
                LOG_INFO_CLI(
                    std::format(Wcs::c_fmtFnFfdVersion, wcsSafeGet(Mbs::c_ffdVersions, ffdVersionResult.m_fnFfdVersion))
                );
                LOG_INFO_CLI(
                    std::format(
                        Wcs::c_fmtFnMaxFfdVersion,
                        wcsSafeGet(Mbs::c_ffdVersions, ffdVersionResult.m_fnMaxFfdVersion)
                    )
                );
                LOG_INFO_CLI(
                    std::format(Wcs::c_fmtFfdVersion, wcsSafeGet(Mbs::c_ffdVersions, ffdVersionResult.m_ffdVersion))
                );
                LOG_INFO_CLI(std::format(Wcs::c_fmtFirmwareVersion, fwVersionResult.m_firmwareVersion));
                LOG_INFO_CLI(std::format(Wcs::c_fmtConfigurationVersion, fwVersionResult.m_configurationVersion));
                LOG_INFO_CLI(std::format(Wcs::c_fmtReleaseVersion, fwVersionResult.m_releaseVersion));
                LOG_INFO_CLI(std::format(Wcs::c_fmtTemplatesVersion, fwVersionResult.m_templatesVersion));
                LOG_INFO_CLI(std::format(Wcs::c_fmtControlUnitVersion, fwVersionResult.m_controlUnitVersion));
                LOG_INFO_CLI(std::format(Wcs::c_fmtBootVersion, fwVersionResult.m_bootVersion));
            }
        } else if (operation == L"demo-print") {
            callMethod(Device { KnownConnParams { serial } }, &Device::printDemo);
        } else if (operation == L"ofd-test") {
            callMethod(Device { KnownConnParams { serial } }, &Device::printOfdTest);
        } else if (operation == L"shift-reports") {
            callMethod(Device { KnownConnParams { serial } }, &Device::printCloseShiftReports);
        } else if (operation == L"last-document") {
            callMethod(Device { KnownConnParams { serial } }, &Device::printLastDocument);
        } else if (operation == L"report-x") {
            callMethod(
                Device { KnownConnParams { serial } },
                &Device::reportX,
                { s_cliOperatorName, s_cliOperatorInn, false, false }
            );
        } else if (operation == L"close-shift") {
            callMethod(
                Device { KnownConnParams { serial } },
                &Device::closeShift,
                { s_cliOperatorName, s_cliOperatorInn, true, false }
            );
        } else if (operation == L"reset-state") {
            callMethod(
                Device { KnownConnParams { serial } },
                &Device::resetState,
                { s_cliOperatorName, s_cliOperatorInn, true, true }
            );
        } else {
            return std::nullopt;
        }

        return EXIT_SUCCESS;
    }
}
