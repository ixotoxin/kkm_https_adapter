@ECHO OFF
CLS
CALL "%~dp0config_env.cmd"
IF NOT EXIST "%~dp0..\deps\fptr10" (
    MKDIR "%~dp0..\deps\fptr10"
    XCOPY /S/E/H "%programfiles%\ATOL\Drivers10\KKT\langs\cpp\fptr10" "%~dp0..\deps\fptr10"
)
CALL %VCPKG_DIR%/bootstrap-vcpkg.bat
%VCPKG_DIR%/vcpkg --x-manifest-root=%~dp0..\deps install
