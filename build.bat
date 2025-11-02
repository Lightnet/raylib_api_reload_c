@echo off
setlocal
set MSYS2_PATH=C:\msys64\mingw64\bin
set PATH=%MSYS2_PATH%;%PATH%
if not exist build mkdir build
cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=%MSYS2_PATH%\gcc.exe -DCMAKE_CXX_COMPILER=%MSYS2_PATH%\g++.exe
cmake --build . --config Debug
endlocal