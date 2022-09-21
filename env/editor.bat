@echo off

if NOT DEFINED proj_root ( call "%~dp0\shell.bat")
pushd "%proj_root%"

start gvim code\*.cpp code\*.h code\*.c

popd REM %proj_root
