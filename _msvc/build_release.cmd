@ECHO OFF
CLS
CALL "%~dp0config_env.cmd"
CALL "%~dp0config_build.cmd"
cmake -G "Visual Studio 17 2022" -D CMAKE_BUILD_TYPE=Release %RELEASE_OPTS% -B ../__temp/release-msvc -S ..
cmake --build ../__temp/release-msvc --config Release --verbose
cmake --install ../__temp/release-msvc --prefix ../_build/ --config Release --verbose
