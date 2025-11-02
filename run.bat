@echo off
setlocal
set MSYS2_PATH=C:\msys64\mingw64\bin
set PATH=%MSYS2_PATH%;%PATH%
if not exist build mkdir build
cd build
main.exe
endlocal