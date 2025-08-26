@ECHO OFF
CLS
CALL "%~dp0config_env.cmd"
cmake -G Ninja -D CMAKE_CXX_COMPILER=clang++ -D CMAKE_BUILD_TYPE=Release -B ../__temp/release-dynamic-clang -S ..
cmake --build ../__temp/release-dynamic-clang --config Release --verbose
cmake --install ../__temp/release-dynamic-clang --prefix ../_build/ --config Release --verbose
