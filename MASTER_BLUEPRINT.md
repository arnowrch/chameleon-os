# Chameleon OS - Master Blueprint

## Vision
Universal embedded operating system for ESP32-S3 with Waveshare 320×172 Touch Display.  
Transforms a single device into 33 specialized applications with luxury UX at 60 FPS.

## Hardware Platform
- **MCU**: ESP32-S3 (Dual-core, 240MHz, with PSRAM)
- **Display**: Waveshare 320×172 LCD with Touch Panel
- **Target**: Production-ready, professional-grade embedded system

## Architecture Overview

### 1. Core Systems
```
┌─────────────────────────────────────────┐
│         Application Layer (33 Apps)     │
│  Organized in 6 Sectors                 │
├─────────────────────────────────────────┤
│      Focus-Carousel UI Framework        │
│  - App Switching                        │
│  - Gesture Navigation                   │
├─────────────────────────────────────────┤
│        Core Services Layer              │
│  - Event Bus (Pub/Sub)                  │
│  - Theme Manager                        │
│  - Settings Manager                     │
├─────────────────────────────────────────┤
│     Hardware Abstraction Layer (HAL)    │
│  - Display | Touch | WiFi | NTP         │
└─────────────────────────────────────────┘
```

### 2. HAL (Hardware Abstraction Layer)

#### Display HAL
- Resolution: 320×172 pixels
- **Panel Gap**: Top=0, Bottom=34 (critical for proper rendering)
- **Double Buffer**: Eliminates flicker, enables 60 FPS
- Color depth: 16-bit (RGB565)
- Driver: TFT_eSPI optimized for ESP32-S3

#### Touch HAL
- Touch panel integrated with display
- Calibration support
- Multi-touch events (tap, swipe, hold)
- Gesture recognition

#### WiFi HAL
- Station mode (client)
- Access Point mode
- Connection management
- Auto-reconnect logic

#### NTP HAL
- Time synchronization
- Timezone support
- RTC integration

### 3. Event Bus System
Central communication hub using publish-subscribe pattern:
- Decouples components
- Enables reactive programming
- Type-safe event handling
- Priority queue support

**Event Types**:
- `EVENT_TOUCH`: Touch screen events
- `EVENT_WIFI`: WiFi connection status
- `EVENT_APP_SWITCH`: App navigation
- `EVENT_THEME_CHANGE`: UI theme updates
- `EVENT_TIME_SYNC`: Time synchronization
- Custom app events

### 4. Theme Manager
Dynamic theming system for luxury UX:
- Color schemes (Dark, Light, Custom)
- Font management
- Icon sets
- Transition animations
- Per-app theme overrides

### 5. Focus-Carousel UI
Innovative navigation system:
- Horizontal carousel for app switching
- Focus indicator on active app
- Smooth 60 FPS transitions
- Gesture-based navigation (swipe left/right)
- Visual feedback for all interactions

### 6. Application Framework

#### App Lifecycle
```
init() → onEnter() → onUpdate() → onExit() → cleanup()
```

#### App Base Structure
```c
typedef struct {
    const char* name;
    const char* icon;
    uint8_t sector;
    void (*init)(void);
    void (*onEnter)(void);
    void (*onUpdate)(void);
    void (*onExit)(void);
    void (*cleanup)(void);
} App_t;
```

#### 33 Apps across 6 Sectors

**Sector 1: Productivity** (6 apps)
1. Calendar
2. Notes
3. Tasks
4. Timer
5. Calculator
6. Voice Recorder

**Sector 2: Communication** (6 apps)
7. WiFi Manager
8. Weather
9. News Feed
10. Email Client
11. Messenger
12. Social Hub

**Sector 3: Media** (5 apps)
13. Music Player
14. Photo Gallery
15. Video Player
16. Radio
17. Podcast Player

**Sector 4: Tools** (6 apps)
18. File Manager
19. Settings
20. System Monitor
21. Terminal
22. Code Editor
23. Package Manager

**Sector 5: Lifestyle** (5 apps)
24. Fitness Tracker
25. Health Monitor
26. Sleep Tracker
27. Meditation
28. Habit Tracker

**Sector 6: Games & Entertainment** (5 apps)
29. Puzzle Game
30. Retro Game
31. Drawing App
32. Animations
33. Screen Saver

## Golden Path Initialization

**Critical Init Sequence** (must follow this order):
```
1. System Init (HAL base, logging)
2. WiFi Init (BEFORE display - critical!)
3. Event Bus Init
4. Display Init (with double-buffer)
5. Touch Init
6. Theme Manager Init
7. NTP Init (time sync)
8. App Framework Init
9. Load Apps (all 33)
10. Focus-Carousel Init
11. Enter Main Loop
```

**Why WiFi Before Display?**
- WiFi initialization can take 2-5 seconds
- Starting WiFi first allows parallel initialization
- Display initialization can use WiFi status
- Reduces perceived boot time
- Avoids blocking UI thread

## Performance Targets

### 60 FPS Rendering
- Frame time: 16.67ms per frame
- Budget allocation:
  - Input processing: 2ms
  - App update: 5ms
  - Rendering: 8ms
  - Buffer swap: 1.67ms

### Memory Management
- PSRAM utilization for framebuffers
- Heap monitoring
- Stack overflow protection
- Resource pooling

### Power Efficiency
- Dynamic frequency scaling
- Sleep modes when idle
- Backlight PWM control
- WiFi power save mode

## Critical Features

### Panel Gap (0, 34)
- Top gap: 0 pixels (full height from top)
- Bottom gap: 34 pixels (reserved area)
- Effective viewport: 320×138 pixels
- All apps must respect this constraint
- UI framework automatically handles offset

### Double Buffering
- Two framebuffers in PSRAM
- Draw to back buffer while displaying front
- Swap on VSync
- Eliminates tearing and flicker
- Essential for 60 FPS

### Luxury UX Principles
- Every interaction has visual feedback
- Smooth animations (easing functions)
- Haptic feedback (if hardware available)
- Consistent design language
- Professional typography
- Attention to micro-interactions

## Build & Deploy

### Development Workflow
```bash
# Build project
pio run

# Upload to device
pio run --target upload

# Monitor serial output
pio device monitor

# Clean build
pio run --target clean
```

### Production Build
```bash
# Optimized build with size optimization
pio run -e esp32-s3-devkitc-1 --target upload
```

## Testing Strategy
- Unit tests for core systems
- Integration tests for HAL
- UI/UX testing on real hardware
- Performance profiling (FPS monitoring)
- Memory leak detection
- Power consumption analysis

## Version History
- **Demo V6**: Stable codebase (baseline)
- **Current**: Production implementation

## Future Enhancements
- Over-the-air (OTA) updates
- App marketplace
- Cloud sync
- Multi-device support
- Custom app development SDK

---

**Status**: Production-Ready Target  
**Target FPS**: 60  
**Code Quality**: Luxury UX Standard
