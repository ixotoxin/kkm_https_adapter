// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <string_view>

namespace KkmOperator::Wcs {
    using Csv = const std::wstring_view;

    constexpr Csv c_usage1 {
        L"    learn {пп} ...      Добавить ККМ\n"
        L"    status {сн}         Вывести статус ККМ\n"
        L"    demo-print {сн}     Выполнить демо-печать\n"
        L"    ofd-test {сн}       Тестировать подключение ККМ к ОФД\n"
        L"    shift-reports {сн}  Напечатать нераспечатанные отчеты о закрытии смены\n"
        L"    last-document {сн}  Напечатать копию последнего документа\n"
        L"    report-x {сн}       Печать Х-отчёта ККМ\n"
        L"    close-shift {сн}    Закрыть смену ККМ\n"
        L"    reset-state {сн}    Возврат ККМ к исходному состоянию\n"
    };

    constexpr Csv c_usage2 {
        L"    {пп}                Параметры подключения (можно указать несколько)\n"
        L"    {сн}                Серийный номер ККМ\n"
    };

    constexpr Csv c_commandPrefix { L"ПП [{}]: " };
    constexpr Csv c_prefixedText { L"ПП [{}]: {}" };
    constexpr Csv c_getKkmInfo { L"ПП [{}]: ККМ [{}]: Получение информации об устройстве" };
    constexpr Csv c_connParamsSaved { L"ПП [{}]: Параметры подключения ККМ [{}] успешно сохранены" };

    constexpr Csv c_fmtModel { L"Модель: {}" };
    constexpr Csv c_fmtSerialNumber { L"Серийный (заводской) номер: {}" };
    constexpr Csv c_fmtBlocked { L"Заблокирована из-за ошибок: {}" };
    constexpr Csv c_fmtLogicalNumber { L"Номер в магазине: {}" };
    constexpr Csv c_fmtDateTime { L"Дата и время ККМ: {}" };
    constexpr Csv c_fmtFnPresent { L"ФН установлен: {}" };
    constexpr Csv c_fmtFnFiscal { L"ФН фискализирован: {}" };
    constexpr Csv c_fmtInvalidFn { L"ФН корректен: {}" };
    constexpr Csv c_fmtFiscal { L"Зарегистрирована: {}" };
    constexpr Csv c_fmtCashDrawerOpened { L"Денежный ящик открыт: {}" };
    constexpr Csv c_fmtCoverOpened { L"Крышка открыта: {}" };
    constexpr Csv c_fmtReceiptPaperPresent { L"Бумага есть: {}" };
    constexpr Csv c_fmtPaperNearEnd { L"Бумага скоро закончится: {}" };
    constexpr Csv c_fmtCutError { L"Ошибка отрезчика: {}" };
    constexpr Csv c_fmtPrinterOverheat { L"Перегрев печатного механизма: {}" };
    constexpr Csv c_fmtReceiptLineLength { L"Ширина чековой ленты, симв.: {}" };
    constexpr Csv c_fmtReceiptLineLengthPix { L"Ширина чековой ленты, пикс.: {}" };
    constexpr Csv c_fmtShiftState { L"Состояние смены: {}" };
    constexpr Csv c_fmtShiftExpiration { L"Дата и время истечения текущей смены: {}" };
    constexpr Csv c_fmtReceiptType { L"Тип открытого чека: {}" };
    constexpr Csv c_fmtDocumentType { L"Тип открытого документа: {}" };
    constexpr Csv c_fmtCashInCount { L"Количество внесений: {}" };
    constexpr Csv c_fmtCashInSum { L"Сумма внесений: {}" };
    constexpr Csv c_fmtCashOutCount { L"Количество внесений: {}" };
    constexpr Csv c_fmtCashOutSum { L"Сумма выплат: {}" };
    constexpr Csv c_fmtSellCashSum { L"Сумма наличных платежей в чеках прихода (продажи): {}" };
    constexpr Csv c_fmtSellReturnCashSum { L"Сумма наличных платежей в чеках возврата прихода (продажи): {}" };
    constexpr Csv c_fmtCashSum { L"Сумма наличных в денежном ящике: {}" };
    constexpr Csv c_ofdExchangeStatus { L"Статус информационного обмена с ОФД:" };
    constexpr Csv c_fmtOfdExSBit0 { L"    транспортное соединение установлено: {}" };
    constexpr Csv c_fmtOfdExSBit1 { L"    есть сообщение для передачи в ОФД: {}" };
    constexpr Csv c_fmtOfdExSBit2 { L"    ожидание ответного сообщения (квитанции) от ОФД: {}" };
    constexpr Csv c_fmtOfdExSBit3 { L"    есть команда от ОФД: {}" };
    constexpr Csv c_fmtOfdExSBit4 { L"    изменились настройки соединения с ОФД: {}" };
    constexpr Csv c_fmtOfdExSBit5 { L"    ожидание ответа на команду от ОФД: {}" };
    constexpr Csv c_fmtUnsentCount { L"Количество неотправленных документов: {}" };
    constexpr Csv c_fmtFirstUnsentNumber { L"Номер первого неотправленного документа: {}" };
    constexpr Csv c_fmtFirstUnsentDateTime { L"Дата и время первого неотправленного документа: {}" };
    constexpr Csv c_fmtOkpDateTime { L"Дата и время последнего успешного ОКП: {}" };
    constexpr Csv c_fmtOfdMessageRead { L"Флаг наличия сообщения для ОФД: {}" };
    constexpr Csv c_fmtLastSentDateTime { L"Дата и время последней успешной отправки документа в ОФД: {}" };
    constexpr Csv c_fmtSuccessDateTime { L"Дата и время последнего успешного соединения с ОФД: {}" };
    constexpr Csv c_fmtNetworkError { L"Код ошибки сети: {}" };
    constexpr Csv c_fmtNetworkErrorText { L"Текст ошибки сети: {}" };
    constexpr Csv c_fmtOfdError { L"Код ошибки ОФД: {}" };
    constexpr Csv c_fmtOfdErrorText { L"Текст ошибки ОФД: {}" };
    constexpr Csv c_fmtFnError { L"Код ошибки ФН: {}" };
    constexpr Csv c_fmtFnErrorText { L"Текст ошибки ФН: {}" };
    constexpr Csv c_fmtDocumentNumber { L"Номер ФД, на котором произошла ошибка: {}" };
    constexpr Csv c_fmtCommandCode { L"Команда ФН, на которой произошла ошибка: {}" };
    constexpr Csv c_fmtDataForSendIsEmpty { L"Контейнер для отправки пуст: {}" };
    constexpr Csv c_fmtDeviceFfdVersion { L"Версия ФФД ККТ: {}" };
    constexpr Csv c_fmtDevMinFfdVersion { L"Минимальная версия ФФД ККТ: {}" };
    constexpr Csv c_fmtDevMaxFfdVersion { L"Максимальная версия ФФД ККТ: {}" };
    constexpr Csv c_fmtFnFfdVersion { L"Версия ФФД ФН: {}" };
    constexpr Csv c_fmtFnMaxFfdVersion { L"Максимальная версия ФФД ФН: {}" };
    constexpr Csv c_fmtFfdVersion { L"Версия ФФД: {}" };
    constexpr Csv c_fmtFirmwareVersion { L"Версия прошивки: {}" };
    constexpr Csv c_fmtConfigurationVersion { L"Версия конфигурации: {}" };
    constexpr Csv c_fmtReleaseVersion { L"Версия релиза: {}" };
    constexpr Csv c_fmtTemplatesVersion { L"Версия движка шаблонов: {}" };
    constexpr Csv c_fmtControlUnitVersion { L"Версия блока управления: {}" };
    constexpr Csv c_fmtBootVersion { L"Версия загрузчика: {}" };
}
