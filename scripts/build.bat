@echo off
REM ===========================================================================
REM  Build numericalCalculater with CMake + Ninja + MSVC (MFC).
REM  All paths are derived from the script location so the tree is relocatable.
REM ===========================================================================
setlocal

set "SCRIPT_DIR=%~dp0"
set "CODE_DIR=%SCRIPT_DIR%.."
set "PROJECT_DIR=%SCRIPT_DIR%..\.."

for %%I in ("%CODE_DIR%")    do set "CODE_DIR=%%~fI"
for %%I in ("%PROJECT_DIR%") do set "PROJECT_DIR=%%~fI"

set "BUILD_DIR=%PROJECT_DIR%\build_msvc"
set "CMAKE=D:\win10\cmake-4.3.2-windows-x86_64\bin\cmake.exe"
set "NINJA=D:\win10\ninja.exe"
set "BUILD_TYPE=%1"
if "%BUILD_TYPE%"=="" set "BUILD_TYPE=Release"

REM ---- Visual Studio environment (x64) for MSVC + MFC + rc.exe -------------
set "VCVARS=C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
if not exist "%VCVARS%" (
    echo [ERROR] vcvars64.bat not found at "%VCVARS%".
    exit /b 1
)
call "%VCVARS%" || exit /b 1

echo.
echo === Configuring (%BUILD_TYPE%) ===
"%CMAKE%" -S "%CODE_DIR%" -B "%BUILD_DIR%" -G Ninja ^
    -DCMAKE_MAKE_PROGRAM="%NINJA%" ^
    -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ^
    -DCMAKE_INSTALL_PREFIX="%PROJECT_DIR%\install" || exit /b 1

echo.
echo === Building ===
"%CMAKE%" --build "%BUILD_DIR%" || exit /b 1

echo.
echo === Build finished. Executable in %PROJECT_DIR%\msvc_release ===
endlocal
