#!/bin/bash
# Chameleon OS - Deployment Script
# Automatically detects serial port and deploys to ESP32-S3

set -e

echo ""
echo "========================================"
echo "   Chameleon OS Deployment"
echo "   Target: ESP32-S3"
echo "========================================"
echo ""

# Check if PlatformIO is installed
if ! command -v platformio &> /dev/null; then
    echo "[ERROR] PlatformIO not found!"
    echo ""
    echo "Please install PlatformIO:"
    echo "  pip install platformio"
    echo ""
    exit 1
fi

# Detect serial port
if [ -e /dev/ttyUSB0 ]; then
    PORT="/dev/ttyUSB0"
elif [ -e /dev/ttyACM0 ]; then
    PORT="/dev/ttyACM0"
elif [ -e /dev/cu.usbserial-* ]; then
    PORT=$(ls /dev/cu.usbserial-* | head -1)
else
    echo "[WARNING] Could not auto-detect serial port."
    echo "Using default configuration from platformio.ini"
    PORT=""
fi

if [ -n "$PORT" ]; then
    echo "Detected serial port: $PORT"
    echo ""
fi

echo "[1/3] Building firmware..."
echo ""
platformio run

echo ""
echo "[2/3] Uploading to ESP32..."
echo ""
if [ -n "$PORT" ]; then
    echo "IMPORTANT: If upload fails, press and hold BOOT button on ESP32,"
    echo "           then release when you see 'Connecting...'"
    echo ""
    platformio run --target upload --upload-port "$PORT"
else
    platformio run --target upload
fi

echo ""
echo "[3/3] Opening serial monitor..."
echo ""
echo "You should see the boot sequence and 'Chameleon OS Ready!' message."
echo "Press Ctrl+C to exit monitor."
echo ""
sleep 2

if [ -n "$PORT" ]; then
    platformio device monitor --port "$PORT"
else
    platformio device monitor
fi

echo ""
echo "========================================"
echo "   Deployment Complete!"
echo "========================================"
echo ""
