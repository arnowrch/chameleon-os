# Chameleon OS - Installation & Deployment Instructions

## Step-by-Step Installation Guide

### Step 1: Install Prerequisites

**Install Python (if not already installed):**
- Download from: https://www.python.org/downloads/
- During installation, check "Add Python to PATH"
- Verify: Open Command Prompt and type `python --version`

**Install PlatformIO:**
Open Command Prompt (Windows) or Terminal (Mac/Linux) and run:
```bash
pip install platformio
```

Verify installation:
```bash
platformio --version
```

### Step 2: Download the Repository

If you have Git installed:
```bash
git clone https://github.com/arnowrch/chameleon-os.git
cd chameleon-os
```

Or download ZIP:
1. Go to: https://github.com/arnowrch/chameleon-os
2. Click "Code" → "Download ZIP"
3. Extract to a folder
4. Open Command Prompt in that folder

### Step 3: Connect Your ESP32-S3

1. Connect ESP32-S3 to your computer via USB
2. Check which COM port it's on:
   - **Windows**: Device Manager → Ports (COM & LPT)
   - **Mac/Linux**: Run `ls /dev/tty*` or `ls /dev/cu.*`
3. The project is pre-configured for **COM7**

### Step 4: Deploy Chameleon OS

**EASIEST METHOD - Just double-click:**

**Windows:**
1. Navigate to the `chameleon-os` folder
2. **Double-click `deploy.bat`**
3. Wait for build and upload to complete
4. Serial monitor opens automatically showing boot sequence

**Mac/Linux:**
1. Navigate to the `chameleon-os` folder
2. **Double-click `deploy.sh`** (or run `./deploy.sh` in Terminal)
3. Wait for build and upload to complete
4. Serial monitor opens automatically

---

**MANUAL METHOD - Command Line:**

**Windows (PowerShell or Command Prompt):**
```cmd
cd path\to\chameleon-os
platformio run --target upload
platformio device monitor
```

**Mac/Linux (Terminal):**
```bash
cd path/to/chameleon-os
platformio run --target upload
platformio device monitor
```

---

**IF YOUR ESP32 IS NOT ON COM7:**

Edit `platformio.ini` file:
1. Open `platformio.ini` in any text editor
2. Find the line: `upload_port = COM7`
3. Change `COM7` to your port (e.g., `COM3`, `/dev/ttyUSB0`, etc.)
4. Save the file
5. Run deployment again

### Step 5: Verify Deployment Success

After deployment, you should see in the serial monitor:

```
========================================
   Chameleon OS - Universal Embedded OS
   ESP32-S3 + 320x172 Touch Display
   Target: 60 FPS Luxury UX
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
   Entering Main Loop (60 FPS target)
========================================
```

**On the Display:**
- You should see the "Home" app screen
- Status bar at top showing "Home 1/33"
- Focus indicators (dots) at bottom
- Text: "Chameleon OS - Universal Embedded OS"

### Troubleshooting

**"platformio: command not found"**
- Restart your terminal/command prompt after installing PlatformIO
- Or use full path: `python -m platformio run --target upload`

**"Access denied" or "Permission denied" on COM7**
- Close any other programs using the port (Arduino IDE, serial monitors)
- On Windows: Run Command Prompt as Administrator
- On Linux: Add yourself to dialout group: `sudo usermod -a -G dialout $USER`

**"Device not responding" or stuck at "Connecting..."**
- Press and hold the **BOOT button** on your ESP32
- Click upload (or run the command)
- Release BOOT button when you see "Connecting..." or when upload starts

**Upload fails with timeout**
- Try a different USB cable (some cables are power-only)
- Try a different USB port
- Lower the upload speed in `platformio.ini`: change `upload_speed = 921600` to `upload_speed = 115200`

**Build errors or compilation fails**
- Make sure you have internet connection (needs to download ESP32 tools first time)
- Try: `platformio platform install espressif32`
- Delete `.pio` folder and try again

## Quick Reference

### Files You Need to Know

**To Deploy:**
- **deploy.bat** (Windows) - Double-click this file
- **deploy.sh** (Mac/Linux) - Double-click this file

**To Configure:**
- **platformio.ini** - Edit COM port here if not COM7

**For Help:**
- **DEPLOYMENT_GUIDE.md** - Complete deployment instructions
- **TESTING_CHECKLIST.md** - How to verify everything works
- **README.md** - Project overview

### Common Commands

**Build only (no upload):**
```bash
platformio run
```

**Upload to device:**
```bash
platformio run --target upload
```

**Open serial monitor:**
```bash
platformio device monitor
```

**Clean build (start fresh):**
```bash
platformio run --target clean
```

**Build + Upload + Monitor (all in one):**
```bash
platformio run --target upload && platformio device monitor
```

## What's Next?

After successful deployment:

1. **Test Navigation:**
   - Swipe left/right on the touch screen
   - Should cycle through all 33 apps
   - Apps include: Home, Clock, WiFi, Calendar, etc.

2. **Configure WiFi (Optional):**
   - Edit `src/main.cpp`
   - Update WiFi credentials (lines 38-42)
   - Rebuild and upload

3. **Explore Apps:**
   - Swipe through all 33 apps
   - Check the status bar showing "AppName X/33"
   - Watch the focus indicators change

4. **Monitor Performance:**
   - Serial monitor shows frame timing
   - Should maintain 60 FPS
   - No "Frame time exceeded" warnings

## Need More Help?

- **Complete guide:** See `DEPLOYMENT_GUIDE.md`
- **Testing procedures:** See `TESTING_CHECKLIST.md`
- **Architecture details:** See `MASTER_BLUEPRINT.md`
- **Developer guide:** See `AGENT_BRIEFING.md`

---

**You're all set! Just double-click `deploy.bat` (Windows) or `deploy.sh` (Mac/Linux) to get started!** 🚀
