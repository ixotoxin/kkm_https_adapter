// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "macro.h"
#include "types.h"
#include <lib/meta.h>
#include <string_view>
#include <array>

namespace Kkm {
    namespace Wcs {
        KKM_WSTR(c_requiresProperty, L"Требуется корректное свойство '{}'");
        KKM_WSTR(c_requiresProperty2, L"Требуется корректное свойство '{}.{}'");

        KKM_WSTR(c_invalidConnParams, L"Ошибка параметров подключения");
        KKM_WSTR(c_alreadyInitialized, L"Параметров подключения уже инициализированы");
        KKM_WSTR(c_notImplemented, L"Не реализовано");
        KKM_WSTR(c_notAvailable, L"Недоступна");
        KKM_WSTR(c_invalidData, L"Ошибка данных");
        KKM_WSTR(c_exchangeError, L"Ошибка обмена с ККМ");
        KKM_WSTR(c_serialNumberMismatch, L"Ошибка подключения (ожидаемый СН: {}, фактический СН: {})");

        KKM_WSTR(c_fault, L"{}ККМ [{}]: Ошибка: {}");
        KKM_WSTR(c_wrongLength, L"{}ККМ [{}]: Не удалось получить ширину ленты, будет использовано значение по-умолчанию");

        KKM_WSTR(c_saved, L"ККМ [{}]: Параметры подключения успешно сохранены");
        KKM_WSTR(c_savingError, L"ККМ [{}]: Не удалось сохранить параметры подключения");
        KKM_WSTR(c_loaded, L"ККМ [{}]: Параметры подключения успешно загружены");
        KKM_WSTR(c_loadingError, L"ККМ [{}]: Не удалось загрузить параметры подключения");
        KKM_WSTR(c_cancelingError, L"{}ККМ [{}]: Ошибка отмены чека: {}");
        KKM_WSTR(c_closingError, L"{}ККМ [{}]: Ошибка закрытия документа: {}");
        KKM_WSTR(c_printingError, L"{}ККМ [{}]: Ошибка печати документа: {}");
        KKM_WSTR(c_checkingError, L"Не удалось проверить закрытие документа");
        KKM_WSTR(c_shiftMismatch, L"{}ККМ [{}]: Номер смены в ККМ не совпадает с номером смены в ФН");

        KKM_WSTR(c_requiresItems, L"Не предоставлено ни одной позиции");
        KKM_WSTR(c_badOperator, L"Данные оператора некорректны");
        KKM_WSTR(c_badCustomer, L"Данные покупателя некорректны");

        KKM_WSTR(c_typeSell, L"ПРИХОД");
        KKM_WSTR(c_typeSellReturn, L"ВОЗВРАТ ПРИХОДА");

        KKM_WSTR(c_subSetOperator, L"{}ККМ [{}]: Операция: Регистрация оператора");
        KKM_WSTR(c_subSetCustomer, L"{}ККМ [{}]: Операция: Регистрация покупателя");
        KKM_WSTR(c_subSetSeller, L"{}ККМ [{}]: Операция: Регистрация продавца");
        KKM_WSTR(c_subRegisterCashInAndPrint, L"{}ККМ [{}]: Операция: Регистрация внесения и печать");
        KKM_WSTR(c_subCashOut, L"{}ККМ [{}]: Операция: Выплата");
        KKM_WSTR(c_subCashOutNoNeed, L"{}ККМ [{}]: Операция: Выплата: Не требуется");
        KKM_WSTR(c_subRegisterCashOutAndPrint, L"{}ККМ [{}]: Операция: Регистрация выплаты и печать");
        KKM_WSTR(c_subRegisterItems, L"{}ККМ [{}]: Операция: Регистрация позиций (товаров/услуг)");
        KKM_WSTR(c_subRegisterPayment, L"{}ККМ [{}]: Операция: Регистрация оплаты");
        KKM_WSTR(c_subRegisterReceiptAndPrint, L"{}ККМ [{}]: Операция: Регистрация чека и печать");
        KKM_WSTR(c_subCloseShift, L"{}ККМ [{}]: Операция: Закрытие смены");
        KKM_WSTR(c_subCloseShiftNoNeed, L"{}ККМ [{}]: Операция: Закрытие смены: Не требуется");
        KKM_WSTR(c_subCancelReceipt, L"{}ККМ [{}]: Операция: Отмена открытого чека");
        KKM_WSTR(c_subCancelReceiptNoNeed, L"{}ККМ [{}]: Операция: Отмена открытого чека: Не требуется");
        KKM_WSTR(c_subPrint, L"{}ККМ [{}]: Операция: Печать");

        KKM_WSTR(c_statusMethod, L"{}ККМ [{}]: Запрос общей информации и статуса");
        KKM_WSTR(c_shiftStateMethod, L"{}ККМ [{}]: Запрос состояния смены");
        KKM_WSTR(c_receiptStateMethod, L"{}ККМ [{}]: Запрос состояния чека");
        KKM_WSTR(c_cashStatMethod, L"{}ККМ [{}]: Запрос статистики по наличным");
        KKM_WSTR(c_ofdExchangeStatusMethod, L"{}ККМ [{}]: Запрос статуса информационного обмена с ОФД");
        KKM_WSTR(c_fnInfoMethod, L"{}ККМ [{}]: Запрос информации и статуса ФН");
        KKM_WSTR(c_registrationInfoMethod, L"{}ККМ [{}]: Запрос информации о регистрации");
        KKM_WSTR(c_lastRegistrationMethod, L"{}ККМ [{}]: Запрос информации о последней (пере)регистрации");
        KKM_WSTR(c_lastReceiptMethod, L"{}ККМ [{}]: Запрос информации о последнем чеке");
        KKM_WSTR(c_lastDocumentMethod, L"{}ККМ [{}]: Запрос информации о последнем фискальном документе");
        KKM_WSTR(c_errorsMethod, L"{}ККМ [{}]: Запрос информации об ошибках обмена с ОФД");
        KKM_WSTR(c_ffdVersionMethod, L"{}ККМ [{}]: Запрос версий ФФД");
        KKM_WSTR(c_fwVersionMethod, L"{}ККМ [{}]: Запрос версий ПО");
        KKM_WSTR(c_printDemoMethod, L"{}ККМ [{}]: Демо-печать");
        KKM_WSTR(c_printHelloMethod, L"{}ККМ [{}]: Печать приветствия");
        KKM_WSTR(c_printNfDocumentMethod, L"{}ККМ [{}]: Печать не фискального документа");
        KKM_WSTR(c_printInfoMethod, L"{}ККМ [{}]: Печать информации о ККТ");
        KKM_WSTR(c_printFnRegistrationsMethod, L"{}ККМ [{}]: Печать итогов регистрации / перерегистрации");
        KKM_WSTR(c_printOfdExchangeStatusMethod, L"{}ККМ [{}]: Печать отчета о состоянии расчетов");
        KKM_WSTR(c_printOfdTestMethod, L"{}ККМ [{}]: Тестирование подключения к ОФД");
        KKM_WSTR(c_printShiftReportsMethod, L"{}ККМ [{}]: Печать нераспечатанных отчетов о закрытии смены");
        KKM_WSTR(c_printLastDocumentMethod, L"{}ККМ [{}]: Печать копии последнего документа");
        KKM_WSTR(c_cashInMethod, L"{}ККМ [{}]: Регистрация внесения");
        KKM_WSTR(c_cashOutMethod, L"{}ККМ [{}]: Регистрация выплаты");
        KKM_WSTR(c_sellMethod, L"{}ККМ [{}]: Регистрация чека прихода (продажи)");
        KKM_WSTR(c_sellReturnMethod, L"{}ККМ [{}]: Регистрация чека возврата прихода (продажи)");
        KKM_WSTR(c_reportXMethod, L"{}ККМ [{}]: Печать X-отчета");
        KKM_WSTR(c_closeShiftMethod, L"{}ККМ [{}]: Закрытие смены");
        KKM_WSTR(c_resetStateMethod, L"{}ККМ [{}]: Возврат к исходному состоянию");

        KKM_WSTR(c_helloTitle, L"Привет!");
        KKM_WSTR(c_helloText, L"\nЯ трудолюбивая касса.\n\nЯ очень рада работать в компании таких замечательных людей.");

        inline const std::array<std::wstring, 11> c_allowedBaudRate {
            std::to_wstring(Atol::LIBFPTR_PORT_BR_1200),
            std::to_wstring(Atol::LIBFPTR_PORT_BR_2400),
            std::to_wstring(Atol::LIBFPTR_PORT_BR_4800),
            std::to_wstring(Atol::LIBFPTR_PORT_BR_9600),
            std::to_wstring(Atol::LIBFPTR_PORT_BR_19200),
            std::to_wstring(Atol::LIBFPTR_PORT_BR_38400),
            std::to_wstring(Atol::LIBFPTR_PORT_BR_57600),
            std::to_wstring(Atol::LIBFPTR_PORT_BR_115200),
            std::to_wstring(Atol::LIBFPTR_PORT_BR_230400),
            std::to_wstring(Atol::LIBFPTR_PORT_BR_460800),
            std::to_wstring(Atol::LIBFPTR_PORT_BR_921600)
        };
    }

    namespace Mbs {
        KKM_STR(c_requiresProperty, "Требуется корректное свойство '{}'");
        KKM_STR(c_requiresProperty2, "Требуется корректное свойство '{}.{}'");

        KKM_STR(c_closedShift, "Сессия закрыта");
        KKM_STR(c_openedShift, "Сессия открыта");
        KKM_STR(c_expiredShift, "Сессия истекла");
        KKM_STR(c_closedReceipt, "Чек закрыт");
        KKM_STR(c_sellReceipt, "Чек прихода");
        KKM_STR(c_sellReturnReceipt, "Чек возврата прихода");
        KKM_STR(c_sellCorrectionReceipt, "Чек коррекции прихода");
        KKM_STR(c_sellReturnCorrectionReceipt, "Чек коррекции возврата прихода");
        KKM_STR(c_buyReceipt, "Чек расхода");
        KKM_STR(c_buyReturnReceipt, "Чек возврата расхода");
        KKM_STR(c_buyCorrectionReceipt, "Чек коррекции расхода");
        KKM_STR(c_buyReturnCorrectionReceipt, "Чек коррекции возврата расхода");
        KKM_STR(c_closedDocument, "Документ закрыт");
        KKM_STR(c_openShiftDocument, "Документ открытия смены");
        KKM_STR(c_closeShiftDocument, "Документ закрытия смены");
        KKM_STR(c_registrationDocument, "Документ пере/регистрации");
        KKM_STR(c_closeArchiveDocument, "Документ закрытия архива ФН");
        KKM_STR(c_ofdExchangeStatusDocument, "Отчёт о состоянии расчётов");
        KKM_STR(c_serviceDocument, "Сервисный документ");
        KKM_STR(c_documentCopy, "Копия документа");

        inline const std::unordered_map<unsigned int, std::string_view> c_models {
            { Atol::LIBFPTR_MODEL_ALLIANCE_20F, "АЛЬЯНС 20Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_1F, "АТОЛ 1Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_2F, "АТОЛ 2Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_11F, "АТОЛ 11Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_15F, "АТОЛ 15Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_20F, "АТОЛ 20Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_22F, "АТОЛ 22Ф (АТОЛ FPrint-22ПТК)" },
            { Atol::LIBFPTR_MODEL_ATOL_22V2F, "АТОЛ 22 v2 Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_25F, "АТОЛ 25Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_27F, "АТОЛ 27Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_27_FP7_F, "АТОЛ 27 FP7 Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_30F, "АТОЛ 30Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_35F, "АТОЛ 35Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_42FA, "АТОЛ 42ФА" },
            { Atol::LIBFPTR_MODEL_ATOL_42FS, "АТОЛ 42ФС" },
            { Atol::LIBFPTR_MODEL_ATOL_47FA, "АТОЛ 47ФА" },
            { Atol::LIBFPTR_MODEL_ATOL_50F, "АТОЛ 50Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_52F, "АТОЛ 52Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_55F, "АТОЛ 55Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_55V2F, "АТОЛ 55 v2 Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_60F, "АТОЛ 60Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_77F, "АТОЛ 77Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_90F, "АТОЛ 90Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_91F, "АТОЛ 91Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_92F, "АТОЛ 92Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_PT_5F, "АТОЛ PT-5Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_SIGMA_10, "АТОЛ Sigma 10" },
            { Atol::LIBFPTR_MODEL_ATOL_SIGMA_7F, "АТОЛ Sigma 7Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_SIGMA_8F, "АТОЛ Sigma 8Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_STB_6F, "АТОЛ СТБ 6Ф" },
            { Atol::LIBFPTR_MODEL_KAZNACHEY_FA, "Казначей ФА" }
        };

        inline const std::unordered_map<std::string, TimeZone> c_timeZoneMap {
            { "device", TimeZone::Device },
            { "tz" + std::to_string(Meta::toUnderlying(TimeZone::Device)), TimeZone::Device },
            { "+2", TimeZone::Zone1 },
            { "utc+2", TimeZone::Zone1 },
            { "tz" + std::to_string(Meta::toUnderlying(TimeZone::Zone1)), TimeZone::Zone1 },
            { "+3", TimeZone::Zone2 },
            { "utc+3", TimeZone::Zone2 },
            { "tz" + std::to_string(Meta::toUnderlying(TimeZone::Zone2)), TimeZone::Zone2 },
            { "+4", TimeZone::Zone3 },
            { "utc+4", TimeZone::Zone3 },
            { "tz" + std::to_string(Meta::toUnderlying(TimeZone::Zone3)), TimeZone::Zone3 },
            { "+5", TimeZone::Zone4 },
            { "utc+5", TimeZone::Zone4 },
            { "tz" + std::to_string(Meta::toUnderlying(TimeZone::Zone4)), TimeZone::Zone4 },
            { "+6", TimeZone::Zone5 },
            { "utc+6", TimeZone::Zone5 },
            { "tz" + std::to_string(Meta::toUnderlying(TimeZone::Zone5)), TimeZone::Zone5 },
            { "+7", TimeZone::Zone6 },
            { "utc+7", TimeZone::Zone6 },
            { "tz" + std::to_string(Meta::toUnderlying(TimeZone::Zone6)), TimeZone::Zone6 },
            { "+8", TimeZone::Zone7 },
            { "utc+8", TimeZone::Zone7 },
            { "tz" + std::to_string(Meta::toUnderlying(TimeZone::Zone7)), TimeZone::Zone7 },
            { "+9", TimeZone::Zone8 },
            { "utc+9", TimeZone::Zone8 },
            { "tz" + std::to_string(Meta::toUnderlying(TimeZone::Zone8)), TimeZone::Zone8 },
            { "+10", TimeZone::Zone9 },
            { "utc+10", TimeZone::Zone9 },
            { "tz" + std::to_string(Meta::toUnderlying(TimeZone::Zone9)), TimeZone::Zone9 },
            { "+11", TimeZone::Zone10 },
            { "utc+11", TimeZone::Zone10 },
            { "tz" + std::to_string(Meta::toUnderlying(TimeZone::Zone10)), TimeZone::Zone10 },
            { "+12", TimeZone::Zone11 },
            { "utc+12", TimeZone::Zone11 },
            { "tz" + std::to_string(Meta::toUnderlying(TimeZone::Zone11)), TimeZone::Zone11 }
        };

        inline const std::unordered_map<FfdVersion, std::string_view> c_ffdVersions {
            { FfdVersion::Unknown, "[неизвестная версия]" },
            { FfdVersion::V_1_0_5, "1.0.5" },
            { FfdVersion::V_1_1, "1.1" },
            { FfdVersion::V_1_2, "1.2" }
        };

        inline const std::unordered_map<ShiftState, std::string_view> c_shiftStateLabels {
            { ShiftState::Closed, Mbs::c_closedShift },
            { ShiftState::Opened, Mbs::c_openedShift },
            { ShiftState::Expired, Mbs::c_expiredShift },
        };

        inline const std::unordered_map<ReceiptType, std::string_view> c_receiptTypeLabels {
            { ReceiptType::Closed, Mbs::c_closedReceipt },
            { ReceiptType::Sell, Mbs::c_sellReceipt },
            { ReceiptType::SellReturn, Mbs::c_sellReturnReceipt },
            { ReceiptType::SellCorrection, Mbs::c_sellCorrectionReceipt },
            { ReceiptType::SellReturnCorrection, Mbs::c_sellReturnCorrectionReceipt },
            { ReceiptType::Buy, Mbs::c_buyReceipt },
            { ReceiptType::BuyReturn, Mbs::c_buyReturnReceipt },
            { ReceiptType::BuyCorrection, Mbs::c_buyCorrectionReceipt },
            { ReceiptType::BuyReturnCorrection, Mbs::c_buyReturnCorrectionReceipt },
        };

        inline const std::unordered_map<DocumentType, std::string_view> c_documentTypeLabels {
            { DocumentType::Closed, Mbs::c_closedDocument },
            { DocumentType::ReceiptSell, Mbs::c_sellReceipt },
            { DocumentType::ReceiptSellReturn, Mbs::c_sellReturnReceipt },
            { DocumentType::ReceiptSellCorrection, Mbs::c_sellCorrectionReceipt },
            { DocumentType::ReceiptSellReturnCorrection, Mbs::c_sellReturnCorrectionReceipt },
            { DocumentType::ReceiptBuy, Mbs::c_buyReceipt },
            { DocumentType::ReceiptBuyReturn, Mbs::c_buyReturnReceipt },
            { DocumentType::ReceiptBuyCorrection, Mbs::c_buyCorrectionReceipt },
            { DocumentType::ReceiptBuyReturnCorrection, Mbs::c_buyReturnCorrectionReceipt },
            { DocumentType::OpenShift, Mbs::c_openShiftDocument },
            { DocumentType::CloseShift, Mbs::c_closeShiftDocument },
            { DocumentType::Registration, Mbs::c_registrationDocument },
            { DocumentType::CloseArchive, Mbs::c_closeArchiveDocument },
            { DocumentType::OfdExchangeStatus, Mbs::c_ofdExchangeStatusDocument },
            { DocumentType::DocumentService, Mbs::c_serviceDocument },
            { DocumentType::DocumentCopy, Mbs::c_documentCopy },
        };

        inline const std::unordered_map<std::string, MeasurementUnit> c_measurementUnitMap {
            { "piece", MeasurementUnit::Piece },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::Piece)), MeasurementUnit::Piece },
            { "gram", MeasurementUnit::Gram },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::Gram)), MeasurementUnit::Gram },
            { "kilogram", MeasurementUnit::Kilogram },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::Kilogram)), MeasurementUnit::Kilogram },
            { "ton", MeasurementUnit::Ton },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::Ton)), MeasurementUnit::Ton },
            { "centimeter", MeasurementUnit::Centimeter },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::Centimeter)), MeasurementUnit::Centimeter },
            { "decimeter", MeasurementUnit::Decimeter },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::Decimeter)), MeasurementUnit::Decimeter },
            { "meter", MeasurementUnit::Meter },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::Meter)), MeasurementUnit::Meter },
            { "squarecentimeter", MeasurementUnit::SquareCentimeter },
            { "square_centimeter", MeasurementUnit::SquareCentimeter },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::SquareCentimeter)), MeasurementUnit::SquareCentimeter },
            { "squaredecimeter", MeasurementUnit::SquareDecimeter },
            { "square_decimeter", MeasurementUnit::SquareDecimeter },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::SquareDecimeter)), MeasurementUnit::SquareDecimeter },
            { "squaremeter", MeasurementUnit::SquareMeter },
            { "square_meter", MeasurementUnit::SquareMeter },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::SquareMeter)), MeasurementUnit::SquareMeter },
            { "milliliter", MeasurementUnit::Milliliter },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::Milliliter)), MeasurementUnit::Milliliter },
            { "liter", MeasurementUnit::Liter },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::Liter)), MeasurementUnit::Liter },
            { "cubicmeter", MeasurementUnit::CubicMeter },
            { "cubic_meter", MeasurementUnit::CubicMeter },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::CubicMeter)), MeasurementUnit::CubicMeter },
            { "kilowatthour", MeasurementUnit::KilowattHour },
            { "kilowatt_hour", MeasurementUnit::KilowattHour },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::KilowattHour)), MeasurementUnit::KilowattHour },
            { "gkal", MeasurementUnit::Gkal },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::Gkal)), MeasurementUnit::Gkal },
            { "day", MeasurementUnit::Day },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::Day)), MeasurementUnit::Day },
            { "hour", MeasurementUnit::Hour },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::Hour)), MeasurementUnit::Hour },
            { "minute", MeasurementUnit::Minute },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::Minute)), MeasurementUnit::Minute },
            { "second", MeasurementUnit::Second },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::Second)), MeasurementUnit::Second },
            { "kilobyte", MeasurementUnit::Kilobyte },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::Kilobyte)), MeasurementUnit::Kilobyte },
            { "megabyte", MeasurementUnit::Megabyte },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::Megabyte)), MeasurementUnit::Megabyte },
            { "gigabyte", MeasurementUnit::Gigabyte },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::Gigabyte)), MeasurementUnit::Gigabyte },
            { "terabyte", MeasurementUnit::Terabyte },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::Terabyte)), MeasurementUnit::Terabyte },
            { "other", MeasurementUnit::Other },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::Other)), MeasurementUnit::Other },
        };

        inline const std::unordered_map<std::string, Tax> c_taxCastMap {
            { "no", Tax::No },
            { std::to_string(Meta::toUnderlying(Tax::No)), Tax::No },
            { "vat0", Tax::Vat0 },
            { std::to_string(Meta::toUnderlying(Tax::Vat0)), Tax::Vat0 },
            { "vat5", Tax::Vat5 },
            { std::to_string(Meta::toUnderlying(Tax::Vat5)), Tax::Vat5 },
            { "vat105", Tax::Vat105 },
            { std::to_string(Meta::toUnderlying(Tax::Vat105)), Tax::Vat105 },
            { "vat7", Tax::Vat7 },
            { std::to_string(Meta::toUnderlying(Tax::Vat7)), Tax::Vat7 },
            { "vat107", Tax::Vat107 },
            { std::to_string(Meta::toUnderlying(Tax::Vat107)), Tax::Vat107 },
            { "vat10", Tax::Vat10 },
            { std::to_string(Meta::toUnderlying(Tax::Vat10)), Tax::Vat10 },
            { "vat110", Tax::Vat110 },
            { std::to_string(Meta::toUnderlying(Tax::Vat110)), Tax::Vat110 },
            { "vat20", Tax::Vat20 },
            { std::to_string(Meta::toUnderlying(Tax::Vat20)), Tax::Vat20 },
            { "vat120", Tax::Vat120 },
            { std::to_string(Meta::toUnderlying(Tax::Vat120)), Tax::Vat120 }
        };

        inline const std::unordered_map<std::string, PaymentType> c_paymentTypeCastMap {
            { "cash", PaymentType::Cash },
            { std::to_string(Meta::toUnderlying(PaymentType::Cash)), PaymentType::Cash },
            { "electronically", PaymentType::Electronically },
            { std::to_string(Meta::toUnderlying(PaymentType::Electronically)), PaymentType::Electronically }
        };
    }
}
