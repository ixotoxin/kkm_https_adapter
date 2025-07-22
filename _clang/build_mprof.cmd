@ECHO OFF
CLS
CALL "%~dp0config_env.cmd"
cmake -G Ninja -D CMAKE_CXX_COMPILER=clang++ -D CMAKE_BUILD_TYPE=Debug -D WITH_MEMORY_PROFILING=ON -D WITH_MEMORY_LEAKS=ON -B ../__temp/mprof-clang -S ..
cmake --build ../__temp/mprof-clang --config Debug --verbose
cmake --install ../__temp/mprof-clang --prefix ../_build/ --config Debug --verbose
