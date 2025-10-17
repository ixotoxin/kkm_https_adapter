@ECHO OFF
CLS
CALL "%~dp0config_env.cmd"
CD %VCPKG_DIR%
REM Возвращаем vcpkg к последнему commit'у с версией openssl 3.5.4
git checkout 8bb030c
CD %~dp0
