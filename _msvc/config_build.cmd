@ECHO OFF

REM Сборка приложения как отдельных исполняемых файлов
SET SEPARATED=OFF

REM Статическая сборка
SET STATIC=OFF

REM Сомнительные оптимизации
SET HACKS=OFF

REM !!! НЕ ИСПОЛЬЗУЕТСЯ В ДАННЫЙ МОМЕНТ !!!
REM Профилирование памяти в отладочной сборке с использованием AddressSanitizer
REM SET ASAN=OFF

REM !!! НЕ ИСПОЛЬЗУЕТСЯ В ДАННЫЙ МОМЕНТ !!!
REM Сборка с UndefinedBehaviorSanitizer
REM SET UBSAN=OFF

REM Профилирование памяти в отладочной сборке с использованием CRT Debug
SET CRTDBG=OFF

REM Создание утечек памяти в отладочной сборке
SET LEAKS=%CRTDBG%

SET RELEASE_OPTS=-D BUILD_SEPARATED=%SEPARATED% -D BUILD_STATIC=%STATIC% -D ENABLE_HACKS=%HACKS%
SET DEBUG_OPTS=%RELEASE_OPTS% -D WITH_ASAN=%ASAN% -D WITH_UBSAN=%UBSAN% -D WITH_CRTDBG=%CRTDBG% -D WITH_LEAKS=%LEAKS%
