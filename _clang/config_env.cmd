@ECHO OFF
IF NOT "%CLANG_DIR%" == "" GOTO CLANG_DIR_IS_SET
SET CLANG_DIR=C:\Devel\Platform\Clang\21.1.1-x86_64
SET CLANG_XSAN_LIB_DIR=%CLANG_DIR%\lib\clang\21\lib\windows
SET CMAKE_DIR=C:\Devel\Platform\CMake\3.31.7-x86_64
SET NINJA_DIR=C:\Devel\Platform\Ninja\1.13.1
SET VCPKG_DIR=C:\Devel\Platform\vcpkg
SET VCPKG_BLD=C:\Devel\Platform\vcpkg\buildtrees
SET VCPKG_TCS=%VCPKG_DIR%\scripts\buildsystems\vcpkg.cmake
SET PATH=%NINJA_DIR%;%CMAKE_DIR%\bin;%CLANG_DIR%\bin;%PATH%
:CLANG_DIR_IS_SET
IF NOT EXIST "%~dp0../__temp" MKDIR "%~dp0../__temp"
IF NOT EXIST "%~dp0../_build" MKDIR "%~dp0../_build"
