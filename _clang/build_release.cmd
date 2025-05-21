@ECHO OFF
CLS
CALL "%~dp0config_env.cmd"
cmake -G Ninja -D CMAKE_CXX_COMPILER=clang++ -D CMAKE_BUILD_TYPE=Release -B ../__temp/clang_release -S ..
cmake --build ../__temp/clang_release --config Release --verbose
cmake --install ../__temp/clang_release --prefix ../_build/ --config Release --verbose
