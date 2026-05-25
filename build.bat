@echo off
setlocal enabledelayedexpansion

rem Check if cl is already in PATH
where cl >nul 2>nul
if %errorlevel% equ 0 goto :compile

rem Try to find Visual Studio installation path using vswhere
set "VS_PATH="
for /f "usebackq tokens=*" %%i in (`"C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe" -latest -property installationPath`) do (
    set "VS_PATH=%%i"
)

if not defined VS_PATH (
    echo Error: Visual Studio installation not found.
    exit /b 1
)

rem Locate vcvarsall.bat or VsDevCmd.bat
set "VCVARS_BAT=%VS_PATH%\VC\Auxiliary\Build\vcvarsall.bat"
if not exist "%VCVARS_BAT%" (
    set "VCVARS_BAT=%VS_PATH%\Common7\Tools\VsDevCmd.bat"
)

if not exist "%VCVARS_BAT%" (
    echo Error: Could not find build environment script ^(vcvarsall.bat or VsDevCmd.bat^).
    exit /b 1
)

echo Setting up Visual Studio environment from: %VCVARS_BAT%
call "%VCVARS_BAT%" amd64 >nul

:compile
echo Compiling list_resolution.exe...
cl.exe /W4 /O2 /Fe:list_resolution.exe main.c User32.lib Gdi32.lib

if %errorlevel% equ 0 (
    echo Build Successful!
    echo Executable: list_resolution.exe
) else (
    echo Build Failed!
)
