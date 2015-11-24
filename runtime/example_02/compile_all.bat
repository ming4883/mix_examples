@echo off
cls
call compile_platform windows
call compile_platform android
call compile_platform ios
call compile_platform osx

echo Packing ios/runtime.zip...
"../../../mix/bin/cscs.exe" ../../../mix/src/devtool/Zip.cs ios/runtime.zip ios common

echo Packing osx/runtime.zip...
"../../../mix/bin/cscs.exe" ../../../mix/src/devtool/Zip.cs osx/runtime.zip osx common
