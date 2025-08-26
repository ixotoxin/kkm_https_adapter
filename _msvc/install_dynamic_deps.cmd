@ECHO OFF
CLS
CALL "%~dp0config_env.cmd"
IF NOT EXIST "%~dp0..\deps\fptr10" (
    MKDIR "%~dp0..\deps\fptr10"
    XCOPY /S/E/H "%programfiles%\ATOL\Drivers10\KKT\langs\cpp\fptr10" "%~dp0..\deps\fptr10"
)
CALL %VCPKG_DIR%\bootstrap-vcpkg.bat -disableMetrics
REM %VCPKG_DIR%\vcpkg --disable-metrics --triplet=x64-windows --vcpkg-root=%VCPKG_DIR% --x-buildtrees-root=%VCPKG_BLD% --x-manifest-root=%~dp0..\deps\dynamic install
%VCPKG_DIR%\vcpkg --disable-metrics --triplet=x64-windows --vcpkg-root=%VCPKG_DIR% --x-manifest-root=%~dp0..\deps\dynamic install
