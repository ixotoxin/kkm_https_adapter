@ECHO OFF
IF NOT "%MSVC_DIR%" == "" GOTO MSVC_DIR_IS_SET
SET MSVC_DIR=C:\Program Files\Microsoft Visual Studio\2022\Community
SET CMAKE_DIR=%MSVC_DIR%\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake
SET NINJA_DIR=%MSVC_DIR%\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja
SET VCPKG_DIR=C:\Devel\Platform\vcpkg
SET VCPKG_TCS=%VCPKG_DIR%\scripts\buildsystems\vcpkg.cmake
SET PATH=%CMAKE_DIR%\bin;%NINJA_DIR%;%VCPKG_DIR%;%PATH%
CALL "%MSVC_DIR%\VC\Auxiliary\Build\vcvars64.bat"
:MSVC_DIR_IS_SET
IF NOT EXIST "..\__temp" MKDIR "..\__temp"
IF NOT EXIST "..\_build" MKDIR "..\_build"
