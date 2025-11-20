## Сборка

Для сборки проекта используются следующие библиотеки и инструменты:
- Библиотека из состава "АТОЛ Драйвер ККТ" версии не ниже 10.10.7 ([Оф.сайт](https://fs.atol.ru/))
- Asio C++ Library ([Git](https://github.com/chriskohlhoff/asio), [Оф.сайт](https://think-async.com/Asio/))
- OpenSSL ([Git](https://github.com/openssl/openssl), [Оф.сайт](https://www.openssl.org/))
- JSON for Modern C++ ([Git](https://github.com/nlohmann/json), [Оф.сайт](https://json.nlohmann.me/))
- Catch2 ([Git](https://github.com/catchorg/Catch2))
- Microsoft Visual Studio 2022 Community Edition ([Оф.сайт](https://visualstudio.microsoft.com/))
- *или* Clang ([Git](https://github.com/llvm/llvm-project), [Оф.сайт](https://clang.llvm.org/)) + CMake ([Git](https://github.com/Kitware/CMake), [Оф.сайт](https://cmake.org/)) + Ninja ([Git](https://github.com/ninja-build/ninja), [Оф.сайт](https://ninja-build.org/))
- vcpkg ([Git](https://github.com/microsoft/vcpkg), [Оф.сайт](https://learn.microsoft.com/en-us/vcpkg/))

**vcpkg** можно заменить другим менеджером зависимостей для C++ или использовать уже готовые сборки указанных выше
библиотек. В этом случае возможно придется поправить `CMakeLists.txt`.

Сборка тестировалась с помощью компиляторов MSVC и Clang. Скрипты с типовой сборкой для этих компиляторов лежат в
директориях `.\_msvc` и `.\_clang` соответственно. Содержание директорий:

| СКРИПТ                     | ОПИСАНИЕ                                                                                                                                                              |
|----------------------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `config_env.cmd`           | Установка переменных окружения и запуск необходимых bootstrap-скриптов. Вызывается из ниже перечисленных скриптов.                                                    |
| `install_dynamic_deps.cmd` | Установка динамически собранных зависимостей и копирование необходимых заголовочных файлов из `%programfiles%\ATOL\Drivers10\KKT\langs\cpp\fptr10` в `.\deps\fptr10`. |
| `install_static_deps.cmd`  | Установка статически собранных зависимостей и копирование необходимых заголовочных файлов из `%programfiles%\ATOL\Drivers10\KKT\langs\cpp\fptr10` в `.\deps\fptr10`.  |
| `config_build.cmd`         | Установка опций сборки.                                                                                                                                               |
| `test.cmd`                 | Сборка отладочной версии и запуск unit-тестов.                                                                                                                        |
| `build_debug.cmd`          | Сборка отладочной версии.                                                                                                                                             |
| `build_release.cmd`        | Сборка релизной версии.                                                                                                                                               |

Предполагается, что необходимое ПО установлено в следующих директориях:

| ПО                                             | ПУТЬ                                                    |
|------------------------------------------------|---------------------------------------------------------|
| АТОЛ Драйвер                                   | `%programfiles%\ATOL`                                   |
| Microsoft Visual Studio 2022 Community Edition | `%programfiles%\Microsoft Visual Studio\2022\Community` |
| Clang                                          | `C:\Devel\Platform\Clang\21.1.5-x86_64`                 |
| CMake                                          | `C:\Devel\Platform\CMake\4.1.2-x86_64`                  |
| Ninja                                          | `C:\Devel\Platform\Ninja\1.13.1`                        |

Пути можно изменить в файлах `config_env.cmd` и `install_*_deps.cmd`. CMake и Ninja можно использовать из состава MSVS.

Опции CMake:

| ОПЦИЯ             | ОПИСАНИЕ                                                              |
|-------------------|-----------------------------------------------------------------------|
| `BUILD_SEPARATED` | Сборка приложения как отдельных исполняемых файлов.                   |
| `BUILD_STATIC`    | Статическая сборка.                                                   |
| `WITH_CRTDBG`     | Профилирование памяти в отладочной сборке с использованием CRT Debug. |
| `WITH_LEAKS`      | Создание утечек памяти в отладочной сборке.                           |
| `WITH_RELSL`      | Использовать относительные пути исходных файлов в приложении.         |

<!-- | `WITH_SBIAC`      | Разрешить инвазивный доступ к буферу std::string (ересь).             | -->

После сборки одним из скриптов `build_*.cmd` в директорию `.\_build` будет установлен файл `kkmha.exe` и, в случае
динамической сборки, файлы `libcrypto-?-x64.dll`, `libssl-?-x64.dll`. После сборки с опцией `-D BUILD_SEPARATED=ON`,
будет создано 3 исполняемых файла: `kkmha.exe`, `kkmop.exe`, `kkmjl.exe`.

Так же в директории `.\_build` уже находятся файлы конфигурации и т.п., формирующие готовое окружение для запуска.

Для запуска на ПК, кроме того, на котором производилась сборка может потребоваться установка последнего пакета
[Microsoft Visual C++ Redistributable](https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170).

---
[Назад к содержанию](../README.md)
