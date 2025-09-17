@REM Сборка приложения как отдельных исполняемых файлов
@SET SEPARATED=OFF

@REM Статическая сборка
@SET STATIC=OFF

@REM Сомнительные оптимизации
@SET HACKS=OFF

@REM Профилирование памяти в отладочной сборке
@SET MEMORY_PROFILING=OFF

@REM Создание утечек памяти в отладочной сборке
@SET MEMORY_LEAKS=OFF

@REM Сборка с AddressSanitizer
@SET WITH_ASAN=OFF

@REM Сборка с UndefinedBehaviorSanitizer
@SET WITH_UBSAN=OFF

@SET RELEASE_OPTS=-D BUILD_SEPARATED=%SEPARATED% -D BUILD_STATIC=%STATIC% -D ENABLE_HACKS=%HACKS% -D WITH_ASAN=%WITH_ASAN% -D WITH_UBSAN=%WITH_UBSAN%
@SET DEBUG_OPTS=%RELEASE_OPTS% -D ENABLE_MEMORY_PROFILING=%MEMORY_PROFILING% -D ENABLE_MEMORY_LEAKS=%MEMORY_LEAKS%
