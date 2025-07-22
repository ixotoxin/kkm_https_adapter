@ECHO OFF
CLS
CALL "%~dp0config_env.cmd"
cmake -G "Visual Studio 17 2022" -D CMAKE_BUILD_TYPE=Debug -D WITH_MEMORY_PROFILING=ON -D WITH_MEMORY_LEAKS=ON -B ../__temp/mprof-msvc -S ..
cmake --build ../__temp/mprof-msvc --config Debug --verbose
cmake --install ../__temp/mprof-msvc --prefix ../_build/ --config Debug --verbose
