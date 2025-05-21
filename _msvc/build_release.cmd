@ECHO OFF
CLS
CALL "%~dp0config_env.cmd"
cmake -G "Visual Studio 17 2022" -D CMAKE_BUILD_TYPE=Release -B ../__temp/msvc_release -S ..
cmake --build ../__temp/msvc_release --config Release --verbose
cmake --install ../__temp/msvc_release --prefix ../_build/ --config Release --verbose
