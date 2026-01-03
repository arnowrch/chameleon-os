# Chameleon OS - Deployment Guide for COM7

## Quick Deployment

Your ESP32-S3 is connected on **COM7**. Follow these steps to deploy Chameleon OS:

### Prerequisites
- PlatformIO installed (`pip install platformio`)
- ESP32-S3 connected to COM7
- USB drivers installed for your ESP32

### Deployment Steps

1. **Build the firmware:**
   ```bash
   platformio run
   ```

2. **Upload to COM7:**
   ```bash
   platformio run --target upload
   ```

3. **Monitor serial output:**
   ```bash
   platformio device monitor
   ```

### One-Line Deployment
```bash
platformio run --target upload && platformio device monitor
```

This will:
- ✅ Build the firmware
- ✅ Upload to ESP32 on COM7
- ✅ Open serial monitor at 115200 baud
- ✅ Show boot sequence and app loading

## Expected Boot Sequence

```
========================================
   Chameleon OS - Universal Embedded OS
   ESP32-S3 + 320x172 Touch Display
   Target: 60 FPS Luxury UX
========================================

Step 1: System initialized
Step 2: Initializing WiFi (BEFORE display)...
WiFi HAL ready (connection optional)
Step 3: Initializing Event Bus...
Event bus initialized successfully
Step 4: Initializing Display (320x172, double-buffer)...
Display initialized: 320x172
Panel gap: top=0, bottom=34
Effective area: 320x138
Back buffer allocated: 88320 bytes in PSRAM
Display HAL initialized successfully
Step 5: Initializing Touch...
Touch HAL initialized successfully
Step 6: Initializing Theme Manager...
Theme manager initialized (default: DARK)
Step 7: Initializing NTP...
NTP HAL initialized successfully
Step 8: Initializing App Framework...
App framework initialized
Step 9: Loading all 33 apps...
Registered app #0: Home (sector 1)
Registered app #1: Clock (sector 1)
Registered app #2: Calendar (sector 1)
...
Loaded 33 apps
Step 10: Initializing Focus-Carousel UI...
Focus-Carousel UI initialized
Step 11: Launching default app...
Switching to app #0: Home

========================================
   Chameleon OS Ready!
   Entering Main Loop (60 FPS target)
========================================
```

## Troubleshooting

### "Access denied to COM7"
- Close any other programs using COM7 (Arduino IDE, serial monitors, etc.)
- Try running your terminal/command prompt as Administrator
- Check Device Manager to confirm COM7 is the correct port

### "Device not found"
- Press and hold BOOT button on ESP32
- Click Upload
- Release BOOT when "Connecting..." appears

### "Upload fails"
- Try lower upload speed: Add `upload_speed = 115200` to platformio.ini
- Use esptool manually:
  ```bash
  esptool.py --port COM7 write_flash 0x0 .pio/build/esp32-s3-devkitc-1/firmware.bin
  ```

### "Compilation errors"
- Ensure all dependencies are installed
- Try: `platformio lib install` to install required libraries
- Check that TFT_eSPI is properly configured

## Verify Deployment Success

After upload, you should see:
1. ✅ Serial output showing boot sequence
2. ✅ "Chameleon OS Ready!" message
3. ✅ Display shows Home app with "Chameleon OS" title
4. ✅ Status bar at top showing "Home 1/33"
5. ✅ Focus indicators (dots) at bottom
6. ✅ Frame counter updating (60 FPS)

## Post-Deployment Testing

### Test App Navigation
- Swipe left/right on touch screen
- Should cycle through 33 apps
- Focus indicator should move

### Test WiFi (Optional)
1. Navigate to WiFi app (swipe 6 times right)
2. Update WiFi credentials in `src/main.cpp`:
   ```cpp
   WiFiConfig_t wifi_config = {
       .ssid = "YourSSID",
       .password = "YourPassword",
       .timeout_ms = 10000
   };
   ```
3. Uncomment the connection line:
   ```cpp
   wifi_hal_connect(&wifi_config);
   ```
4. Rebuild and upload

### Monitor Performance
- Check serial monitor for frame time warnings
- Should see no "Frame time exceeded" messages
- All frames should complete in <16.67ms

## Production Ready Status

✅ **Build**: Compiles without errors  
✅ **Upload**: Configured for COM7  
✅ **Boot**: Golden Path initialization  
✅ **Display**: Double-buffered, panel gap handled  
✅ **Apps**: All 33 apps loaded and registered  
✅ **Performance**: 60 FPS target enforced  
✅ **UI**: Focus-Carousel navigation ready  

**Status**: Production-ready for deployment! 🚀

## Need Help?

Check the documentation:
- `README.md` - Quick start
- `MASTER_BLUEPRINT.md` - Architecture
- `AGENT_BRIEFING.md` - Developer guide
- `QUICK_REFERENCE.md` - API reference

---

**Your Chameleon OS is ready to transform your ESP32-S3 into 33 specialized apps!**
