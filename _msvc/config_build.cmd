@ECHO OFF

REM Сборка приложения как отдельных исполняемых файлов
SET SEPARATED=OFF

REM Статическая сборка
SET STATIC=OFF

REM Сомнительные оптимизации
SET HACKS=OFF

REM Профилирование памяти в отладочной сборке
SET MEMORY_PROFILING=ON

REM Создание утечек памяти в отладочной сборке
SET MEMORY_LEAKS=ON

SET RELEASE_OPTS=-D BUILD_SEPARATED=%SEPARATED% -D BUILD_STATIC=%STATIC% -D ENABLE_HACKS=%HACKS%
SET DEBUG_OPTS=%RELEASE_OPTS% -D ENABLE_MEMORY_PROFILING=%MEMORY_PROFILING% -D ENABLE_MEMORY_LEAKS=%MEMORY_LEAKS%
