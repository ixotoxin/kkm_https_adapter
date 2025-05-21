@ECHO OFF
IF NOT "%CLANG_DIR%" == "" GOTO CLANG_DIR_IS_SET
SET CLANG_DIR=C:\Devel\Platform\Clang\20.1.5-x86_64
SET CMAKE_DIR=C:\Devel\Platform\CMake\3.31.7-x86_64
SET NINJA_DIR=C:\Devel\Platform\Ninja\1.12.1
SET PATH=%CLANG_DIR%\bin;%CMAKE_DIR%\bin;%NINJA_DIR%;%PATH%
:CLANG_DIR_IS_SET
IF NOT EXIST "../__temp" MKDIR "../__temp"
IF NOT EXIST "../_build" MKDIR "../_build"
