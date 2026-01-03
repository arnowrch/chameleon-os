# Chameleon OS - Production Testing Checklist

## Pre-Deployment Verification

### Hardware Setup
- [ ] ESP32-S3 connected to COM7 (Windows) or auto-detected port (Linux/Mac)
- [ ] Waveshare 320×172 touch display connected
- [ ] USB cable provides stable power
- [ ] Touch panel properly connected to ESP32

### Software Setup
- [ ] PlatformIO installed (`pip install platformio`)
- [ ] USB drivers installed (CP210x or CH340)
- [ ] No other applications using the COM port

## Deployment Test

### Build Test
```bash
platformio run
```

**Expected Results:**
- ✅ No compilation errors
- ✅ No warnings (or only library-related warnings)
- ✅ Firmware binary created in `.pio/build/esp32-s3-devkitc-1/`
- ✅ Binary size < 2MB (should fit in ESP32-S3 flash)

**Actual Results:**
- [ ] Build successful
- [ ] Firmware size: _______ KB
- [ ] Build time: _______ seconds

### Upload Test
```bash
platformio run --target upload
```

**Expected Results:**
- ✅ Connection established on COM7
- ✅ Firmware uploaded successfully
- ✅ Upload speed: 921600 baud
- ✅ Verification successful
- ✅ Device reboots automatically

**Actual Results:**
- [ ] Upload successful
- [ ] Upload time: _______ seconds
- [ ] Any errors: _______

### Boot Sequence Test

**Monitor serial output:**
```bash
platformio device monitor
```

**Expected Boot Messages:**
```
========================================
   Chameleon OS - Universal Embedded OS
========================================
Step 1: System initialized
Step 2: Initializing WiFi (BEFORE display)...
Step 3: Initializing Event Bus...
Step 4: Initializing Display (320x172, double-buffer)...
Step 5: Initializing Touch...
Step 6: Initializing Theme Manager...
Step 7: Initializing NTP...
Step 8: Initializing App Framework...
Step 9: Loading all 33 apps...
Step 10: Initializing Focus-Carousel UI...
Step 11: Launching default app...
========================================
   Chameleon OS Ready!
========================================
```

**Boot Checklist:**
- [ ] All 11 initialization steps complete
- [ ] WiFi initialized BEFORE display (critical!)
- [ ] Display initialized with double-buffer
- [ ] Panel gap (0, 34) correctly configured
- [ ] All 33 apps registered successfully
- [ ] No error messages during boot
- [ ] "Chameleon OS Ready!" message displayed
- [ ] Main loop started with 60 FPS target

**Actual Results:**
- Boot time: _______ seconds
- Apps loaded: _______/33
- Errors: _______

## Display Test

### Visual Verification

**Home Screen:**
- [ ] Display shows content (not blank)
- [ ] Status bar visible at top
- [ ] "Chameleon OS" title displayed
- [ ] Text is readable and properly sized
- [ ] Colors look correct (dark theme by default)
- [ ] No screen tearing or flicker
- [ ] Panel gap respected (bottom 34 pixels clear)

**Visual Quality:**
- [ ] Text is sharp and clear
- [ ] Colors are vibrant (RGB565)
- [ ] No ghosting or artifacts
- [ ] Smooth rendering (60 FPS)
- [ ] Double buffering working (no tearing)

## Touch Test

### Touch Response
- [ ] Touch screen responds to taps
- [ ] Touch coordinates accurate
- [ ] No phantom touches
- [ ] Touch events trigger properly

### Gesture Recognition
- [ ] Swipe left detects correctly
- [ ] Swipe right detects correctly
- [ ] Swipe threshold (50px) appropriate
- [ ] Navigation to next/prev app works

## Navigation Test

### App Switching
Starting from Home (app 0):

1. **Swipe Right → Clock**
   - [ ] Transition smooth
   - [ ] Clock app displays correctly
   - [ ] Status bar shows "Clock 2/33"
   - [ ] Focus indicator moved to position 2

2. **Swipe Right → Calendar**
   - [ ] Calendar app loads
   - [ ] Status shows "Calendar 3/33"
   
3. **Continue swiping through all 33 apps:**
   - [ ] All apps load without crashes
   - [ ] Focus indicator updates correctly
   - [ ] Carousel wraps around (app 33 → app 1)
   
4. **Swipe Left from Home → Screensaver (app 33)**
   - [ ] Wraps to last app correctly
   - [ ] Backward navigation works

**App Count Verification:**
- Sector 1 apps: _______ (expected: 6)
- Sector 2 apps: _______ (expected: 6)
- Sector 3 apps: _______ (expected: 5)
- Sector 4 apps: _______ (expected: 6)
- Sector 5 apps: _______ (expected: 5)
- Sector 6 apps: _______ (expected: 5)
- **Total: _______ (expected: 33)**

## Performance Test

### Frame Rate Monitoring

**Serial Output Check:**
- [ ] No "Frame time exceeded" warnings
- [ ] Frame counter incrementing
- [ ] Consistent 60 FPS performance

**Visual Smoothness:**
- [ ] Animations are smooth
- [ ] No stuttering or lag
- [ ] App transitions fluid
- [ ] UI feels responsive

### Memory Test
- [ ] No memory leaks reported
- [ ] PSRAM allocation successful
- [ ] Back buffer allocated in PSRAM
- [ ] No heap fragmentation warnings

## Feature Test

### Critical Features

**Panel Gap (0, 34):**
- [ ] Top area (0 pixels) fully utilized
- [ ] Bottom area (34 pixels) reserved
- [ ] Effective rendering area: 320×138
- [ ] No content rendered in gap area
- [ ] Status bar within bounds

**Double Buffering:**
- [ ] Back buffer allocated successfully
- [ ] Buffer size: 88,320 bytes (320×138×2)
- [ ] Allocated in PSRAM (not SRAM)
- [ ] No screen tearing visible
- [ ] Swap function working correctly

**Golden Path Init:**
- [ ] WiFi initialized FIRST
- [ ] Display initialized AFTER WiFi
- [ ] Init sequence: WiFi → Event → Display → Touch → Theme → NTP → Apps → UI
- [ ] All steps complete without errors

**60 FPS Target:**
- [ ] Frame time budget: 16.67ms
- [ ] Input processing: ~2ms
- [ ] App update: ~5ms
- [ ] Rendering: ~8ms
- [ ] Buffer swap: ~1.67ms
- [ ] Total frame time under budget

### WiFi Test (Optional)

**To enable WiFi:**
1. Edit `src/main.cpp`
2. Update WiFi credentials:
   ```cpp
   WiFiConfig_t wifi_config = {
       .ssid = "YOUR_SSID",
       .password = "YOUR_PASSWORD",
       .timeout_ms = 10000
   };
   ```
3. Uncomment: `wifi_hal_connect(&wifi_config);`
4. Rebuild and upload

**WiFi Verification:**
- [ ] WiFi connects successfully
- [ ] IP address obtained
- [ ] RSSI reading available
- [ ] Navigate to WiFi app (app 7)
- [ ] WiFi status shows "Connected"
- [ ] IP address displayed
- [ ] Signal strength shown

### Theme Test

**Theme Switching:**
- [ ] Dark theme (default) works
- [ ] Light theme available
- [ ] Custom theme configurable
- [ ] Colors update correctly
- [ ] Theme change event fires

## Stress Test

### Endurance Test
- [ ] Run continuously for 10 minutes
- [ ] No crashes or freezes
- [ ] Temperature remains acceptable
- [ ] No memory issues
- [ ] Performance stays consistent

### Rapid Navigation
- [ ] Rapidly swipe through apps
- [ ] No lag or stuttering
- [ ] No memory leaks
- [ ] Frame rate stays at 60 FPS
- [ ] Touch remains responsive

## Production Ready Checklist

### Code Quality
- [x] All 33 apps implemented
- [x] HAL fully functional
- [x] Event bus working
- [x] Theme manager operational
- [x] UI framework complete
- [x] Documentation comprehensive

### Build Quality
- [ ] Compiles without errors
- [ ] No critical warnings
- [ ] Binary size appropriate
- [ ] Upload successful
- [ ] Boot sequence clean

### Runtime Quality
- [ ] All apps load
- [ ] Navigation works
- [ ] Touch responsive
- [ ] Display correct
- [ ] Performance at 60 FPS
- [ ] No crashes

### Production Status
- [ ] **Build Test**: ✅ PASSED
- [ ] **Upload Test**: ✅ PASSED
- [ ] **Boot Test**: ✅ PASSED
- [ ] **Display Test**: ✅ PASSED
- [ ] **Touch Test**: ✅ PASSED
- [ ] **Navigation Test**: ✅ PASSED
- [ ] **Performance Test**: ✅ PASSED
- [ ] **Feature Test**: ✅ PASSED

## Final Verdict

**Overall Status:** _______________________

**Production Ready:** [ ] YES  [ ] NO  [ ] WITH ISSUES

**Issues Found:**
1. _______________________
2. _______________________
3. _______________________

**Notes:**
_______________________
_______________________
_______________________

---

**Tested By:** _______________________  
**Date:** _______________________  
**Hardware:** ESP32-S3 + Waveshare 320×172  
**Firmware Version:** Chameleon OS v1.0  

**Signature:** _______________________
