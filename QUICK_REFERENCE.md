# Chameleon OS - Quick Reference Card

## Essential Commands

```bash
# Build
platformio run

# Upload to device
platformio run --target upload

# Monitor serial
platformio device monitor

# Clean build
platformio run --target clean
```

## Critical Constants

```cpp
#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 172
#define PANEL_GAP_TOP 0
#define PANEL_GAP_BOTTOM 34
#define EFFECTIVE_HEIGHT 138  // 172 - 0 - 34
#define TARGET_FPS 60
#define FRAME_TIME_MS 16.67
```

## HAL Quick Reference

### Display
```cpp
#include "hal.h"

// Init (call after WiFi!)
display_hal_init();

// Drawing
display_hal_clear(COLOR_BLACK);
display_hal_pixel(x, y, COLOR_WHITE);
display_hal_rect(x, y, w, h, COLOR_BLUE);
display_hal_line(x0, y0, x1, y1, COLOR_RED);
display_hal_text(x, y, "Hello", COLOR_WHITE, 2);

// Must call to present
display_hal_swap();
```

### Touch
```cpp
TouchEvent_t event;
if (touch_hal_read(&event)) {
    // event.x, event.y, event.pressed
}
```

### WiFi
```cpp
// Init BEFORE display!
wifi_hal_init();

// Connect
WiFiConfig_t cfg = {
    .ssid = "MySSID",
    .password = "password",
    .timeout_ms = 10000
};
wifi_hal_connect(&cfg);

// Status
WiFiStatus_t status = wifi_hal_get_status();
int8_t rssi = wifi_hal_get_rssi();
char ip[16];
wifi_hal_get_ip(ip);
```

### NTP
```cpp
ntp_hal_init();
ntp_hal_sync("pool.ntp.org", 5000);
time_t now = ntp_hal_get_time();
bool synced = ntp_hal_is_synced();
```

## Event Bus

### Publish
```cpp
#include "event_bus.h"

Event_t event = {
    .type = EVENT_TOUCH,
    .data = &my_data,
    .size = sizeof(my_data),
    .timestamp = 0
};
event_bus_publish(&event);
```

### Subscribe
```cpp
void my_handler(Event_t* event) {
    // Handle event
}

event_bus_subscribe(EVENT_TOUCH, my_handler);
```

### Event Types
```cpp
EVENT_TOUCH
EVENT_WIFI_STATUS
EVENT_APP_SWITCH
EVENT_THEME_CHANGE
EVENT_TIME_SYNC
EVENT_CUSTOM_START  // +100 for custom
```

## Theme Manager

```cpp
#include "theme_manager.h"

// Get colors
uint16_t bg = theme_get_color(THEME_COLOR_BACKGROUND);
uint16_t fg = theme_get_color(THEME_COLOR_FOREGROUND);
uint16_t primary = theme_get_color(THEME_COLOR_PRIMARY);

// Switch theme
theme_set(THEME_DARK);    // or THEME_LIGHT, THEME_CUSTOM

// Custom color
theme_set_custom_color(THEME_COLOR_PRIMARY, 0x1C9F);
```

### Color IDs
```cpp
THEME_COLOR_BACKGROUND
THEME_COLOR_FOREGROUND
THEME_COLOR_PRIMARY
THEME_COLOR_SECONDARY
THEME_COLOR_ACCENT
THEME_COLOR_SUCCESS
THEME_COLOR_WARNING
THEME_COLOR_ERROR
THEME_COLOR_TEXT
THEME_COLOR_TEXT_SECONDARY
THEME_COLOR_BORDER
THEME_COLOR_SHADOW
```

## App Development

### App Template
```cpp
#include "app_framework.h"
#include "theme_manager.h"
#include "hal.h"

static void my_app_init(void) {
    // One-time setup
}

static void my_app_on_enter(void) {
    // Called when app becomes active
}

static void my_app_on_update(void) {
    // Called every frame (60 FPS)
    // MUST complete in <5ms!
    
    uint16_t fg = theme_get_color(THEME_COLOR_FOREGROUND);
    uint16_t primary = theme_get_color(THEME_COLOR_PRIMARY);
    
    display_hal_text(100, 50, "My App", primary, 2);
    display_hal_text(50, 80, "App content", fg, 1);
}

static void my_app_on_exit(void) {
    // Called when leaving app
}

static void my_app_cleanup(void) {
    // Free resources
}

App_t app_myapp = {
    .name = "My App",
    .icon = "myicon",
    .sector = 1,  // 1-6
    .init = my_app_init,
    .on_enter = my_app_on_enter,
    .on_update = my_app_on_update,
    .on_exit = my_app_on_exit,
    .cleanup = my_app_cleanup
};

// Register in app_registry.cpp
```

## RGB565 Colors

```cpp
// Format: RRRRRGGGGGGBBBBB
#define COLOR_BLACK   0x0000
#define COLOR_WHITE   0xFFFF
#define COLOR_RED     0xF800
#define COLOR_GREEN   0x07E0
#define COLOR_BLUE    0x001F
#define COLOR_YELLOW  0xFFE0
#define COLOR_CYAN    0x07FF
#define COLOR_MAGENTA 0xF81F
#define COLOR_GRAY    0x8410

// Convert RGB888 to RGB565
uint16_t rgb(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0xF8) << 8) | 
           ((g & 0xFC) << 3) | 
           (b >> 3);
}
```

## Performance Tips

### Frame Budget (16.67ms)
```
Input:   2ms   ▓▓░░░░░░░░░░░░░░
Update:  5ms   ▓▓▓▓▓░░░░░░░░░░░
Render:  8ms   ▓▓▓▓▓▓▓▓░░░░░░░░
Swap:    1.67ms▓░░░░░░░░░░░░░░░
```

### Do's
- ✅ Use PSRAM for large buffers
- ✅ Pre-calculate in init()
- ✅ Use lookup tables
- ✅ Batch draw operations
- ✅ Profile with esp_timer_get_time()

### Don'ts
- ❌ malloc/free in update loop
- ❌ Blocking operations
- ❌ Heavy calculations in onUpdate()
- ❌ Drawing outside effective area
- ❌ Forgetting display_hal_swap()

## Profiling

```cpp
#include <esp_timer.h>

int64_t start = esp_timer_get_time();
// Your code
int64_t elapsed = esp_timer_get_time() - start;

ESP_LOGI("PERF", "Operation took %lld us", elapsed);
```

## Debugging

```cpp
#include <esp_log.h>

ESP_LOGI("TAG", "Info: %d", value);
ESP_LOGW("TAG", "Warning: %s", str);
ESP_LOGE("TAG", "Error: %d", code);
ESP_LOGD("TAG", "Debug: %f", val);  // Debug builds only
```

## Common Patterns

### Draw Centered Text
```cpp
const char* text = "Hello";
int text_width = strlen(text) * 6 * size;  // Approx
int x = (DISPLAY_WIDTH - text_width) / 2;
int y = (EFFECTIVE_HEIGHT - 8 * size) / 2;
display_hal_text(x, y, text, color, size);
```

### Animate Value
```cpp
static int target = 100;
static int current = 0;

void update() {
    // Smooth interpolation
    current += (target - current) * 0.1;
    
    char buf[32];
    snprintf(buf, sizeof(buf), "%d", current);
    display_hal_text(100, 50, buf, COLOR_WHITE, 2);
}
```

### State Machine
```cpp
typedef enum {
    STATE_IDLE,
    STATE_RUNNING,
    STATE_COMPLETE
} State_t;

static State_t state = STATE_IDLE;

void update() {
    switch(state) {
        case STATE_IDLE:
            // Handle idle
            break;
        case STATE_RUNNING:
            // Handle running
            break;
        case STATE_COMPLETE:
            // Handle complete
            break;
    }
}
```

## File Locations

```
include/hal.h              - HAL interface
include/event_bus.h        - Event system
include/theme_manager.h    - Theming
include/app_framework.h    - App lifecycle
include/ui_framework.h     - UI framework
include/app_registry.h     - App registry

src/main.cpp              - Entry point
src/hal/*.cpp             - HAL implementations
src/core/*.cpp            - Core systems
src/apps/*.cpp            - App framework
src/apps/sector*/*.cpp    - App implementations
```

## Golden Path Init Order

```
1. WiFi Init          ← BEFORE display!
2. Event Bus
3. Display Init
4. Touch Init
5. Theme Manager
6. NTP Init
7. App Framework
8. Register Apps
9. UI Framework
10. Launch Home App
```

## Troubleshooting

**Display not updating?**
- Check display_hal_swap() is called
- Verify drawing within effective area (y < 138)

**App not switching?**
- Check event bus initialized
- Verify app registered in registry
- Check swipe threshold (50px)

**Performance issues?**
- Profile with esp_timer_get_time()
- Check frame time < 16.67ms
- Look for blocking operations

**WiFi not connecting?**
- Verify WiFi init before display
- Check credentials
- Increase timeout_ms

---

**Quick Help**: See MASTER_BLUEPRINT.md for architecture, AGENT_BRIEFING.md for detailed guide
