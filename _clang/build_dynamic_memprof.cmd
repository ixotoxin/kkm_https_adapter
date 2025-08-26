@ECHO OFF
CLS
CALL "%~dp0config_env.cmd"
cmake -G Ninja -D CMAKE_CXX_COMPILER=clang++ -D CMAKE_BUILD_TYPE=Debug -D ENABLE_MEMORY_PROFILING=ON -D ENABLE_MEMORY_LEAKS=ON -B ../__temp/memprof-dynamic-clang -S ..
cmake --build ../__temp/memprof-dynamic-clang --config Debug --verbose
cmake --install ../__temp/memprof-dynamic-clang --prefix ../_build/ --config Debug --verbose
