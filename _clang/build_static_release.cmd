@ECHO OFF
CLS
CALL "%~dp0config_env.cmd"
cmake -G Ninja -D CMAKE_CXX_COMPILER=clang++ -D CMAKE_BUILD_TYPE=Release -D BUILD_STATIC=ON -B ../__temp/release-static-clang -S ..
cmake --build ../__temp/release-static-clang --config Release --verbose
cmake --install ../__temp/release-static-clang --prefix ../_build/ --config Release --verbose
