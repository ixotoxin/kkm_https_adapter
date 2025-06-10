// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <string>

namespace Basic {
    namespace Wcs {
        using S = std::wstring_view;

        constexpr S c_commandPrefix { L"ПП [{}]: " };
        constexpr S c_prefixedText { L"ПП [{}]: {}" };
        constexpr S c_connParamsSaved { L"ПП [{}]: Параметры подключения ККМ [{}] успешно сохранены" };
        constexpr S c_getKkmInfo { L"ПП [{}]: ККМ [{}]: Получение информации об устройстве" };
        constexpr S c_requiresProperty { L"Требуется корректное свойство '{}'" };
    }

    namespace Mbs {
        using S = std::string_view;

        constexpr S c_requiresProperty { "Требуется корректное свойство '{}'" };
    }
}

namespace Config::Wcs {
    using S = std::wstring_view;

    constexpr S c_cantReadConfig { L"Не удалось прочитать конфигурационный файл '{}'" };
    constexpr S c_cantParseConfig { L"Конфигурационный файл '{}' содержит ошибки" };
    constexpr S c_invalidConfig { L"Ошибка чтения конфигурации" };
}

namespace Service::Wcs {
    using S = std::wstring_view;

    constexpr S c_starting { L"Запуск службы..." };
    constexpr S c_startingFailed { L"Не удалось запустить службу" };
    constexpr S c_startingTimeout { L"Истекло время ожидания запуска службы" };
    constexpr S c_alreadyStarted { L"Служба уже запущена" };
    constexpr S c_started { L"Служба запущена" };
    constexpr S c_stopping { L"Остановка службы..." };
    constexpr S c_stoppingFailed { L"Не удалось остановить службу" };
    constexpr S c_stoppingTimeout { L"Истекло время ожидания остановки службы" };
    constexpr S c_alreadyStopped { L"Служба уже остановлена" };
    constexpr S c_stopped { L"Служба остановлена" };
    constexpr S c_installed { L"Служба установлена" };
    constexpr S c_uninstalled { L"Служба удалена" };
}

namespace Server::Wcs {
    using S = std::wstring_view;

    // constexpr S c_osslProviderNotLoaded { L"Не загружен {}-провайдер библиотеки OpenSSL" };
    // constexpr S c_osslInitializationFailed { L"Не удалось инициализировать библиотеку OpenSSL" };

    constexpr S c_starting { L"Запуск сервера..." };
    constexpr S c_startingFailed { L"Не удалось запустить сервер" };
    constexpr S c_started { L"Сервер запущен" };
    constexpr S c_stopping { L"Остановка сервера..." };
    constexpr S c_stoppingFailed { L"Не удалось остановить сервер" };
    constexpr S c_stopped { L"Сервер остановлен" };
    constexpr S c_servicingFailed { L"Сбой сервера" };
    constexpr S c_processingSuccess { L"Запрос успешно обработан" };
    constexpr S c_processingFailed { L"Не удалось обработать запрос" };
    constexpr S c_maximumIsExceeded { L"Превышено количество одновременных запросов" };
}

namespace Http {
    namespace Wcs {
        using S = std::wstring_view;

        constexpr S c_requestPrefix { L"Запрос [{:04x}]: " };
        constexpr S c_prefixedText { L"Запрос [{:04x}]: {}" };
        constexpr S c_forbidden { L"Запрос [{:04x}]: Сбой авторизации" };
        constexpr S c_fromCache { L"Запрос [{:04x}]: Данные взяты из кеша" };
        constexpr S c_redirectToStatic { L"Запрос [{:04x}]: Перенаправляем в /static/" };
        constexpr S c_bodySizeLimitExceeded { L"Запрос [{:04x}]: Превышен допустимый размер тела запроса" };
        constexpr S c_cacheMaintain { L"Обслуживание кэша (размер {} => {})" };
    }

    namespace Mbs {
        using S = std::string_view;

        constexpr S c_requestPrefix { "Запрос [{:04x}]: " };
        constexpr S c_prefixedText { "Запрос [{:04x}]: {}" };
        constexpr S c_badRequest { "Некорректный запрос" };
        constexpr S c_methodNotAllowed { "Невозможно выполнить данный запрос" };
        constexpr S c_unknownMethod { "Неизвестный метод" };
        constexpr S c_notImplemented { "Не реализовано" };
        constexpr S c_forbidden { "Сбой авторизации" };
        constexpr S c_badPath { "Некорректный путь" };
        constexpr S c_notFound { "Документ не найден" };
        constexpr S c_bodySizeLimitExceeded { "Превышен допустимый размер тела запроса" };
        constexpr S c_invalidContentType { "Заголовок 'Content-Type' отсутствует или некорректен" };
        constexpr S c_invalidXSecret { "Заголовок 'X-Secret' отсутствует или некорректен" };
        constexpr S c_invalidXIdempotencyKey { "Заголовок 'X-Idempotency-Key' отсутствует или некорректен" };
        constexpr S c_requestedMethod { "От клиента [{}] поступил запрос [{} {}]" };
        constexpr S c_badFilesystemEntityType { "Неподходящий тип объекта файловой системы" };
        constexpr S c_fileTooLarge { "Размер файла превышает разрешённый" };
        constexpr S c_unknownMimeType { "Неизвестный тип файла" };
    }
}

namespace Kkm {
    namespace Wcs {
        using S = std::wstring_view;
        using P = std::wstring_view; // const wchar_t *;

        using Basic::Wcs::c_requiresProperty;
        using Http::Wcs::c_requestPrefix;
        using Http::Wcs::c_prefixedText;

        constexpr S c_fault { L"{}ККМ [{}]: Ошибка: {}" };
        constexpr S c_invalidConnParams { L"Ошибка параметров подключения" };
        constexpr S c_alreadyInitialized { L"Параметров подключения уже инициализированы" };
        constexpr S c_notImplemented { L"Не реализовано" };
        constexpr S c_notAvailable { L"Недоступна" };
        constexpr S c_invalidData { L"Ошибка данных" };
        constexpr S c_exchangeError { L"Ошибка обмена с ККМ" };
        constexpr S c_serialNumberMismatch { L"Ошибка подключения (ожидаемый СН: {}, фактический СН: {})" };

        constexpr S c_wrongLength {
            L"{}ККМ [{}]: Не удалось получить ширину ленты, будет использовано значение по-умолчанию"
        };

        constexpr S c_selectKkm { L"Запрос [{:04x}]: Выбрана ККМ [{}] (параметры подключения: {})" };
        constexpr S c_getKkmInfo { L"Запрос [{:04x}]: ККМ [{}]: Получение информации об устройстве" };
        constexpr S c_connParamsSaved { L"Запрос [{:04x}]: Параметры подключения ККМ [{}] успешно сохранены" };

        constexpr S c_saved { L"ККМ [{}]: Параметры подключения успешно сохранены" };
        constexpr S c_cantSave { L"ККМ [{}]: Не удалось сохранить параметры подключения" };
        constexpr S c_loaded { L"ККМ [{}]: Параметры подключения успешно загружены" };
        constexpr S c_loadingError { L"ККМ [{}]: Не удалось загрузить параметры подключения" }; // c_cantLoad
        constexpr S c_cancelingError { L"{}ККМ [{}]: Ошибка отмены чека: {}" }; // c_cantCancel
        constexpr S c_closingError { L"{}ККМ [{}]: Ошибка закрытия документа: {}" };
        constexpr S c_printingError { L"{}ККМ [{}]: Ошибка печати документа: {}" };
        constexpr S c_checkingError { L"Не удалось проверить закрытие документа" };
        constexpr S c_shiftMismatch { L"{}ККМ [{}]: Номер смены в ККМ не совпадает с номером смены в ФН" };

        constexpr S c_requiresItems { L"Не предоставлено ни одной позиции" };
        constexpr S c_badOperator { L"Данные оператора некорректны" };
        constexpr S c_badCustomer { L"Данные покупателя некорректны" };

        constexpr S c_typeSell { L"ПРИХОД" };
        constexpr S c_typeSellReturn { L"ВОЗВРАТ ПРИХОДА" };

        constexpr S c_subSetOperator { L"{}ККМ [{}]: Операция: Регистрация оператора" };
        constexpr S c_subSetCustomer { L"{}ККМ [{}]: Операция: Регистрация покупателя" };
        constexpr S c_subRegisterCashInAndPrint { L"{}ККМ [{}]: Операция: Регистрация внесения и печать" };
        constexpr S c_subCashOut { L"{}ККМ [{}]: Операция: Выплата" };
        constexpr S c_subCashOutNoNeed { L"{}ККМ [{}]: Операция: Выплата: Не требуется" };
        constexpr S c_subRegisterCashOutAndPrint { L"{}ККМ [{}]: Операция: Регистрация выплаты и печать" };
        constexpr S c_subRegisterItems { L"{}ККМ [{}]: Операция: Регистрация позиций (товаров/услуг)" };
        constexpr S c_subRegisterPayment { L"{}ККМ [{}]: Операция: Регистрация оплаты" };
        constexpr S c_subRegisterReceiptAndPrint { L"{}ККМ [{}]: Операция: Регистрация чека и печать" };
        constexpr S c_subCloseShift { L"{}ККМ [{}]: Операция: Закрытие смены" };
        constexpr S c_subCloseShiftNoNeed { L"{}ККМ [{}]: Операция: Закрытие смены: Не требуется" };
        constexpr S c_subCancelReceipt { L"{}ККМ [{}]: Операция: Отмена открытого чека" };
        constexpr S c_subCancelReceiptNoNeed { L"{}ККМ [{}]: Операция: Отмена открытого чека: Не требуется" };
        constexpr S c_subPrint { L"{}ККМ [{}]: Операция: Печать" };

        constexpr S c_statusMethod { L"{}ККМ [{}]: Запрос общей информации и статуса" };
        constexpr S c_shiftStateMethod { L"{}ККМ [{}]: Запрос состояния смены" };
        constexpr S c_receiptStateMethod { L"{}ККМ [{}]: Запрос состояния чека" };
        constexpr S c_cashStatMethod { L"{}ККМ [{}]: Запрос статистики по наличным" };
        constexpr S c_ofdExchangeStatusMethod { L"{}ККМ [{}]: Запрос статуса информационного обмена с ОФД" };
        constexpr S c_fnInfoMethod { L"{}ККМ [{}]: Запрос информации и статуса ФН" };
        constexpr S c_lastRegistrationMethod { L"{}ККМ [{}]: Запрос информации о последней (пере)регистрации" };
        constexpr S c_lastReceiptMethod { L"{}ККМ [{}]: Запрос информации о последнем чеке" };
        constexpr S c_lastDocumentMethod { L"{}ККМ [{}]: Запрос информации о последнем фискальном документе" };
        constexpr S c_errorsMethod { L"{}ККМ [{}]: Запрос информации об ошибках обмена с ОФД" };
        constexpr S c_versionMethod { L"{}ККМ [{}]: Запрос версий ПО" };
        constexpr S c_printDemoMethod { L"{}ККМ [{}]: Демо-печать" };
        constexpr S c_printHelloMethod { L"{}ККМ [{}]: Печать приветствия" };
        constexpr S c_printNfDocumentMethod { L"{}ККМ [{}]: Печать не фискального документа" };
        constexpr S c_printInfoMethod { L"{}ККМ [{}]: Печать информации о ККТ" };
        constexpr S c_printFnRegistrationsMethod { L"{}ККМ [{}]: Печать итогов регистрации / перерегистрации" };
        constexpr S c_printOfdExchangeStatusMethod { L"{}ККМ [{}]: Печать отчета о состоянии расчетов" };
        constexpr S c_printOfdTestMethod { L"{}ККМ [{}]: Тестирование подключения к ОФД" };
        constexpr S c_printShiftReportsMethod { L"{}ККМ [{}]: Печать нераспечатанных отчетов о закрытии смены" };
        constexpr S c_printLastDocumentMethod { L"{}ККМ [{}]: Печать копии последнего документа" };
        constexpr S c_cashInMethod { L"{}ККМ [{}]: Регистрация внесения" };
        constexpr S c_cashOutMethod { L"{}ККМ [{}]: Регистрация выплаты" };
        constexpr S c_sellMethod { L"{}ККМ [{}]: Регистрация чека прихода (продажи)" };
        constexpr S c_sellReturnMethod { L"{}ККМ [{}]: Регистрация чека возврата прихода (продажи)" };
        constexpr S c_reportXMethod { L"{}ККМ [{}]: Печать X-отчета" };
        constexpr S c_closeShiftMethod { L"{}ККМ [{}]: Закрытие смены" };
        constexpr S c_resetStateMethod { L"{}ККМ [{}]: Возврат к исходному состоянию" };

        constexpr S c_helloTitle { L"Привет!" };

        constexpr S c_helloText {
            L"\nЯ трудолюбивая касса.\n\nЯ очень рада работать в компании таких замечательных людей."
        };

        constexpr P c_closedShift { L"Сессия закрыта" };
        constexpr P c_openedShift { L"Сессия открыта" };
        constexpr P c_expiredShift { L"Сессия истекла" };
        constexpr P c_closedReceipt { L"Чек закрыт" };
        constexpr P c_sellReceipt { L"Чек прихода" };
        constexpr P c_sellReturnReceipt { L"Чек возврата прихода" };
        constexpr P c_sellCorrectionReceipt { L"Чек коррекции прихода" };
        constexpr P c_sellReturnCorrectionReceipt { L"Чек коррекции возврата прихода" };
        constexpr P c_buyReceipt { L"Чек расхода" };
        constexpr P c_buyReturnReceipt { L"Чек возврата расхода" };
        constexpr P c_buyCorrectionReceipt { L"Чек коррекции расхода" };
        constexpr P c_buyReturnCorrectionReceipt { L"Чек коррекции возврата расхода" };
        constexpr P c_closedDocument { L"Документ закрыт" };
        constexpr P c_openShiftDocument { L"Документ открытия смены" };
        constexpr P c_closeShiftDocument { L"Документ закрытия смены" };
        constexpr P c_registrationDocument { L"Документ пере/регистрации" };
        constexpr P c_closeArchiveDocument { L"Документ закрытия архива ФН" };
        constexpr P c_ofdExchangeStatusDocument { L"Отчёт о состоянии расчётов" };
        constexpr P c_serviceDocument { L"Сервисный документ" };
        constexpr P c_documentCopy { L"Копия документа" };
    }

    namespace Mbs {
        using S = std::string_view;
        using Basic::Mbs::c_requiresProperty;

        constexpr S c_notFound { "Запрос [{:04x}]: ККМ [{}] не доступна" };
        constexpr S c_cantClearRegistry { "Не удалось очистить реестр параметров подключения" };
    }
}
