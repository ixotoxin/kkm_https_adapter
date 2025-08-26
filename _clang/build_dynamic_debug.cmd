@ECHO OFF
CLS
CALL "%~dp0config_env.cmd"
cmake -G Ninja -D CMAKE_CXX_COMPILER=clang++ -D CMAKE_BUILD_TYPE=Debug -B ../__temp/debug-dynamic-clang -S ..
cmake --build ../__temp/debug-dynamic-clang --config Debug --verbose
cmake --install ../__temp/debug-dynamic-clang --prefix ../_build/ --config Debug --verbose
