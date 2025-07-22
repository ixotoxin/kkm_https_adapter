@ECHO OFF
CLS
CALL "%~dp0config_env.cmd"
cmake -G Ninja -D CMAKE_CXX_COMPILER=clang++ -D CMAKE_BUILD_TYPE=Debug -B ../__temp/debug-clang -S ..
cmake --build ../__temp/debug-clang --config Debug --verbose
cmake --install ../__temp/debug-clang --prefix ../_build/ --config Debug --verbose
