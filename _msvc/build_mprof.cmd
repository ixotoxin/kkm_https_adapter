@ECHO OFF
CLS
CALL "%~dp0config_env.cmd"
cmake -G "Visual Studio 17 2022" -D CMAKE_BUILD_TYPE=Debug -D WITH_MEMORY_PROFILING=ON -D WITH_MEMORY_LEAKS=ON -B ../__temp/msvc_mprof -S ..
cmake --build ../__temp/msvc_mprof --config Debug --verbose
cmake --install ../__temp/msvc_mprof --prefix ../_build/ --config Debug --verbose
