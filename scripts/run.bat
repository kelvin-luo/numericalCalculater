@echo off
REM Run the built application from its runtime directory (so the relative
REM input/ and ../output paths resolve correctly).
setlocal
set "SCRIPT_DIR=%~dp0"
set "PROJECT_DIR=%SCRIPT_DIR%..\.."
for %%I in ("%PROJECT_DIR%") do set "PROJECT_DIR=%%~fI"

set "RUN_DIR=%PROJECT_DIR%\msvc_release"
if not exist "%RUN_DIR%\numericalCalculater.exe" (
    echo [ERROR] Executable not found. Build first with scripts\build.bat
    exit /b 1
)
pushd "%RUN_DIR%"
start "" "numericalCalculater.exe"
popd
endlocal
