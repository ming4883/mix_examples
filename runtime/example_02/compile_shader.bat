@echo off
set src=%1
set platform=%2

set shaderc="../../../mix/bin/shaderc.exe"
set base=../../src/example_02/shader
set includes="%base%/common;../../../mix/vendor/bgfx/src"

set outdir="%platform%/shader/"
if not exist %outdir% mkdir %outdir%

set vs_profile=
set fs_profile=

if %platform%==windows set vs_profile=-p vs_4_0
if %platform%==windows set fs_profile=-p ps_4_0

echo - %src% vs...
%shaderc% -f %base%/%src%/vs_main.sc -o %outdir%\%src%_vs_main.sb --platform %platform% -i %includes% --type vertex %vs_profile%
if %errorlevel% neq 0 exit /b %errorlevel%

echo - %src% fs...
%shaderc% -f %base%/%src%/fs_main.sc -o %outdir%\%src%_fs_main.sb --platform %platform% -i %includes% --type fragment %fs_profile%
if %errorlevel% neq 0 exit /b %errorlevel%
