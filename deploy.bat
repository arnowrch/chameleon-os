@echo off
REM Chameleon OS - Deployment Script for Windows (COM7)
REM Run this script to build and upload to ESP32-S3

echo.
echo ========================================
echo    Chameleon OS Deployment
echo    Target: ESP32-S3 on COM7
echo ========================================
echo.

REM Check if PlatformIO is installed
where platformio >nul 2>nul
if %errorlevel% neq 0 (
    echo [ERROR] PlatformIO not found!
    echo.
    echo Please install PlatformIO:
    echo   pip install platformio
    echo.
    pause
    exit /b 1
)

echo [1/3] Building firmware...
echo.
platformio run
if %errorlevel% neq 0 (
    echo.
    echo [ERROR] Build failed!
    echo Check the error messages above.
    pause
    exit /b 1
)

echo.
echo [2/3] Uploading to COM7...
echo.
echo IMPORTANT: If upload fails, press and hold BOOT button on ESP32,
echo            then release when you see "Connecting..."
echo.
platformio run --target upload
if %errorlevel% neq 0 (
    echo.
    echo [ERROR] Upload failed!
    echo.
    echo Troubleshooting:
    echo - Make sure COM7 is the correct port (check Device Manager)
    echo - Close any other programs using COM7
    echo - Try holding BOOT button during upload
    echo - Run this script as Administrator
    echo.
    pause
    exit /b 1
)

echo.
echo [3/3] Opening serial monitor...
echo.
echo You should see the boot sequence and "Chameleon OS Ready!" message.
echo Press Ctrl+C to exit monitor.
echo.
timeout /t 2 /nobreak >nul
platformio device monitor

echo.
echo ========================================
echo    Deployment Complete!
echo ========================================
echo.
pause
