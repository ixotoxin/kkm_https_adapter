@ECHO OFF
CLS
CALL "%~dp0config_env.cmd"
cmake -G Ninja -D CMAKE_CXX_COMPILER=clang++ -D CMAKE_BUILD_TYPE=Debug -D BUILD_STATIC=ON -B ../__temp/debug-static-clang -S ..
cmake --build ../__temp/debug-static-clang --config Debug --verbose
cmake --install ../__temp/debug-static-clang --prefix ../_build/ --config Debug --verbose
