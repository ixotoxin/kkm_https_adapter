## API

***Данный раздел руководства находится в стадии разработки.***

После запуска как службы или foreground-процесса сервер будет принимать HTTPS соединения на порт 5757 (можно изменить
в конфигурационном файле). Если запрос имеет тело, то оно должно быть в формате JSON в кодировке UTF-8. Заголовок
`Content-Type` может отсутствовать, либо иметь следующий вид:
```
Content-Type: application/json
```
```
Content-Type: application/json; charset=utf-8
```
Обязателен заголовок `X-Secret` являющийся access-токеном. В конфигурационном файле можно отключить требование
`X-Secret` для локальных запросов, типа `GET https://127.0.0.1:5757/kkm/98765433456789/status`. В случае неверного
access-токена сервер вернет код HTTP-статуса `403 Forbidden`. Access-токен задается в конфигурационном файле.

Для POST-запросов обязательным заголовком является `X-Idempotency-Key` содержащий ключ идемпотентности. Этот ключ
позволяет избежать многократного выполнения операции в случае дублирования запросов, которое может случится, например,
из-за нестабильной работы сети. Так же ключ идемпотентности используется для кеширования результатов запроса. То есть
многократное повторение запроса от одного клиента с одинаковым ключом приведет к однократному выполнению операции, а
возвращаться всегда будет один и тот-же результат. Ключ идемпотентности может присутствовать и у GET-запросов, тогда их
результат тоже будет кешироваться.

Так же для POST-запросов обязателен корректный заголовок `Content-Length`.

Другие заголовки игнорируются.

Почти все запросы (кроме `https://192.168.11.22:5757/static/{file-path}` и `https://192.168.11.22:5757/`) возвращают
JSON с обязательными полями `"!message"` и `"!success"`. Так же возвращаются ситуативно корректные коды HTTP-статуса.

***Для идентификации ККМ подключенной к компьютеру как в API, так и в командной строке используется её серийный номер.***

### Получение статуса ККМ

Запрос:
```http request
GET https://192.168.11.22:5757/kkm/{serial-number}/base-status
```
```http request
GET https://192.168.11.22:5757/kkm/98765433456789/base-status
```
Заголовки запроса:
```
Content-Type: application/json; charset=utf-8
X-Idempotency-Key: TO6XVSGQWH1Z3E6CIS27KOSN2UEKE6SJ
X-Secret: lorem.ipsum.dolor.sit.amet
```
Ответ:
```
HTTP/1.1 200 Ok
```
Тело ответа:
```json
{
    "!message": "Ok",
    "!success": true,
    "status": {
        "blocked": false,
        "cashDrawerOpened": false,
        "coverOpened": false,
        "cutError": false,
        "dateTime": "2025-01-27 12:24:44",
        "documentNumber": 1,
        "documentType": 0,
        "documentTypeText": "Документ закрыт",
        "fiscal": true,
        "fnFiscal": true,
        "fnPresent": true,
        "invalidFn": false,
        "logicalNumber": 0,
        "mode": 0,
        "model": 63,
        "operatorId": 0,
        "operatorRegistered": true,
        "paperNearEnd": false,
        "printerConnectionLost": false,
        "printerError": false,
        "printerOverheat": false,
        "receiptLineLength": 42,
        "receiptLineLengthPix": 384,
        "receiptNumber": 0,
        "receiptPaperPresent": true,
        "receiptSum": 0.0,
        "receiptType": 0,
        "receiptTypeText": "Чек закрыт",
        "serialNumber": "98765433456789",
        "shiftNumber": 0,
        "shiftState": 0,
        "shiftStateText": "Сессия закрыта",
        "subMode": 0
    }
}
```
В остальных примерах заголовки и код статуса ответа будут опущены.

### Получение полного статуса ККМ

Запрос:
```http request
GET https://192.168.11.22:5757/kkm/{serial-number}/full-status
```
```http request
GET https://192.168.11.22:5757/kkm/98765433456789/full-status
```
Тело ответа:
```json
{
    "!message": "Ok",
    "!success": true,
    "cashStat": {
        "field": "здесь и далее содержимое опущено"
    },
    "fnInfo": {
    },
    "fndtErrors": {
    },
    "lastDocument": {
    },
    "lastReceipt": {
    },
    "lastRegistration": {
    },
    "ofdExchangeStatus": {
    },
    "receiptState": {
    },
    "shiftState": {
    },
    "status": {
    },
    "versions": {
    }
}
```
или
```json
{
    "!message": "Описание ошибки",
    "!success": false
}
```
и, возможно, иной код HTTP-статуса.

### Добавление новой ККМ в базу

Запрос:
```http request
POST https://192.168.11.22:5757/kkm/learn
```
Тело запроса:
```json
{
    "connParams": "com,4"
}
```
Тело ответа:
```json
{
    "!message": "Ok",
    "!success": true
}
```
или
```json
{
    "!message": "Описание ошибки",
    "!success": false
}
```
и, возможно, иной код HTTP-статуса. Подобным образом сервер отвечает и на другие запросы, поэтому далее ответы без
дополнительных полей будут опущены.

### Сброс кеша параметров подключения

Запрос:
```http request
POST https://192.168.11.22:5757/kkm/reset-registry
```
Выполнение данного запроса полезно для исключения коллизий в случае замены или удаления ККМ.

### Демонстрационная печать

Запрос:
```http request
POST https://192.168.11.22:5757/kkm/{serial-number}/print-demo
```
```http request
POST https://192.168.11.22:5757/kkm/98765433456789/print-demo
```

### Печать нефискального документа

Запрос:
```http request
POST https://192.168.11.22:5757/kkm/{serial-number}/print-non-fiscal-doc
```
```http request
POST https://192.168.11.22:5757/kkm/98765433456789/print-non-fiscal-doc
```
Тело запроса:
```json
{
    "TODO": "Добавить структуру"
}
```

### Печать информации о ККМ

Запрос:
```http request
POST https://192.168.11.22:5757/kkm/{serial-number}/print-info
```
```http request
POST https://192.168.11.22:5757/kkm/98765433456789/print-info
```

### Печать итогов регистрации / перерегистрации

Запрос:
```http request
POST https://192.168.11.22:5757/kkm/{serial-number}/print-fn-registrations
```
```http request
POST https://192.168.11.22:5757/kkm/98765433456789/print-fn-registrations
```

### Печать отчета о состоянии расчетов

Запрос:
```http request
POST https://192.168.11.22:5757/kkm/{serial-number}/print-ofd-exchange-status
```
```http request
POST https://192.168.11.22:5757/kkm/98765433456789/print-ofd-exchange-status
```

### Тестирование подключения ККМ к ОФД и печать результата

Запрос:
```http request
POST https://192.168.11.22:5757/kkm/{serial-number}/print-ofd-test
```
```http request
POST https://192.168.11.22:5757/kkm/98765433456789/print-ofd-test
```

### Печать нераспечатанных отчетов о закрытии смены

Запрос:
```http request
POST https://192.168.11.22:5757/kkm/{serial-number}/print-close-shift-reports
```
```http request
POST https://192.168.11.22:5757/kkm/98765433456789/print-close-shift-reports
```

### Печать копии последнего документа

Запрос:
```http request
POST https://192.168.11.22:5757/kkm/{serial-number}/print-last-document
```
```http request
POST https://192.168.11.22:5757/kkm/98765433456789/print-last-document
```

### Получить информацию о наличных

Запрос:
```http request
GET https://192.168.11.22:5757/kkm/{serial-number}/cash-stat
```
```http request
GET https://192.168.11.22:5757/kkm/98765433456789/cash-stat
```
Тело ответа:
```json
{
    "!message": "Ok",
    "!success": true,
    "cashStat": {
        "cashInCount": 0,
        "cashInSum": 0.0,
        "cashOutCount": 0,
        "cashOutSum": 0.0,
        "cashSum": 0.0,
        "sellCashSum": 0.0,
        "sellReturnCashSum": 0.0
    }
}
```

### Внесение наличных и печать отчета

Запрос:
```http request
POST https://192.168.11.22:5757/kkm/{serial-number}/cash-in
```
```http request
POST https://192.168.11.22:5757/kkm/98765433456789/cash-in
```
Тело запроса:
```json
{
    "operator": {
        "name": "Оператор"
    },
    "cashSum": 1000.0,
    "electronically": false
}
```
Поле `"electronically"` указывает печатать ли бумажный чек. Это поле имеет такой же эффект для запросов:
- `/kkm/{serial-number}/cash-out`;
- `/kkm/{serial-number}/sell`;
- `/kkm/{serial-number}/sell-return`.

Поле `"operator.name"` является обязательным, в том числе для запросов:
- `/kkm/{serial-number}/cash-out`;
- `/kkm/{serial-number}/sell`;
- `/kkm/{serial-number}/sell-return`;
- `/kkm/{serial-number}/report-x`;
- `/kkm/{serial-number}/report-z`;
- `/kkm/{serial-number}/close-shift`;
- `/kkm/{serial-number}/reset-state`.

### Выплата наличных и печать отчета

Запрос:
```http request
POST https://192.168.11.22:5757/kkm/{serial-number}/cash-out
```
```http request
POST https://192.168.11.22:5757/kkm/98765433456789/cash-out
```
Тело запроса:
```json
{
    "operator": {
        "name": "Оператор"
    },
    "cashSum": 1000.0
}
```

### Регистрация прихода и печать чека

Запрос:
```http request
POST https://192.168.11.22:5757/kkm/{serial-number}/sell
```
```http request
POST https://192.168.11.22:5757/kkm/98765433456789/sell
```
Тело запроса:
```json
{
    "operator": {
        "name": "Оператор"
    },
    "customer": {
        "account": 123456,
        "contact": "email@example.com"
    },
    "items": [
        {
            "title": "Молоко, 0.95л",
            "price": 86.0,
            "quantity": 2,
            "unit": "piece",
            "tax": 10
        },
        {
            "title": "Хлопья",
            "price": 130.0,
            "quantity": 2,
            "unit": "piece",
            "tax": 20
        }
    ],
    "text": {
        "content": "Необязательный текст в чеке",
        "center": false,
        "magnified": false,
        "separated": false
    },
    "headerText": {
        "content": "Необязательный текст в клише чека",
        "center": false,
        "magnified": false,
        "separated": false
    },
    "footerText": {
        "content": "Необязательный текст в подвале чека",
        "center": false,
        "magnified": false,
        "separated": false
    }
}
```

### Регистрация возврата прихода и печать чека

Запрос:
```http request
POST https://192.168.11.22:5757/kkm/{serial-number}/sell-return
```
```http request
POST https://192.168.11.22:5757/kkm/98765433456789/sell-return
```
Тело запроса:
```json
{
    "operator": {
        "name": "Оператор"
    },
    "customer": {
        "account": 123456,
        "contact": "email@example.com"
    },
    "items": [
        {
            "title": "Молоко, 0.95л",
            "price": 86.0,
            "quantity": 1,
            "unit": "piece",
            "tax": 10
        }
    ]
}
```

### Печать X-отчета

Запрос:
```http request
POST https://192.168.11.22:5757/kkm/{serial-number}/report-x
```
```http request
POST https://192.168.11.22:5757/kkm/98765433456789/report-x
```
Тело запроса:
```json
{
    "operator": {
        "name": "Оператор"
    },
    "closeShift": true,
    "cashOut": true
}
```

### Закрытие смены и печать Z-отчета

Запрос:
```http request
POST https://192.168.11.22:5757/kkm/{serial-number}/report-z
```
```http request
POST https://192.168.11.22:5757/kkm/98765433456789/report-z
```
```http request
POST https://192.168.11.22:5757/kkm/{serial-number}/close-shift
```
```http request
POST https://192.168.11.22:5757/kkm/98765433456789/close-shift
```
Тело запроса:
```json
{
    "operator": {
        "name": "Оператор"
    },
    "cashOut": true
}
```

### Сброс состояния ККМ

Запрос отменяет чек, если таковой открыт. Делает выплату наличных, если запрошено. Закрывает смену, если таковая открыта.

Запрос:
```http request
POST https://192.168.11.22:5757/kkm/{serial-number}/reset-state
```
```http request
POST https://192.168.11.22:5757/kkm/98765433456789/reset-state
```
Тело запроса:
```json
{
    "operator": {
        "name": "Оператор"
    },
    "closeShift": true,
    "cashOut": true
}
```

### Пинг службы

Запрос:
```http request
POST https://192.168.11.22:5757/ping
```
Тело ответа:
```json
{
    "!message": "Pong",
    "!success": true
}
```

### Получение файлов

Запрос:
```http request
GET https://192.168.11.22:5757/static/{file-path}
```
```http request
GET https://192.168.11.22:5757/static/index.html
```
Сервер вернет файл `{work-fir}\static\index.html` (директорию можно изменить в конфигурационном файле).

### Получение конфигурации

Запрос:
```http request
GET https://192.168.11.22:5757/config/general
```
Тело ответа:
```json
{
    "!message": "OK",
    "!success": true,
    "cliOperator": {
        "inn": "",
        "name": "Оператор"
    },
    "knownDevices": [
        "12345678901234",
        "98765433456789"
    ]
}
```
Сервер вернет JSON с конфигурацией достаточной для реализации веб-приложения.

### Примечание

Последние 2 вида запросов (получение файлов и конфигурации) позволяют реализовать локальное веб-приложение для работы
с ККМ.

Больше примеров запросов к API можно найти в директории `.\examples\php\`. Представленный в этой директории код не
является библиотекой, потому как не содержит никаких проверок входных данным и прочего обязательного кода. Это именно
примеры.

---
[Назад к содержанию](../README.md)
