@echo off
set platform=%1

if not exist %platform% mkdir %platform%

echo compiling for %platform%...

call compile_shader displayuv %platform%
call compile_shader displaytex %platform%
