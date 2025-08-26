# KKM HTTPS Adapter

## Описание

Реализация службы для ОС Windows предоставляющей доступ к подключенной к компьютеру контрольно-кассовой машине (далее
ККМ) поддерживаемой драйвером компании АТОЛ. Для доступа к ККМ используется REST JSON API поверх HTTPS. Это не
самостоятельный продукт, это компонент распределенной системы. С помощью него можно реализовать доступ ПО работающего
на удаленном сервере к ККМ локально подключенной к тонкому клиенту оператора. Роль этого сервера - применение в
приложениях имеющих веб-интерфейс и бизнес-логику реализованную на удаленном сервере. В связи с этим полноценное
интерактивно взаимодействие с ККМ в некоторых случаях труднореализуемо. Поэтому все стадии выполнения операции
(регистрация прихода, закрытие смены и т.п.) выполняются атомарно.

Так же данный код можно рассматривать как пример реализации HTTPS-сервера на C++ с использованием Asio, OpenSSL и
корутин.

## Конфигурирование

После запуска `kkmha.exe` рабочей директорией устанавливается директория, в которой находится `kkmha.exe`. Все
относительные пути вычисляются относительно рабочей директории (далее `{work-dir}`). Если не установлена переменная
окружения `KKMHA_CONF_DIR` конфигурационный файл `kkmha.json` ищется в директории `{work-fir}\conf`. Пример
конфигурационного файла:

```json
{
    "log": {
        "console": {
            "level": "debug",
            "outputTimestamp": false,
            "outputLevel": true
        },
        "file": {
            "level": "debug",
            "directory": "logs"
        },
        "eventLog": {
            "level": "none"
        },
        "appendLocation": false
    },
    "server": {
        "ipv4Only": false,
        "port": 5757,
        "concurrencyLimit": 10,
        "enableLegacyTls": "no",
        "securityLevel": 5,
        "certificateChainFile": "conf\\kkmha.crt",
        "privateKeyFile": "conf\\kkmha.key",
        "privateKeyPassword": "",
        "secret": "lorem.ipsum.dolor.sit.amet",
        "loopbackWithoutSecret": false,
        "enableStatic": false,
        "staticDirectory": "static",
        "indexFile": "index.html",
        "mimeMap": "conf\\mime.json",
        "enableUnknownType": false
    },
    "kkm": {
        "dbDirectory": "conf\\kkm",
        "defaultBaudRate": 115200,
        "defaultLineLength": 42,
        "timeZone": "device",
        "documentClosingTimeout": 5000,
        "cliOperator": {
            "name": "Оператор"
        },
        "customerAccountField": "Лицевой счёт: ",
        "maxCashInOut": 500000,
        "maxPrice": 300000,
        "maxQuantity": 1000
    }
}
```

| Опция                          | Описание                                                                                                     |
|--------------------------------|--------------------------------------------------------------------------------------------------------------|
| `log.console.level`            | Уровень логирования в консоль                                                                                |
| `log.console.outputTimestamp`  | Включить/выключить вывод даты и времени в консоль                                                            |
| `log.console.outputLevel`      | Включить/выключить вывод уровня сообщений в консоль                                                          |
| `log.file.level`               | Уровень логирования в файл                                                                                   |
| `log.file.directory`           | Директория, в которую будет происходить логирование                                                          |
| `log.eventLog.level`           | Уровень логирования в журнал событий Windows                                                                 |
| `log.appendLocation`           | Включить/выключить вывод точки происхождения сообщения в исходных файлах                                     |
| `server.ipv4Only`              | Включить/выключить поддержку IPv6                                                                            |
| `server.port`                  | Порт, который будет слушать сервер                                                                           |
| `server.concurrencyLimit`      | Ограничение максимального количества одновременных соединений                                                |
| `server.enableLegacyTls`       | Разрешить/запретить поддержку TLS 1.0 и TLS 1.1                                                              |
| `server.securityLevel`         | Уровень безопасности устанавливаемый в библиотеке OpenSSL (0 - 5)                                            |
| `server.certificateChainFile`  | Путь к файлу сертификата                                                                                     |
| `server.privateKeyFile`        | Путь к файлу ключа                                                                                           |
| `server.privateKeyPassword`    | Пароль от ключа                                                                                              |
| `server.secret`                | Access-токен                                                                                                 |
| `server.loopbackWithoutSecret` | Разрешить/запретить локальные запросы без access-токена                                                      | 
| `server.enableStatic`          | Разрешить/запретить обработку запросов `https://127.0.0.1:5757/static/{file-path}`                           |
| `server.staticDirectory`       | Путь к директории, содержимое которой будет отдаваться для запросов `/static/{file-path}`                    |
| `server.indexFile`             | Имя индексного файла (на этот файл происходит перенаправление, если запрашиваемый путь является директорией) |
| `server.mimeMap`               | Путь к файлу с описанием типа содержимого                                                                    |
| `server.enableUnknownType`     | Разрешить/запретить отдавать файлы с расширениями не представленными в файле `mime.json`                     |
| `kkm.dbDirectory`              | Путь к директории, в которой будет формироваться БД известных ККМ                                            |
| `kkm.defaultBaudRate`          | Скорость COM-порта по-умолчанию                                                                              |
| `kkm.defaultLineLength`        | Ширина чековой ленты по-умолчанию. Используется, если данное свойство не удается получить опросом ККМ        |
| `kkm.timeZone`                 | Временная зона передаваемая ОФД при регистрации чека                                                         |
| `kkm.documentClosingTimeout`   | Таймаут проверки корректности закрытия документа                                                             |
| `kkm.cliOperator.name`         | Имя оператора для консольных команд требующих логина оператора ККМ                                           |
| `kkm.customerAccountField`     | Заголовок поля с номером лицевого счета клиента                                                              |
| `kkm.maxCashInOut`             | Максимальная сумма для внесения или выплаты                                                                  |
| `kkm.maxPrice`                 | Максимальная цена товара/услуги в чеке                                                                       |
| `kkm.maxQuantity`              | Максимальное количество товара/услуги в чеке                                                                 |

***Сертификат и ключ никак не проверяются***, поэтому можно использовать самоподписанный сертификат.
<!-- Создать его можно например так:
```cmd
openssl req -x509 -sha256 -nodes -days 3650 -newkey rsa:2048 -keyout kkmha.test.ss.key -out kkmha.test.ss.crt
```
-->
Так же можно изменить параметры по-умолчанию и недоступные для конфигурирования в файле `.\src\defauls.h`

## Использование

Получаемый после сборки исполняемый файл использует интерфейс командной строки.

```
kkmha.exe команда [аргумент ...]
```

| Команда               | Описание                                                  |
|-----------------------|-----------------------------------------------------------|
| help                  | Вывести справку                                           |
| show-config           | Вывести конфигурацию                                      |
| install               | Установить сервер как службу Windows                      |
| uninstall             | Удалить службу Windows                                    |
| start                 | Запустить службу                                          |
| stop                  | Остановить службу                                         |
| restart               | Перезапустить службу                                      |
| foreground            | Запустить сервер как foreground-процесс                   |
| learn _пп_ [_пп_ ...] | Добавить ККМ                                              |
| status _сн_           | Вывести статус ККМ _(п1)_                                 |
| full-status _сн_      | Вывести полный статус ККМ _(п1)_                          |
| cash-stat _сн_        | Вывести информации о наличных _(п1)_                      |
| demo-print _сн_       | Выполнить демо-печать _(п2)_                              |
| info _сн_             | Напечатать информацию о ККМ _(п2)_                        |
| fn-regs _сн_          | Напечатать итоги регистрации / перерегистрации _(п2)_     |
| ofd-status _сн_       | Напечатать отчет о состоянии расчетов _(п2)_              |
| ofd-test _сн_         | Тестировать подключение ККМ к ОФД _(п2)_                  |
| shift-reports _сн_    | Напечатать нераспечатанные отчеты о закрытии смены _(п2)_ |
| last-document _сн_    | Напечатать копию последнего документа _(п2)_              |
| report-x _сн_         | Печать Х-отчёта ККМ _(п2)_                                |
| close-shift _сн_      | Закрыть смену ККМ _(п2)_                                  |
| reset-state _сн_      | Возврат ККМ к исходному состоянию _(п2)_                  |

где
- _пп_ - параметры подключения (можно указать несколько);
- _сн_ - серийный номер ККМ;
- _(п1)_ - вывод в консоль в JSON-формате;
- _(п2)_ - печать документа + вывод в консоль в JSON-формате.

***Для идентификации ККМ подключенной к компьютеру как в командной строке, так и в API используется её серийный номер.***

Параметры подключения имеют следующий формат `тип,аргумент1,аргумент2,...,аргументN`, где `тип` - это протокол или
интерфейс подключения ККМ. В данный момент реализован подключения только по COM-порту (тип `"com"`) (в том числе
виртуальному, т.е. подключение через USB). Аргументы для данного типа подключения:
- аргумент1 - номер COM-порта;
- аргумент2 - скорость COM-порта. Если данный аргумент опущен, то используется значение из конфигурационного файла
  `kkm.defaultBaudRate`. Если же и в конфигурационном файле это значение не задано, то используется 115200.

Так же зарезервированы, но не реализованы следующие протоколы и интерфейсы: `"usb"`, `"tcpip"` (`"ip"`), `"bluetooth"`
(`"bt"`).

Примеры:
```cmd
kkmha.exe learn com,4 com,6
```
```cmd
kkmha.exe close-shift 98765433456789
```
```cmd
kkmha.exe status 98765433456789
```

## API

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

### Получение статуса ККМ

Запрос:
```http request
GET https://192.168.11.22:5757/kkm/{serial-number}/status
```
```http request
GET https://192.168.11.22:5757/kkm/98765433456789/status
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

Больше примеров запросов к API можно найти в директории `.\examples\`. Представленный в этой директории код не является
библиотекой, потому как не содержит никаких проверок входных данным и прочего обязательного кода. Это именно примеры.

## Особенности реализации

Реализация сервера не претендует на полное соответствие стандартам описывающих протоколы HTTP и HTTPS. HTTPS здесь
используется только как транспортный протокол позволяющий замаскировать трафик в локальной сети и имеющий множество
клиентов для разных ЯП. Реализация протокола HTTP здесь минимально необходимая и достаточная для решения поставленной
задачи. Для достижения максимальной безопасности необходимо реализовать проверку сертификатов и, крайне
желательно, требовать от клиента клиентский сертификат, который тоже надо проверять. 

Не реализована работа с таймаутами, поэтому вероятность 'повисших' соединений отлична от 0, но маловероятна при
стабильной работе сети и отсутствии ошибок в запросах клиентов. Сами по себе зависшие соединения серверу не
помешают обрабатывать другие подключения, потому как сервер неблокирующий. Но при должном упорстве можно устроить
DOS-атаку на сервер. Поэтому без доработки во внешний мир доступ к нему лучше не открывать.

Сервер не имеет встроенных механизмов фильтрации. Это ещё одна причина, почему рекомендуется ограничить доступ к серверу
файерволом.

Не реализовано URI-декодирование, поэтому не стоит использовать в обработчиках запросов и именах файлов в директории
`{work-fir}\static\` буквы и символы, которые требуют URI-кодирования.

## Требуемые компоненты

Используются следующие библиотеки:
- Библиотека из состава АТОЛ Драйвер ККТ v10.10.7+ ([Оф.сайт](https://fs.atol.ru/))
- Asio C++ Library ([Git](https://github.com/chriskohlhoff/asio), [Оф.сайт](https://think-async.com/Asio/))
- OpenSSL ([Git](https://github.com/openssl/openssl), [Оф.сайт](https://www.openssl.org/))
- JSON for Modern C++ ([Git](https://github.com/nlohmann/json), [Оф.сайт](https://json.nlohmann.me/))
- Catch2 ([Git](https://github.com/catchorg/Catch2))

## Сборка

Для сборки прежде всего потребуются:
- АТОЛ Драйвер ККТ версии не ниже 10.10.7 ([Оф.сайт](https://fs.atol.ru/))
- vcpkg ([Git](https://github.com/microsoft/vcpkg), [Оф.сайт](https://learn.microsoft.com/en-us/vcpkg/))

Последний можно заменить другим менеджером зависимостей для C++ или использовать уже готовые сборки указанных выше 
библиотек. В этом случае возможно придется поправить `CMakeLists.txt`.

Сборка тестировалась с помощью компиляторов MSVC и Clang. Скрипты с типовой сборкой для этих компиляторов лежат в
директориях `.\_msvc` и `.\_clang` соответственно.

| Скрипт                      | Описание                                                                                                                                                             |
|-----------------------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `config_env.cmd`            | Установка переменных окружения и запуск необходимых bootstrap-скриптов. Вызывается из ниже перечисленных скриптов                                                    |
| `install_dynamic_deps.cmd`  | Установка динамически собранных зависимостей и копирование необходимых заголовочных файлов из `%programfiles%\ATOL\Drivers10\KKT\langs\cpp\fptr10` в `.\deps\fptr10` |
| `install_static_deps.cmd`   | Установка статически собранных зависимостей и копирование необходимых заголовочных файлов из `%programfiles%\ATOL\Drivers10\KKT\langs\cpp\fptr10` в `.\deps\fptr10`  |
| `test_dynamic.cmd`          | Сборка динамической отладочной версии и запуск unit-тестов                                                                                                           |
| `test_static.cmd`           | Сборка статической отладочной версии и запуск unit-тестов                                                                                                            |
| `build_dynamic_mprof.cmd`   | Сборка динамической отладочной версии с включенным профилированием памяти                                                                                            |
| `build_static_mprof.cmd`    | Сборка статической отладочной версии с включенным профилированием памяти                                                                                             |
| `build_dynamic_debug.cmd`   | Сборка динамической отладочной версии                                                                                                                                |
| `build_static_debug.cmd`    | Сборка статической отладочной версии                                                                                                                                 |
| `build_dynamic_release.cmd` | Сборка динамической релизной версии                                                                                                                                  |
| `build_static_release.cmd`  | Сборка статической релизной версии                                                                                                                                   |

Предполагается, что необходимое ПО установлено в следующих директориях:

| ПО                                             | Путь                                                    |
|------------------------------------------------|---------------------------------------------------------|
| АТОЛ Драйвер                                   | `%programfiles%\ATOL`                                   |
| Microsoft Visual Studio 2022 Community Edition | `%programfiles%\Microsoft Visual Studio\2022\Community` |
| Clang                                          | `C:\Devel\Platform\Clang\20.1.8-x86_64`                 |
| CMake                                          | `C:\Devel\Platform\CMake\3.31.7-x86_64`                 |
| Ninja                                          | `C:\Devel\Platform\Ninja\1.13.1`                        |

Пути можно изменить в файлах `config_env.cmd` и `install_*_deps.cmd`. CMake и Ninja можно использовать из состава MSVS.

После сборки одним из скриптов `build_*.cmd` в директорию `.\_build` будет установлен файл `kkmha.exe` и, в случае
динамической сборки, файлы `libcrypto-?-x64.dll`, `libssl-?-x64.dll`. Так же в этой директории уже находятся файлы
конфигурации и т.п., формирующие готовое окружение для запуска.

Для запуска на ПК, кроме того, на котором производилась сборка может потребоваться установка последнего пакета
[Microsoft Visual C++ Redistributable](https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170).

---
Данный текст является кратким обзорным описанием содержимого.
Более полная документация возможно появится позже, но это не точно.
