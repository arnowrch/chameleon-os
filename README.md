# Chameleon OS

A universal OS for ESP32-S3 with Touch-Display - 33 specialized apps from one device

## Overview

Chameleon OS is a production-ready embedded operating system for ESP32-S3 microcontrollers with a Waveshare 320×172 touch display. It transforms a single device into 33 specialized applications, organized across 6 sectors, with luxury UX at 60 FPS.

## Key Features

- **33 Apps in One Device**: Complete app ecosystem across 6 sectors
- **60 FPS Performance**: Double-buffered rendering with optimized frame timing
- **Focus-Carousel UI**: Innovative horizontal swipe navigation
- **Hardware Abstraction Layer**: Clean separation of hardware and software
- **Event-Driven Architecture**: Pub/sub event bus for decoupled components
- **Dynamic Theming**: Dark, light, and custom themes
- **Golden Path Initialization**: WiFi before display for optimal boot time

## Architecture

### Hardware Platform
- **MCU**: ESP32-S3 (Dual-core, 240MHz, with PSRAM)
- **Display**: Waveshare 320×172 LCD with Touch Panel
- **Panel Gap**: Top=0, Bottom=34 (effective area: 320×138)

### Core Systems
1. **HAL**: Display, Touch, WiFi, NTP
2. **Event Bus**: Publish-subscribe event system
3. **Theme Manager**: Dynamic theming engine
4. **UI Framework**: Focus-Carousel navigation
5. **App Framework**: App lifecycle management

### 33 Apps Across 6 Sectors

**Sector 1: Productivity** (6 apps)
- Home, Clock, Calendar, Notes, Tasks, Timer

**Sector 2: Communication** (6 apps)
- WiFi, Weather, News, Email, Messenger, Social

**Sector 3: Media** (5 apps)
- Music, Photos, Video, Radio, Podcast

**Sector 4: Tools** (6 apps)
- Files, Settings, Monitor, Terminal, Editor, Packages

**Sector 5: Lifestyle** (5 apps)
- Fitness, Health, Sleep, Meditation, Habits

**Sector 6: Games** (5 apps)
- Puzzle, Retro, Drawing, Animations, Screensaver

## Quick Start

### Prerequisites
- PlatformIO installed (`pip install platformio`)
- ESP32-S3 development board
- Waveshare 320×172 touch display

### Quick Deployment (Windows with COM7)

**Using deployment script:**
```bash
deploy.bat
```

**Manual deployment:**
```bash
# Build the project
platformio run

# Upload to device on COM7
platformio run --target upload

# Monitor serial output
platformio device monitor
```

### Quick Deployment (Linux/Mac)

**Using deployment script:**
```bash
./deploy.sh
```

**Manual deployment:**
```bash
# Build the project
platformio run

# Upload to device (auto-detects port)
platformio run --target upload

# Monitor serial output
platformio device monitor
```

### Deployment Documentation
- **DEPLOYMENT_GUIDE.md** - Complete deployment instructions
- **TESTING_CHECKLIST.md** - Production testing procedures
- **deploy.bat** - Windows deployment script
- **deploy.sh** - Linux/Mac deployment script

### Project Structure

```
chameleon-os/
├── platformio.ini          # ESP32-S3 configuration
├── MASTER_BLUEPRINT.md     # Architecture documentation
├── AGENT_BRIEFING.md       # Developer guide
├── src/
│   ├── main.cpp           # Entry point
│   ├── hal/               # Hardware abstraction
│   ├── core/              # Core systems
│   └── apps/              # 33 applications
└── include/               # Public headers
```

## Documentation

- **MASTER_BLUEPRINT.md**: Complete architecture overview
- **AGENT_BRIEFING.md**: Development guidelines and API reference

## Critical Implementation Details

### Golden Path Initialization
WiFi **MUST** initialize before Display:
1. WiFi Init (2-5s, parallel)
2. Display Init (with double-buffer)
3. Remaining systems

### Panel Gap (0, 34)
- Top gap: 0 pixels
- Bottom gap: 34 pixels
- All rendering respects effective area: 320×138

### Double Buffering
- Two framebuffers in PSRAM
- Draw to back buffer while displaying front
- Swap on complete to avoid tearing

### 60 FPS Target
Frame budget: 16.67ms
- Input: 2ms
- Update: 5ms
- Render: 8ms
- Swap: 1.67ms

## License

See LICENSE file for details.

## Contributing

This is a production-ready codebase targeting luxury UX standards. All contributions must maintain:
- 60 FPS performance
- Zero compromises on quality
- Comprehensive documentation
- Clean, modular architecture

