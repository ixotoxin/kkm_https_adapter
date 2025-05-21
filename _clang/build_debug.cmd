@ECHO OFF
CLS
CALL "%~dp0config_env.cmd"
cmake -G Ninja -D CMAKE_CXX_COMPILER=clang++ -D CMAKE_BUILD_TYPE=Debug -B ../__temp/clang_debug -S ..
cmake --build ../__temp/clang_debug --config Debug --verbose
cmake --install ../__temp/clang_debug --prefix ../_build/ --config Debug --verbose
