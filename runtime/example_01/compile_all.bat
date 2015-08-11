@echo off
cls
call compile_platform windows
call compile_platform android
call compile_platform ios
call compile_platform osx
