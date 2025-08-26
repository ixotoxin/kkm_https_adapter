@ECHO OFF
CLS
CALL "%~dp0config_env.cmd"
cmake -G "Visual Studio 17 2022" -D CMAKE_BUILD_TYPE=Release -B ../__temp/release-dynamic-msvc -S ..
cmake --build ../__temp/release-dynamic-msvc --config Release --verbose
cmake --install ../__temp/release-dynamic-msvc --prefix ../_build/ --config Release --verbose
