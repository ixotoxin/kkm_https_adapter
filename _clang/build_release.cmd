@ECHO OFF
CLS
CALL "%~dp0config_env.cmd"
CALL "%~dp0build_config.cmd"
cmake -G Ninja -D CMAKE_CXX_COMPILER=clang++ -D CMAKE_BUILD_TYPE=Release %RELEASE_OPTS% -B ../__temp/release-clang -S ..
cmake --build ../__temp/release-clang --config Release --verbose
cmake --install ../__temp/release-clang --prefix ../_build/ --config Release --verbose
