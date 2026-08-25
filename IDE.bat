@echo off
title ~!@#$% IDE (Light)
color 0A

:: 自动查找第一个 .exe 文件作为解释器
set "INTERPRETER="
for %%f in (*.exe) do (
    set "INTERPRETER=%%f"
    goto found
)
:found
if "%INTERPRETER%"=="" (
    echo [Error] No .exe file found in current directory.
    echo Please place the interpreter executable here.
    pause
    exit /b 1
)

:: 如果文件被拖放至此，直接运行
if "%~1" neq "" goto runfile_drop

:menu
cls
echo ============================
echo   ~!@#$% IDE (Light)
echo   (c) Creative Gears
echo ============================
echo  1. New File
echo  2. Open File
echo  3. Run File
echo  4. Examples
echo  5. Exit
echo ============================
set /p choice="Select (1-5): "

if "%choice%"=="1" goto newfile
if "%choice%"=="2" goto openfile
if "%choice%"=="3" goto runfile
if "%choice%"=="4" goto examples
if "%choice%"=="5" exit
goto menu

:newfile
set /p fname="Enter file name (without extension): "
if "%fname%"=="" goto menu
echo. > "%fname%.~"
echo File created: %fname%.~
notepad "%fname%.~"
goto menu

:openfile
echo Current .~ files:
dir *.~ 2>nul
echo.
set /p fname="Enter file name (with extension): "
if exist "%fname%" (
    notepad "%fname%"
) else (
    echo File not found!
    pause
)
goto menu

:runfile
echo Current .~ files:
dir *.~ 2>nul
echo.
set /p fname="Enter file name (with extension): "
if exist "%fname%" (
    echo Running %fname% ...
    echo ============================
    "%INTERPRETER%" "%fname%"
    echo ============================
    echo Run finished.
) else (
    echo File not found!
)
pause
goto menu

:runfile_drop
echo Running %~1 ...
echo ============================
"%INTERPRETER%" "%~1"
echo ============================
echo Run finished.
pause
exit

:examples
if not exist "examples" (
    echo Examples directory not found, creating...
    mkdir examples
)
echo Examples directory:
dir examples\*.~ 2>nul
echo.
echo You can copy example files to current directory.
pause
goto menu