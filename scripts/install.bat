@echo off
REM Install numericalCalculater into %PROJECT_DIR%\install
setlocal
set "SCRIPT_DIR=%~dp0"
set "CODE_DIR=%SCRIPT_DIR%.."
set "PROJECT_DIR=%SCRIPT_DIR%..\.."
for %%I in ("%CODE_DIR%")    do set "CODE_DIR=%%~fI"
for %%I in ("%PROJECT_DIR%") do set "PROJECT_DIR=%%~fI"

set "BUILD_DIR=%PROJECT_DIR%\build_msvc"
set "CMAKE=D:\win10\cmake-4.3.2-windows-x86_64\bin\cmake.exe"
set "VCVARS=C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat"

if not exist "%BUILD_DIR%" (
    echo [ERROR] Build directory not found. Run scripts\build.bat first.
    exit /b 1
)
call "%VCVARS%" >nul 2>&1

echo === Installing to %PROJECT_DIR%\install ===
"%CMAKE%" --install "%BUILD_DIR%" --prefix "%PROJECT_DIR%\install" || exit /b 1

REM Copy docs alongside the install tree.
if exist "%CODE_DIR%\docs" (
    xcopy /E /I /Y "%CODE_DIR%\docs" "%PROJECT_DIR%\install\docs" >nul
)
echo === Install finished ===
endlocal
