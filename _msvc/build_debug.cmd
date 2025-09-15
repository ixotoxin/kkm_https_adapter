@ECHO OFF
CLS
CALL "%~dp0config_env.cmd"
CALL "%~dp0build_config.cmd"
cmake -G "Visual Studio 17 2022" -D CMAKE_BUILD_TYPE=Debug %DEBUG_OPTS% -B ../__temp/debug-msvc -S ..
cmake --build ../__temp/debug-msvc --config Debug --verbose
cmake --install ../__temp/debug-msvc --prefix ../_build/ --config Debug --verbose
