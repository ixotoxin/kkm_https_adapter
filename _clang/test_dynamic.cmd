@ECHO OFF
CLS
CALL "%~dp0config_env.cmd"
cmake -G Ninja -D CMAKE_CXX_COMPILER=clang++ -D CMAKE_BUILD_TYPE=Debug -D BUILD_TESTS=ON -B ../__temp/tests-dynamic-clang -S ..
cmake --build ../__temp/tests-dynamic-clang --config Debug --verbose
ctest --test-dir ../__temp/tests-dynamic-clang --build-config Debug --rerun-failed --output-on-failure
