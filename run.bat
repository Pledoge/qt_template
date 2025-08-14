@echo off
setlocal enabledelayedexpansion

echo ========================================
echo Qt Project Auto Runner
echo ========================================

set "PROJECT_NAME=qt_project"
set "BUILD_DIR=build"

:: 检查build目录是否存在
if not exist "%BUILD_DIR%" (
    echo Error: Build directory '%BUILD_DIR%' not found!
    echo Please run cmake build first.
    pause
    exit /b 1
)

:: 寻找可执行文件
set "EXECUTABLE_FOUND=0"
set "EXECUTABLE_PATH="

:: 检查常见的构建配置目录
for %%d in (Debug Release RelWithDebInfo MinSizeRel) do (
    if exist "%BUILD_DIR%\%%d\%PROJECT_NAME%.exe" (
        set "EXECUTABLE_PATH=%BUILD_DIR%\%%d\%PROJECT_NAME%.exe"
        set "EXECUTABLE_FOUND=1"
        set "BUILD_TYPE=%%d"
        goto :found
    )
)

:: 检查build根目录
if exist "%BUILD_DIR%\%PROJECT_NAME%.exe" (
    set "EXECUTABLE_PATH=%BUILD_DIR%\%PROJECT_NAME%.exe"
    set "EXECUTABLE_FOUND=1"
    set "BUILD_TYPE=Unknown"
    goto :found
)

:found
if "!EXECUTABLE_FOUND!"=="0" (
    echo Error: Executable '%PROJECT_NAME%.exe' not found in build directory!
    echo Searched in:
    echo   - %BUILD_DIR%\Debug\
    echo   - %BUILD_DIR%\Release\
    echo   - %BUILD_DIR%\RelWithDebInfo\
    echo   - %BUILD_DIR%\MinSizeRel\
    echo   - %BUILD_DIR%\
    pause
    exit /b 1
)

echo Found executable: !EXECUTABLE_PATH!
echo Build type: !BUILD_TYPE!
echo.
echo Starting application...
echo ========================================

:: 运行可执行文件
"!EXECUTABLE_PATH!"

echo.
echo ========================================
echo Application finished.