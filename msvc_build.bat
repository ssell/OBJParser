@echo off
setlocal enabledelayedexpansion

 REM Simple batch script to build all OBJParser library binaries
 REM for a specific architecture/visual studio combination.
 REM 
 REM Takes in two parameters:
 REM
 REM     arch: Architecture type. Valid values: x86 or x64
 REM     msvc: Visual studio version. Value values: 120 or 140
 REM
 REM Example usage:
 REM
 REM     > msvc_build.bat x86 120

set arch=%1
set msvc=%2

set PATHMSVC120="C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat"
set PATHMSVC140="C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat"
set PATHMSVC=""
set PATHPROJECT=""
set PATHPROJECT120="projects\vs2013\OBJParser.sln"
set PATHPROJECT140="projects\vs2015\OBJParser.sln"

 REM ---------------------------------------------------------------------
 REM Verify Architecture Parameter
 REM ---------------------------------------------------------------------

echo.

if "%arch%" == "x86" goto valid_arch
if "%arch%" == "x64" goto valid_arch 

echo Error: Invalid architecture specified. Expected 'x86' or 'x64'
goto eof

 REM ---------------------------------------------------------------------
 REM Verify Visual Studio Parameter
 REM ---------------------------------------------------------------------

:valid_arch

if "%msvc%" == "120" (
    set PATHMSVC=!PATHMSVC120!
    set PATHPROJECT=!PATHPROJECT120!
    goto valid_msvc
    )

if "%msvc%" == "140" (
    set PATHMSVC=!PATHMSVC140!
    set PATHPROJECT=!PATHPROJECT140!
    goto valid_msvc
    )

echo Error: Invalid Visual Studio specfied. Expected '120' or '140'
goto eof

 REM ---------------------------------------------------------------------
 REM Confirm Visual Studio Path
 REM ---------------------------------------------------------------------

:valid_msvc

if exist !PATHMSVC! goto valid_msvc_path

echo Error: Visual Studio executable not found. Expected at !PATHMSVC!
goto eof

 REM ---------------------------------------------------------------------
 REM Run Visual Studio
 REM ---------------------------------------------------------------------

:valid_msvc_path

echo ] Arch: !arch!
echo ] MSVC: !msvc!
echo ] Path: !PATHMSVC!

echo ]

call !PATHMSVC! !arch!

 REM Build Debug no MemMap

echo ] Building Debug no MemMap ...

if "%arch%" == "x86" devenv !PATHPROJECT! /build "Debug|Win32"
if "%arch%" == "x64" devenv !PATHPROJECT! /build "Debug|x64"

echo ] ... Complete

 REM Build Debug w/ MemMap

echo ] Building Debug w/ MemMap ...

if "%arch%" == "x86" devenv !PATHPROJECT! /build "DebugMemMap|Win32"
if "%arch%" == "x64" devenv !PATHPROJECT! /build "DebugMemMap|x64"

echo ] ... Complete

 REM Build Release no MemMap

echo ] Building Release

if "%arch%" == "x86" devenv !PATHPROJECT! /build "Release|Win32"
if "%arch%" == "x64" devenv !PATHPROJECT! /build "Release|x64"

echo ] ... Complete

 REM Build Release w/ MemMap

echo ] Building Release w/ MemMap ...

if "%arch%" == "x86" devenv !PATHPROJECT! /build "ReleaseMemMap|Win32"
if "%arch%" == "x64" devenv !PATHPROJECT! /build "ReleaseMemMap|x64"

echo ] ... Complete

 REM ---------------------------------------------------------------------

:eof
echo.