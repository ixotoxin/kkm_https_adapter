@ECHO OFF
CLS
CALL "%~dp0config_env.cmd"
cmake -G "Visual Studio 17 2022" -D CMAKE_BUILD_TYPE=Debug -D BUILD_TESTS=ON -B ../__temp/tests-msvc -S ..
cmake --build ../__temp/tests-msvc --config Debug --verbose
ctest --test-dir ../__temp/tests-msvc --build-config Debug --rerun-failed --output-on-failure
