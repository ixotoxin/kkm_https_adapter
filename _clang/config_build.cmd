@ECHO OFF

REM Сборка приложения как отдельных исполняемых файлов
SET SEPARATED=OFF

REM Статическая сборка
SET STATIC=OFF

REM !!! НЕ РЕАЛИЗОВАНО !!!
REM Профилирование памяти в отладочной сборке с использованием AddressSanitizer
REM SET ASAN=OFF

REM !!! НЕ РЕАЛИЗОВАНО !!!
REM Сборка с UndefinedBehaviorSanitizer
REM SET UBSAN=OFF

REM Профилирование памяти в отладочной сборке с использованием CRT Debug
SET CRTDBG=OFF

REM Создание утечек памяти в отладочной сборке
SET LEAKS=%CRTDBG%

REM Использовать относительные пути исходных файлов в приложении
SET RELSL=ON

REM Разрешить инвазивный доступ к буферу std::string (ересь)
SET SSIAC=OFF

SET RELEASE_OPTS=-D BUILD_SEPARATED=%SEPARATED% -D BUILD_STATIC=%STATIC% -D WITH_RELSL=%RELSL% -D WITH_SSIAC=%SSIAC%
SET DEBUG_OPTS=%RELEASE_OPTS% -D WITH_ASAN=%ASAN% -D WITH_UBSAN=%UBSAN% -D WITH_CRTDBG=%CRTDBG% -D WITH_LEAKS=%LEAKS%
