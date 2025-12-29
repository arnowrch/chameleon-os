# 🤖 CHAMELEON OS: Agent Briefing für VS Code

> **Für**: VS Code Agent (Claude, Cursor, Copilot, etc.)  
> **Zweck**: Komplette Anleitung zur Implementierung von Chameleon OS  
> **Format**: Agent-freundlich, strukturiert, ausführbar

---

## 🎯 DEINE MISSION

Du wirst **Chameleon OS** implementieren: Ein universelles Betriebssystem für ESP32-S3 mit Touch-Display, das sich in 33 verschiedene spezialisierte Apps verwandelt.

**Dein Rolle**: System Architect Assistant
- Implementiere nach Blueprint
- Halte dich an Best Practices
- Dokumentiere alles
- Arbeite iterativ (Dummy → Funktionsfähig → Abgelegt)

---

## 📊 PROJEKT-ÜBERSICHT

### Hardware
- **MCU**: ESP32-S3 Dual-Core (240 MHz)
- **Display**: Waveshare 320×172 RGB (JD9853 Controller)
- **Touch**: CST816S (I2C)
- **RAM**: 8 MB PSRAM (essentiell für 60 FPS)
- **Flash**: 16 MB (Firmware + Assets)
- **Storage**: TF-Card Slot (für Bilder, Fonts)

### Toolchain
- **Build System**: PlatformIO (nicht ESP-IDF!)
- **Framework**: LVGL v9
- **Language**: C
- **Version Control**: Git + GitHub

### Architektur (4 Schichten)

```
Layer 1: HAL (Hardware Abstraction)
├─ Display (ST7789, Panel Gap 34, Rotation 90°)
├─ Touch (CST816S, I2C)
├─ WiFi (Auto-Reconnect, Reentrancy Guards)
└─ NTP (elegante Sync-Animation)

Layer 2: Framework
├─ Event Bus (Sensoren → Apps)
├─ Theme Manager (Farben + Styles)
├─ Carousel Navigation (Focus-Scaling)
└─ Gesture Recognition (Swipe, Long-Press)

Layer 3: Apps (33 Dummies)
├─ Sector A: Smart Home (Thermostat, Weather, etc.)
├─ Sector B: Desktop & Workflow (Pomodoro, etc.)
├─ Sector C: Industrial (WiFi Analyzer, etc.)
├─ Sector D: Automotive
├─ Sector E: Lifestyle
└─ Sector F: Security

Layer 4: Quality
├─ Startup Validator
├─ Performance Monitor
├─ Build Automation
└─ Git Workflow
```

---

## 🚀 PHASE 1: FOUNDATION (Deine erste Aufgabe)

### Schritt 1: Projekt-Struktur aufbauen

```bash
chameleon-os/
├── platformio.ini                 # PlatformIO Konfiguration
├── src/
│   ├── main.c                     # Einstiegspunkt
│   ├── config.h                   # Globale Konfiguration
│   ├── hal/                       # Hardware Abstraction Layer
│   │   ├── display.c              # ST7789 + JD9853 Init
│   │   ├── display.h
│   │   ├── touch.c                # CST816S Init
│   │   ├── touch.h
│   │   ├── wifi.c                 # WiFi + Auto-Reconnect
│   │   ├── wifi.h
│   │   ├── ntp.c                  # SNTP Init
│   │   ├── ntp.h
│   │   └── hal.h                  # HAL Interface
│   ├── framework/
│   │   ├── event_bus.c            # Event-Driven Architecture
│   │   ├── event_bus.h
│   │   ├── theme_manager.c        # Zentrale Theme-Verwaltung
│   │   ├── theme_manager.h
│   │   ├── carousel.c             # Hauptmenü-Navigation
│   │   ├── carousel.h
│   │   ├── gesture.c              # Swipe, Long-Press
│   │   └── gesture.h
│   ├── apps/                      # Die 33 Apps
│   │   ├── app_interface.h        # Standard App-Interface
│   │   ├── app_registry.c         # App-Registrierung
│   │   ├── sector_a/              # Smart Home
│   │   │   ├── weather_crystal/
│   │   │   │   ├── weather.c
│   │   │   │   ├── weather.h
│   │   │   │   └── ui.c
│   │   │   ├── thermostat/
│   │   │   └── ...
│   │   ├── sector_b/              # Desktop & Workflow
│   │   │   ├── pomodoro_timer/
│   │   │   │   ├── pomodoro.c
│   │   │   │   ├── pomodoro.h
│   │   │   │   └── ui.c
│   │   │   ├── wifi_analyzer/
│   │   │   └── ...
│   │   ├── sector_c/
│   │   ├── sector_d/
│   │   ├── sector_e/
│   │   └── sector_f/
│   ├── assets/
│   │   ├── fonts/
│   │   ├── icons/
│   │   └── themes/
│   └── startup_validator.c        # Boot-Diagnose
├── docs/
│   ├── README.md
│   ├── ARCHITECTURE.md
│   ├── BEST_PRACTICES.md
│   ├── APP_DEVELOPMENT.md
│   └── PLAYBOOK.md
├── .gitignore
└── README.md
```

### Schritt 2: platformio.ini erstellen

**Datei: `platformio.ini`**

```ini
[env:chameleon-os]
platform = espressif32
board = esp32-s3-devkitc-1
framework = arduino

# Build-Optionen
build_flags = 
    -DCONFIG_SPIRAM=y
    -DCONFIG_SPIRAM_MODE_QUAD=y
    -DCONFIG_SPIRAM_SPEED_80M=y
    -O3
    -march=xtensa-lx7
    -mtune=xtensa-lx7

# Speicher-Konfiguration
board_build.partitions = partitions.csv

# Monitor
monitor_speed = 115200
monitor_filters = esp32_exception_decoder

# Upload
upload_speed = 460800

# Libraries
lib_deps = 
    lvgl/lvgl @ ^9.0.0
    espressif/esp-idf-cxx @ ^1.0.0

# Includes
lib_extra_dirs = lib/
```

### Schritt 3: main.c - Golden Path Initialisierung

**Datei: `src/main.c`**

```c
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "hal.h"
#include "framework/event_bus.h"
#include "framework/theme_manager.h"
#include "framework/carousel.h"
#include "startup_validator.h"

static const char *TAG = "CHAMELEON_OS";

// ============================================
// GOLDEN PATH INITIALIZATION
// ============================================
void app_main(void) {
    ESP_LOGI(TAG, "\n\n=== CHAMELEON OS Boot ===\n");
    
    // 1. NVS (Non-Volatile Storage)
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    ESP_LOGI(TAG, "✓ NVS initialized");
    
    // 2. I2C Bus (für Touch)
    ESP_ERROR_CHECK(i2c_bus_init());
    ESP_LOGI(TAG, "✓ I2C bus initialized");
    
    // 3. WiFi (MUSS vor Display sein!)
    ESP_ERROR_CHECK(wifi_init());
    ESP_LOGI(TAG, "✓ WiFi initialized");
    
    // 4. Display (mit Panel Gap!)
    ESP_ERROR_CHECK(display_init());
    ESP_LOGI(TAG, "✓ Display initialized (320×172, rotation 90°)");
    
    // 5. Touch
    ESP_ERROR_CHECK(touch_init());
    ESP_LOGI(TAG, "✓ Touch initialized");
    
    // 6. LVGL
    ESP_ERROR_CHECK(lvgl_port_init());
    ESP_LOGI(TAG, "✓ LVGL initialized");
    
    // 7. Backlight
    ESP_ERROR_CHECK(backlight_init());
    ESP_LOGI(TAG, "✓ Backlight initialized");
    
    // 8. NTP (nach WiFi)
    ESP_ERROR_CHECK(ntp_init());
    ESP_LOGI(TAG, "✓ NTP initialized");
    
    // 9. Event Bus
    event_bus_init();
    ESP_LOGI(TAG, "✓ Event bus initialized");
    
    // 10. Theme Manager
    theme_init();
    ESP_LOGI(TAG, "✓ Theme manager initialized");
    
    // 11. Startup Validation
    startup_validate();
    
    // 12. UI
    ui_init();
    ESP_LOGI(TAG, "✓ UI initialized");
    
    // 13. Carousel (App-Menü)
    carousel_start();
    ESP_LOGI(TAG, "✓ Carousel started");
    
    ESP_LOGI(TAG, "\n✅ Boot complete! System ready.\n");
    
    // System läuft jetzt in LVGL Task (Core 0)
    // Event Loop wird in LVGL Timer Handler aufgerufen
}
```

---

## 📋 PHASE 2: DIE 3 ERSTEN APPS

### App 1: Weather Crystal (Sektor A)

**Anforderung**: Visuelle Wetteranimationen ohne Text

**Entwicklungs-Workflow**:

1. **Dummy erstellen** (Tag 1)
   - App-Grundgerüst
   - Statische UI (Kreis mit Farben)
   - Dummy-Wetterdaten hardcodiert
   - Commit: "Weather Crystal: Dummy v1.0"

2. **Funktionsfähig machen** (Tag 2-3)
   - WiFi-Integration (OpenWeatherMap API)
   - Echte Wetterdaten abrufen
   - Animationen (Wolken, Regen, Sonne)
   - Event-Bus Integration
   - Commit: "Weather Crystal: Feature Complete v1.0"

3. **Abgelegt & dokumentiert** (Tag 4)
   - Code-Review
   - README.md
   - Tests
   - Version getaggt: `weather_crystal/v1.0.0`
   - Abhaken in PROJECT_STATUS.md

**Template: `src/apps/sector_a/weather_crystal/weather.c`**

```c
#include "lvgl.h"
#include "app_interface.h"
#include "framework/event_bus.h"
#include "framework/theme_manager.h"

static const char *TAG = "WEATHER_CRYSTAL";

// === APP STATE ===
typedef struct {
    float temperature;
    float humidity;
    const char *condition;  // "sunny", "cloudy", "rainy", etc.
    lv_obj_t *container;
} weather_state_t;

static weather_state_t state = {
    .temperature = 15.5f,
    .humidity = 65.0f,
    .condition = "cloudy",
    .container = NULL,
};

// === LIFECYCLE ===
void weather_init(void) {
    ESP_LOGI(TAG, "Weather Crystal initialized");
    
    // Abonniere Weather-Updates vom Event Bus
    event_subscribe(EVENT_WEATHER_UPDATE, weather_on_update);
}

void weather_deinit(void) {
    ESP_LOGI(TAG, "Weather Crystal deinitialized");
}

void weather_start(void) {
    ESP_LOGI(TAG, "Weather Crystal started");
    
    // Starte API-Abruf
    // (wird asynchron über Event Bus benachrichtigt)
}

void weather_stop(void) {
    ESP_LOGI(TAG, "Weather Crystal stopped");
}

// === RENDERING ===
void weather_render(void) {
    if (state.container == NULL) {
        state.container = lv_obj_create(lv_screen_active());
        lv_obj_set_size(state.container, LV_PCT(100), LV_PCT(100));
        lv_obj_set_style_bg_color(state.container, THEME_COLOR(bg_primary), 0);
    }
    
    // Zeichne Wetter-Animation basierend auf state.condition
    // (TODO: Implementieren)
}

void weather_update(void) {
    // Wird von LVGL Timer aufgerufen
    // Update Animationen, etc.
}

// === INTERACTION ===
void weather_on_touch(touch_event_t event) {
    // Handle Touch-Events
}

void weather_on_button(button_event_t event) {
    // Handle Button-Events
}

// === EVENT HANDLER ===
void weather_on_update(event_t *event) {
    weather_data_t *data = (weather_data_t *)event->data;
    state.temperature = data->temperature;
    state.humidity = data->humidity;
    state.condition = data->condition;
    
    ESP_LOGI(TAG, "Weather updated: %.1f°C, %s", state.temperature, state.condition);
}

// === CONFIGURATION ===
void weather_configure(config_t* config) {
    // Konfiguriere App
}

config_t* weather_get_config(void) {
    return NULL;
}

// === METADATA ===
const char* weather_get_icon(void) {
    return "🌤️";
}

const char* weather_get_description(void) {
    return "Visual Weather Crystal - Animated weather display";
}

// === APP REGISTRATION ===
const app_t weather_crystal_app = {
    .app_name = "Weather Crystal",
    .app_version = "1.0.0",
    .sector = "a",
    
    .app_init = weather_init,
    .app_deinit = weather_deinit,
    .app_start = weather_start,
    .app_stop = weather_stop,
    
    .app_render = weather_render,
    .app_update = weather_update,
    
    .app_on_touch = weather_on_touch,
    .app_on_button = weather_on_button,
    
    .app_configure = weather_configure,
    .app_get_config = weather_get_config,
    
    .app_get_icon = weather_get_icon,
    .app_get_description = weather_get_description,
};
```

### App 2: WiFi Analyzer (Sektor C)

**Anforderung**: WiFi-Spektrum-Analyse mit Graphen

**Entwicklungs-Workflow**: Gleich wie Weather Crystal (Dummy → Funktionsfähig → Abgelegt)

### App 3: Pomodoro Timer (Sektor B)

**Anforderung**: Fokus-Visualisierung mit sanftem Glow-Down

**Entwicklungs-Workflow**: Gleich wie Weather Crystal (Dummy → Funktionsfähig → Abgelegt)

---

## 📊 PROJECT_STATUS.md (Deine Checkliste)

**Datei: `docs/PROJECT_STATUS.md`**

```markdown
# CHAMELEON OS: Project Status

## CORE FRAMEWORK
- [x] Projekt-Struktur
- [x] PlatformIO Konfiguration
- [x] HAL: Display (ST7789, Panel Gap, Rotation)
- [x] HAL: Touch (CST816S)
- [x] HAL: WiFi (Auto-Reconnect)
- [x] HAL: NTP (SNTP)
- [x] Framework: Event Bus
- [x] Framework: Theme Manager
- [x] Framework: Carousel Navigation
- [x] Framework: Gesture Recognition
- [x] Startup Validator
- [x] LVGL Integration

## APPS (33 Total)

### Sector A: Smart Home & Ambient
- [ ] Weather Crystal (Dummy → Funktionsfähig → v1.0.0) ← FIRST
- [ ] Thermostat (Dummy)
- [ ] Light Controller (Dummy)
- [ ] ... (6 weitere)

### Sector B: Desktop & Workflow
- [ ] Pomodoro Timer (Dummy → Funktionsfähig → v1.0.0) ← THIRD
- [ ] Performance HUD (Dummy)
- [ ] ... (5 weitere)

### Sector C: Industrial & Professional
- [ ] WiFi Analyzer (Dummy → Funktionsfähig → v1.0.0) ← SECOND
- [ ] 3D Printer Monitor (Dummy)
- [ ] ... (3 weitere)

### Sector D: Automotive & Mobility
- [ ] OBD2 Dashboard (Dummy)
- [ ] ... (3 weitere)

### Sector E: Lifestyle & Gadgets
- [ ] Cyberpunk Watch (Dummy)
- [ ] ... (4 weitere)

### Sector F: Security & Tools
- [ ] 2FA Token (Dummy)
- [ ] ... (3 weitere)

## SUMMARY
- **Framework**: ✅ Complete
- **Apps (Dummy)**: 0 / 33
- **Apps (Functional)**: 0 / 3
- **Overall Progress**: 0%
```

---

## 🎯 BEST PRACTICES FÜR DICH

### 1. Immer Golden Path befolgen
```
NVS → I2C → WiFi → Display → Touch → LVGL → Backlight → NTP → UI → Carousel
```

### 2. Fehlertoleranz ist Pflicht
```c
// ✅ RICHTIG - Graceful Degradation
esp_err_t err = wifi_init();
if (err != ESP_OK) {
    ESP_LOGW(TAG, "WiFi init failed, continuing without WiFi");
}

// ❌ FALSCH - Hard Reset bei Fehler
ESP_ERROR_CHECK(wifi_init());
```

### 3. Event Bus verwenden, nicht Timer
```c
// ✅ RICHTIG - Apps abonnieren Events
event_subscribe(EVENT_WIFI_SCAN_DONE, app_on_wifi_scan);

// ❌ FALSCH - Jede App hat eigenen Timer
lv_timer_create(app_update_timer, 1000, NULL);
```

### 4. Theme Colors verwenden, nicht hardcoded
```c
// ✅ RICHTIG - Zentrale Farb-Verwaltung
lv_obj_set_style_bg_color(obj, THEME_COLOR(bg_primary), 0);

// ❌ FALSCH - Hardcoded RGB-Werte
lv_obj_set_style_bg_color(obj, lv_color_hex(0x1a1a1a), 0);
```

### 5. Panel Gap nicht vergessen!
```c
// KRITISCH für Rotation 90°!
esp_lcd_panel_set_gap(panel_handle, 0, 34);
```

---

## 🔍 DEBUGGING TIPPS

### Serial Monitor
```bash
# Standard
pio run -t monitor

# Mit Filter
pio run -t monitor --print_filter "WEATHER"

# Beenden: Ctrl+]
```

### Heap-Überwachung
```c
ESP_LOGI(TAG, "Free heap: %ld bytes", esp_get_free_heap_size());
```

### Task-Monitoring
```c
char task_list[1024];
vTaskList(task_list);
ESP_LOGI(TAG, "Tasks:\n%s", task_list);
```

---

## 📚 RESSOURCEN

- **LVGL Docs**: https://docs.lvgl.io/
- **ESP32-S3 Datasheet**: https://www.espressif.com/sites/default/files/documentation/esp32-s3_datasheet_en.pdf
- **PlatformIO Docs**: https://docs.platformio.org/
- **Waveshare Docs**: https://www.waveshare.com/wiki/ESP32-S3-Touch-LCD-1.47

---

## ✅ DEINE AUFGABEN (In Reihenfolge)

### Woche 1: Foundation
- [ ] Projekt-Struktur aufbauen
- [ ] platformio.ini erstellen
- [ ] main.c mit Golden Path
- [ ] HAL: Display (mit Panel Gap!)
- [ ] HAL: Touch
- [ ] HAL: WiFi
- [ ] HAL: NTP
- [ ] Framework: Event Bus
- [ ] Framework: Theme Manager
- [ ] Startup Validator
- [ ] Erste Build & Flash testen

### Woche 2: Erste 3 Apps (Dummies)
- [ ] Weather Crystal (Dummy)
- [ ] WiFi Analyzer (Dummy)
- [ ] Pomodoro Timer (Dummy)
- [ ] Carousel-Navigation testen

### Woche 3: Erste 3 Apps (Funktionsfähig)
- [ ] Weather Crystal (Funktionsfähig)
- [ ] WiFi Analyzer (Funktionsfähig)
- [ ] Pomodoro Timer (Funktionsfähig)

### Woche 4: Alle 33 Apps (Dummies)
- [ ] Alle 33 Apps als Grundgerüste
- [ ] App-Registry aktualisieren
- [ ] Carousel mit allen 33 Apps testen

### Woche 5+: Weitere Apps (Funktionsfähig)
- [ ] Iterativ weitere Apps implementieren
- [ ] Performance optimieren
- [ ] Dokumentation
- [ ] Release v1.0.0

---

## 🚀 LOS GEHT'S!

Du hast alles, was du brauchst:
- ✅ Blueprint (Architektur)
- ✅ Best Practices (Lessons Learned)
- ✅ Code-Templates (Ready-to-use)
- ✅ Checklisten (Strukturiert)

**Starte mit Woche 1, Schritt 1: Projekt-Struktur aufbauen.**

Viel Erfolg! 🎯
