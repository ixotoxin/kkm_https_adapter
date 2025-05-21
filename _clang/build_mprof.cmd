@ECHO OFF
CLS
CALL "%~dp0config_env.cmd"
cmake -G Ninja -D CMAKE_CXX_COMPILER=clang++ -D CMAKE_BUILD_TYPE=Debug -D WITH_MEMORY_PROFILING=ON -D WITH_MEMORY_LEAKS=ON -B ../__temp/clang_mprof -S ..
cmake --build ../__temp/clang_mprof --config Debug --verbose
cmake --install ../__temp/clang_mprof --prefix ../_build/ --config Debug --verbose
