## Конфигурирование

После запуска `kkmha.exe` рабочей директорией устанавливается директория, в которой находится `kkmha.exe`. Все
относительные пути вычисляются относительно рабочей директории (далее `{work-dir}`). Если не установлена переменная
окружения `KKMHA_CONF_DIR` конфигурационный файл `kkmha.json` ищется в директории `{work-fir}\conf`. Все
относительные пути в конфигурационном файле вычисляются относительно директории с основным конфигурационным файлом,
кроме опции `log.file.directory`. Она вычисляется относительно рабочей директории. Пример конфигурационного файла:

```json
{
    "log": {
        "console": {
            "level": "debug",
            "outputTimestamp": false,
            "outputLevel": true
        },
        "file": {
            "level": {
                "foreground": "none",
                "background": "debug"
            },
            "directory": "logs"
        },
        "eventLog": {
            "level": {
                "foreground": "none",
                "background": "info"
            }
        },
        "appendLocation": false
    },
    "server": {
        "ipv4Only": false,
        "port": 5757,
        "requestTimeout": 180,
        "concurrencyLimit": 10,
        "enableLegacyTls": "no",
        "securityLevel": 5,
        "certificateChainFile": "kkmha.crt",
        "privateKeyFile": "kkmha.key",
        "privateKeyPassword": "",
        "secret": "lorem.ipsum.dolor.sit.amet",
        "loopbackWithoutSecret": false,
        "enableStatic": false,
        "staticDirectory": "..\\static",
        "indexFile": "index.html",
        "mimeMap": "mime.json",
        "enableUnknownType": false
    },
    "kkm": {
        "dbDirectory": "kkm",
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

| ОПЦИЯ                           | ОПИСАНИЕ                                                                                                              |
|---------------------------------|-----------------------------------------------------------------------------------------------------------------------|
| `log.console.level`             | Уровень логирования в консоль.                                                                                        |
| `log.console.outputTimestamp`   | Включить/выключить вывод даты и времени в консоль.                                                                    |
| `log.console.outputLevel`       | Включить/выключить вывод уровня сообщений в консоль.                                                                  |
| `log.file.level.foreground`     | Уровень логирования в файл для foreground-процесса.                                                                   |
| `log.file.level.background`     | Уровень логирования в файл для background-процесса.                                                                   |
| `log.file.directory`            | Директория, в которую будет происходить логирование.                                                                  |
| `log.eventLog.level.foreground` | Уровень логирования в журнал событий Windows для foreground-процесса.                                                 |
| `log.eventLog.level.background` | Уровень логирования в журнал событий Windows для background-процесса.                                                 |
| `log.appendLocation`            | Включить/выключить вывод точки происхождения сообщения в исходных файлах.                                             |
| `server.ipv4Only`               | Включить/выключить поддержку IPv6.                                                                                    |
| `server.port`                   | Порт, который будет слушать сервер.                                                                                   |
| `server.requestTimeout`         | Таймаут (в секундах).                                                                                                 |
| `server.concurrencyLimit`       | Ограничение максимального количества одновременных соединений.                                                        |
| `server.enableLegacyTls`        | Разрешить/запретить поддержку TLS 1.0 и TLS 1.1.                                                                      |
| `server.securityLevel`          | Уровень безопасности устанавливаемый в библиотеке OpenSSL (0 - 5). Только для `"enableLegacyTls": false`.             |
| `server.certificateChainFile`   | Путь к файлу сертификата.                                                                                             |
| `server.privateKeyFile`         | Путь к файлу ключа.                                                                                                   |
| `server.privateKeyPassword`     | Пароль от ключа.                                                                                                      |
| `server.secret`                 | Access-токен.                                                                                                         |
| `server.loopbackWithoutSecret`  | Разрешить/запретить локальные запросы без access-токена.                                                              | 
| `server.enableStatic`           | Разрешить/запретить обработку запросов `https://127.0.0.1:5757/static/{file-path}`.                                   |
| `server.staticDirectory`        | Путь к директории, содержимое которой будет отдаваться для запросов `/static/{file-path}`.                            |
| `server.indexFile`              | Имя индексного файла. На этот файл происходит перенаправление, если запрашиваемый путь является директорией.          |
| `server.mimeMap`                | Путь к файлу с описанием типа содержимого.                                                                            |
| `server.enableUnknownType`      | Разрешить/запретить отдавать файлы с расширениями не представленными в файле `mime.json` заданном опцией `"mimeMap"`. |
| `kkm.dbDirectory`               | Путь к директории, в которой будет формироваться БД известных ККМ.                                                    |
| `kkm.defaultBaudRate`           | Скорость COM-порта по-умолчанию.                                                                                      |
| `kkm.defaultLineLength`         | Ширина чековой ленты по-умолчанию. Используется, если данное свойство не удается получить опросом ККМ.                |
| `kkm.timeZone`                  | Временная зона передаваемая ОФД при регистрации чека.                                                                 |
| `kkm.documentClosingTimeout`    | Таймаут проверки корректности закрытия документа (в миллисекундах).                                                   |
| `kkm.cliOperator.name`          | Имя оператора для консольных команд требующих логина оператора ККМ.                                                   |
| `kkm.customerAccountField`      | Заголовок поля с номером лицевого счета клиента.                                                                      |
| `kkm.maxCashInOut`              | Максимальная сумма для внесения или выплаты.                                                                          |
| `kkm.maxPrice`                  | Максимальная цена товара/услуги в чеке.                                                                               |
| `kkm.maxQuantity`               | Максимальное количество товара/услуги в чеке.                                                                         |

***Сертификат и ключ никак не проверяются***, поэтому можно использовать самоподписанный сертификат.
<!-- Создать его можно например так:
```cmd
openssl req -x509 -sha256 -nodes -days 3650 -newkey rsa:2048 -keyout kkmha.test.ss.key -out kkmha.test.ss.crt
```
-->
Так же можно изменить параметры по-умолчанию и недоступные для конфигурирования в следующих файлах:
- `.\src\library\log\defaults.h`;
- `.\src\library\kkm\defaults.h`;
- `.\src\library\config\defaults.h`;
- `.\src\kkmha\http_defaults.h`;
- `.\src\kkmha\server_defaults.h`;
- `.\src\kkmha\server_static_defaults.h`;
- `.\src\kkmha\service_defaults.h`.

---
[Назад к содержанию](../README.md)
