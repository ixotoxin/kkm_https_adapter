@ECHO OFF
CLS
CALL "%~dp0config_env.cmd"
cmake -G Ninja -D CMAKE_CXX_COMPILER=clang++ -D CMAKE_BUILD_TYPE=Debug -D BUILD_STATIC=ON -D ENABLE_MEMORY_PROFILING=ON -D ENABLE_MEMORY_LEAKS=ON -B ../__temp/memprof-static-clang -S ..
cmake --build ../__temp/memprof-static-clang --config Debug --verbose
cmake --install ../__temp/memprof-static-clang --prefix ../_build/ --config Debug --verbose
