# Agent Briefing - Chameleon OS Development Guide

## Quick Start for Development Agents

### Project Context
You are working on **Chameleon OS**, a production-ready embedded operating system for ESP32-S3 with a 320×172 touch display. This is a **PlatformIO** project built on the **Demo V6** stable codebase.

### Critical Requirements ⚠️

1. **Panel Gap (0, 34)**: 
   - Top gap = 0 pixels
   - Bottom gap = 34 pixels
   - Effective display area = 320×138
   - NEVER render in the bottom 34 pixels

2. **Double Buffering**:
   - Always use two framebuffers
   - Draw to back buffer, swap on complete
   - Essential for 60 FPS

3. **Golden Path Initialization**:
   - WiFi MUST initialize BEFORE Display
   - Follow exact sequence in MASTER_BLUEPRINT.md
   - No exceptions to init order

4. **60 FPS Target**:
   - Frame budget: 16.67ms
   - Optimize all rendering paths
   - Profile and measure performance

### File Structure
```
chameleon-os/
├── platformio.ini          # ESP32-S3 configuration
├── MASTER_BLUEPRINT.md     # Architecture (READ THIS FIRST)
├── AGENT_BRIEFING.md       # This file
├── src/
│   ├── main.c             # Entry point, Golden Path init
│   ├── hal/               # Hardware Abstraction Layer
│   │   ├── hal.h          # HAL interface
│   │   ├── display_hal.c  # Display driver (320×172, double-buffer)
│   │   ├── touch_hal.c    # Touch screen driver
│   │   ├── wifi_hal.c     # WiFi management
│   │   └── ntp_hal.c      # Time synchronization
│   ├── core/              # Core systems
│   │   ├── event_bus.c    # Pub/Sub event system
│   │   ├── theme_manager.c # Theming engine
│   │   └── ui_framework.c # Focus-Carousel UI
│   └── apps/              # Application layer (33 apps)
│       ├── app_framework.c # App lifecycle management
│       ├── sector1_productivity/
│       ├── sector2_communication/
│       ├── sector3_media/
│       ├── sector4_tools/
│       ├── sector5_lifestyle/
│       └── sector6_games/
└── include/               # Public headers
```

### Key Entry Points

#### 1. platformio.ini
- ESP32-S3 board configuration
- Build flags for display dimensions, panel gap
- Library dependencies
- Optimization settings

#### 2. src/main.c
- `setup()`: Golden Path initialization
- `loop()`: 60 FPS main loop
- Critical init order enforcement

#### 3. src/hal/hal.h
- HAL wrapper interface
- Unified API for all hardware
- Hardware abstraction guarantees portability

### Development Workflow

#### Building
```bash
cd /home/runner/work/chameleon-os/chameleon-os
pio run
```

#### Uploading (if hardware available)
```bash
pio run --target upload
```

#### Monitoring
```bash
pio device monitor
```

### Coding Standards

#### Style
- **C Language**: C99 standard
- **Naming**: 
  - Functions: `snake_case()`
  - Types: `PascalCase_t`
  - Constants: `UPPER_SNAKE_CASE`
  - Globals: `g_variableName`
- **Indentation**: 4 spaces, no tabs
- **Line Length**: 100 characters max
- **Comments**: Doxygen-style for public APIs

#### Example
```c
/**
 * @brief Initialize the display HAL
 * @return 0 on success, error code otherwise
 */
int display_hal_init(void) {
    // Implementation with panel gap and double-buffer
}
```

### HAL Development Guidelines

#### Display HAL (`display_hal.c`)
```c
// Must implement:
int display_hal_init(void);                    // Init with double-buffer
void display_hal_clear(uint16_t color);        // Clear back buffer
void display_hal_pixel(int x, int y, uint16_t color);
void display_hal_swap(void);                   // Swap buffers
void display_hal_set_brightness(uint8_t level);
// Respect panel gap: render area is 320×138
```

#### Touch HAL (`touch_hal.c`)
```c
// Must implement:
int touch_hal_init(void);
bool touch_hal_read(int* x, int* y, bool* pressed);
void touch_hal_calibrate(void);
```

#### WiFi HAL (`wifi_hal.c`)
```c
// Must implement:
int wifi_hal_init(void);                       // Init BEFORE display!
void wifi_hal_connect(const char* ssid, const char* password);
bool wifi_hal_is_connected(void);
void wifi_hal_disconnect(void);
```

#### NTP HAL (`ntp_hal.c`)
```c
// Must implement:
int ntp_hal_init(void);
void ntp_hal_sync(void);
time_t ntp_hal_get_time(void);
```

### Event Bus Usage

#### Publishing Events
```c
#include "core/event_bus.h"

Event_t event = {
    .type = EVENT_TOUCH,
    .data = &touch_data,
    .size = sizeof(touch_data)
};
event_bus_publish(&event);
```

#### Subscribing to Events
```c
void on_touch_event(Event_t* event) {
    TouchData_t* data = (TouchData_t*)event->data;
    // Handle touch event
}

event_bus_subscribe(EVENT_TOUCH, on_touch_event);
```

### Theme Manager Usage

```c
#include "core/theme_manager.h"

// Get current theme colors
uint16_t bg = theme_get_color(THEME_COLOR_BACKGROUND);
uint16_t fg = theme_get_color(THEME_COLOR_FOREGROUND);

// Switch theme
theme_set(THEME_DARK);
```

### App Development

#### Creating a New App
```c
#include "apps/app_framework.h"

static void my_app_init(void) {
    // One-time initialization
}

static void my_app_on_enter(void) {
    // Called when app becomes active
}

static void my_app_on_update(void) {
    // Called every frame (60 FPS)
    // Keep under 5ms!
}

static void my_app_on_exit(void) {
    // Called when leaving app
}

static void my_app_cleanup(void) {
    // Free resources
}

App_t my_app = {
    .name = "My App",
    .icon = "icon_myapp",
    .sector = 1,  // Productivity sector
    .init = my_app_init,
    .onEnter = my_app_on_enter,
    .onUpdate = my_app_on_update,
    .onExit = my_app_on_exit,
    .cleanup = my_app_cleanup
};
```

### Performance Optimization

#### Frame Budget (60 FPS = 16.67ms/frame)
- Input: 2ms
- Update: 5ms
- Render: 8ms
- Swap: 1.67ms

#### Tips
1. Use PSRAM for large buffers
2. Avoid malloc/free in update loops
3. Use lookup tables instead of calculations
4. Profile with `esp_timer_get_time()`
5. Batch draw calls when possible

#### Profiling Example
```c
#include <esp_timer.h>

int64_t start = esp_timer_get_time();
// Your code here
int64_t elapsed = esp_timer_get_time() - start;
if (elapsed > 5000) {  // More than 5ms
    ESP_LOGW("PERF", "Slow operation: %lld us", elapsed);
}
```

### Common Pitfalls ⚠️

1. **Don't** initialize Display before WiFi
2. **Don't** render in bottom 34 pixels
3. **Don't** forget to swap buffers after drawing
4. **Don't** block in app update functions
5. **Don't** allocate memory in 60 FPS loops
6. **Don't** ignore the frame time budget

### Testing Checklist

Before committing code, verify:
- [ ] Code compiles without warnings
- [ ] Respects panel gap (0, 34)
- [ ] Uses double buffering correctly
- [ ] WiFi initializes before display
- [ ] Frame time under 16.67ms
- [ ] No memory leaks
- [ ] Follows coding standards
- [ ] Documentation updated

### Debugging

#### Serial Logging
```c
#include <esp_log.h>

ESP_LOGI("TAG", "Info message");
ESP_LOGW("TAG", "Warning message");
ESP_LOGE("TAG", "Error message");
ESP_LOGD("TAG", "Debug message");  // Only in debug builds
```

#### Logging Levels
Set in platformio.ini:
- 0 = None
- 1 = Error
- 2 = Warning
- 3 = Info
- 4 = Debug
- 5 = Verbose

### Resources

- **MASTER_BLUEPRINT.md**: Complete architecture documentation
- **platformio.ini**: Build configuration
- **src/main.c**: Application entry point
- **ESP32-S3 Docs**: https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/
- **TFT_eSPI**: https://github.com/Bodmer/TFT_eSPI

### Questions?

When in doubt:
1. Check MASTER_BLUEPRINT.md first
2. Follow the Golden Path initialization
3. Respect the 60 FPS frame budget
4. Maintain luxury UX standards

---

**Remember**: This is production-ready code. Quality over speed. Every line matters.

**Target**: 60 FPS, Luxury UX, Zero compromises.
