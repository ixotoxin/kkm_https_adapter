@ECHO OFF
CLS
CALL "%~dp0config_env.cmd"
@REM -D CMAKE_TOOLCHAIN_FILE=%VCPKG_TCS%
cmake -G "Visual Studio 17 2022" -D CMAKE_BUILD_TYPE=Debug -B ../__temp/msvc_debug -S ..
cmake --build ../__temp/msvc_debug --config Debug --verbose
cmake --install ../__temp/msvc_debug --prefix ../_build/ --config Debug --verbose
