@echo off

:: Check if winget is installed 
winget -v >nul 2>&1
if %errorlevel% neq 0 (
    echo ERROR: Winget is not installed. Please install it manually.
    exit /b
)

:: Install qt6
echo Installing Qt6...
winget install -e --id QtGroup.Qt --accept-package-agreements --accept-source-agreements

:: Install cmake
echo Installing CMake...
winget install -e --id Kitware.CMake --accept-package-agreements --accept-source-agreements

:: Install mingw
echo Installing MinGW...
winget install -e --id MSYS2.MSYS2 --accept-package-agreements --accept-source-agreements

pause

