@ECHO OFF
CLS
CALL "%~dp0config_env.cmd"
cmake -G "Visual Studio 17 2022" -D CMAKE_BUILD_TYPE=Debug -B ../__temp/debug-dynamic-msvc -S ..
cmake --build ../__temp/debug-dynamic-msvc --config Debug --verbose
cmake --install ../__temp/debug-dynamic-msvc --prefix ../_build/ --config Debug --verbose
