@ECHO OFF
CLS
CALL "%~dp0config_env.cmd"
cmake -G "Visual Studio 17 2022" -D CMAKE_BUILD_TYPE=Debug -D ENABLE_MEMORY_PROFILING=ON -D ENABLE_MEMORY_LEAKS=ON -B ../__temp/memprof-dynamic-msvc -S ..
cmake --build ../__temp/memprof-dynamic-msvc --config Debug --verbose
cmake --install ../__temp/memprof-dynamic-msvc --prefix ../_build/ --config Debug --verbose
