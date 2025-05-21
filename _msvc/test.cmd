@ECHO OFF
CLS
CALL "%~dp0config_env.cmd"
@REM cmake -G "Visual Studio 17 2022" -D CMAKE_BUILD_TYPE=Debug -D BUILD_STATIC=ON -D BUILD_TESTS=ON -B ../__temp/msvc_tests -S ..
cmake -G "Visual Studio 17 2022" -D CMAKE_BUILD_TYPE=Debug -D BUILD_TESTS=ON -B ../__temp/msvc_tests -S ..
cmake --build ../__temp/msvc_tests --config Debug --verbose
ctest --test-dir ../__temp/msvc_tests --build-config Debug --rerun-failed --output-on-failure
