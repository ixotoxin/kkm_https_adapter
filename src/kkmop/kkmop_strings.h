// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <string_view>

namespace Main::Wcs {
    using Csv = const std::wstring_view;

    constexpr Csv c_kkmopUsage {
        L"    learn _пп_ ...      Добавить ККМ\n"
        L"    base-status _сн_    Вывести базовый статус ККМ (п1)\n"
        L"    status _сн_         Вывести статус ККМ (п1)\n"
        L"    full-status _сн_    Вывести полный статус ККМ (п1)\n"
        L"    cash-stat _сн_      Вывести информации о наличных (п1)\n"
        L"    demo-print _сн_     Выполнить демо-печать (п2)\n"
        L"    info _сн_           Напечатать информацию о ККМ (п2)\n"
        L"    fn-regs _сн_        Напечатать итоги регистрации / перерегистрации (п2)\n"
        L"    ofd-status _сн_     Напечатать отчет о состоянии расчетов (п2)\n"
        L"    ofd-test _сн_       Тестировать подключение ККМ к ОФД (п2)\n"
        L"    shift-reports _сн_  Напечатать нераспечатанные отчеты о закрытии смены (п2)\n"
        L"    last-document _сн_  Напечатать копию последнего документа (п2)\n"
        L"    report-x _сн_       Печать Х-отчёта ККМ (п2)\n"
        L"    close-shift _сн_    Закрыть смену ККМ (п2)\n"
        L"    reset-state _сн_    Возврат ККМ к исходному состоянию (п2)\n"
        L"где _пп_ - параметры подключения (можно указать несколько)\n"
        L"    _сн_ - серийный номер ККМ\n"
        L"    (п1) - вывод в консоль в JSON-формате\n"
        L"    (п2) - печать документа + вывод в консоль в JSON-формате\n"
    };

    constexpr Csv c_commandPrefix { L"ПП [{}]: " };
    constexpr Csv c_prefixedText { L"ПП [{}]: {}" };
    constexpr Csv c_getKkmInfo { L"ПП [{}]: ККМ [{}]: Получение информации об устройстве" };
    constexpr Csv c_connParamsSaved { L"ПП [{}]: Параметры подключения ККМ [{}] успешно сохранены" };
}
