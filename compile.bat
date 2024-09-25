@echo off
rem This batch file runs the Makefile
echo Compiling project using Makefile...

rem Run the make command
cd ./bin/
mingw32-make

rem Check if make was successful
if %errorlevel% neq 0 (
    echo Compilation failed!
    pause
    exit /b %errorlevel%
) else (
    echo Compilation successful!
)
