# 🦎 CHAMELEON OS: MASTER BLUEPRINT

> **Status**: ✅ FINAL & READY FOR AGENT  
> **Basis**: Demo V6 Stable (funktioniert, getestet, 30 FPS)  
> **Target**: Waveshare ESP32-S3 1.47" Touch-LCD  
> **Engine**: LVGL v8.4.0 + PlatformIO  
> **Architektur**: 4-Schichten, modular, skalierbar

---

## 🎯 VISION (Für dich & den Agent)

**Ein Gerät. Eine Hardware. 33 verschiedene Möglichkeiten.**

Chameleon OS ist eine **Produktionsplattform**:
1. Bestellung kommt rein: "WiFi Analyzer"
2. Vorkonfigurierter ESP32-S3 wird angeschlossen
3. Agent sagt: "Das ist für WiFi Analyzer"
4. Agent spielt auf: Alle notwendigen Apps und Konfigurationen
5. Gerät ist produktionsreif

**Ergebnis**: Jedes Produkt fühlt sich spezialisiert an, obwohl alle die gleiche Hardware nutzen.

---

## 🏗️ HARDWARE-CORE (Verified by Demo V6)

### A. Display & Touch (The "Golden Setup")

**Display**: JD9853 Controller, SPI (80 MHz), Rotation 90°, Gap (0, 34)

```c
// GPIO-Belegung (NICHT ÄNDERN!)
#define LCD_CS    GPIO_NUM_21    // Chip Select
#define LCD_SCLK  GPIO_NUM_38    // SPI Clock
#define LCD_MOSI  GPIO_NUM_39    // SPI MOSI
#define LCD_RST   GPIO_NUM_40    // Reset
#define LCD_DC    GPIO_NUM_45    // Data/Command
#define LCD_BL    GPIO_NUM_46    // Backlight (PWM)

// Panel Gap (KRITISCH bei Rotation 90°!)
esp_lcd_panel_set_gap(panel_handle, 0, 34);

// Buffer Configuration
#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 172
#define DISPLAY_DRAW_BUFF_HEIGHT 50  // Optimal für DMA
#define DISPLAY_DOUBLE_BUFFER 1      // Double Buffering
```

**Touch**: AXS5106 Controller, I2C (shared mit IMU)

```c
// GPIO-Belegung
#define I2C_SDA   GPIO_NUM_42    // I2C Data
#define I2C_SCL   GPIO_NUM_41    // I2C Clock
#define TP_INT    GPIO_NUM_47    // Touch Interrupt
#define TP_RST    GPIO_NUM_48    // Touch Reset

// I2C Konfiguration
#define I2C_PORT_NUM 0
#define I2C_FREQ_HZ 400000
```

### B. Shared I2C & Sensors

**Shared Bus**: Touch und IMU (QMI8658) teilen sich GPIO 41/42

```c
// IMU QMI8658 (I2C Adresse: 0x6B)
// WICHTIG: Non-Fatal Init!
esp_err_t imu_ret = bsp_qmi8658_init(i2c_bus_handle);
if (imu_ret != ESP_OK) {
    ESP_LOGW(TAG, "QMI8658 init failed, continue without IMU");
    // System läuft auch ohne IMU!
}
```

**Battery**: ADC1_CH0 (GPIO 1), Voltage Divider 3x

```c
#define BATTERY_ADC_CHANNEL ADC_CHANNEL_1  // GPIO1
#define BATTERY_ADC_UNIT ADC_UNIT_2
#define BATTERY_ADC_ATTEN ADC_ATTEN_DB_12

// Spannung in Prozent umrechnen
float percent = ((voltage - 3.0f) / (4.2f - 3.0f)) * 100.0f;
```

### C. Dual-Core Distribution

```
Core 0 (System):
├─ WiFi (Station Mode)
├─ NTP Time-Sync
├─ Battery-ADC
└─ Hintergrund-Daten

Core 1 (Experience):
├─ LVGL Port Task
├─ Animationen
├─ Carousel-Logik
└─ Touch-Handling
```

---

## 🎨 CHAMELEON OS ARCHITEKTUR (4 Schichten)

### Layer 1: HAL (Hardware Abstraction)

```c
// Aus Demo V6 direkt übernehmen!
src/hal/
├── display.c/h      // bsp_display.c
├── touch.c/h        // bsp_touch.c
├── i2c.c/h          // bsp_i2c.c
├── wifi.c/h         // bsp_wifi.c
├── ntp.c/h          // NTP Integration
├── battery.c/h      // bsp_battery.c
└── hal.h            // HAL Interface
```

**Golden Path Initialisierung**:
```
1. NVS
2. I2C Bus
3. IMU (non-fatal!)
4. Battery
5. WiFi (VOR Display!)
6. Display (mit Panel Gap!)
7. Touch
8. LVGL
9. Backlight
10. NTP (nach WiFi)
11. Event Bus
12. Theme Manager
13. Startup Validator
14. UI
15. Carousel
```

### Layer 2: Framework

```c
src/framework/
├── event_bus.c/h         // Event-Driven Architecture
├── theme_manager.c/h     // Zentrale Theme-Verwaltung
├── carousel.c/h          // Focus-Carousel Navigation
└── gesture.c/h           // Swipe, Long-Press, Tap
```

**Theme Manager** (Semantische Farben):
```c
// Luxe Theme (aus Demo V6)
#define UI_COL_BG_PRIMARY       lv_color_hex(0xEFE9E1)  // Eggshell
#define UI_COL_TEXT_PRIMARY     lv_color_hex(0x322D29)  // Dark Brown
#define UI_COL_ACCENT_PRIMARY   lv_color_hex(0x72383D)  // Burgundy

// Verwendung in Apps
lv_obj_set_style_bg_color(obj, UI_COL_BG_PRIMARY, 0);
```

**Focus-Carousel** (Z-Achsen Animation):
```c
// Beim Scrollen wird der LV_EVENT_SCROLL-Handler genutzt:
// Distanz Zentrum = 0: Scale 100%, Opacity 255
// Distanz Zentrum > 100: Scale 70%, Opacity 100

lv_obj_t *carousel = lv_tileview_create(lv_scr_act());
lv_obj_add_event_cb(carousel, carousel_scroll_handler, LV_EVENT_SCROLL, NULL);
```

### Layer 3: Apps (33 Dummies)

```c
src/apps/
├── app_interface.h       // Standard App-Interface
├── app_registry.c        // App-Registrierung
├── sector_a/             // Smart Home (8 Apps)
│   ├── weather_crystal/
│   ├── thermostat/
│   └── ...
├── sector_b/             // Desktop & Workflow (6 Apps)
│   ├── pomodoro_timer/
│   └── ...
├── sector_c/             // Industrial (5 Apps)
│   ├── wifi_analyzer/
│   └── ...
├── sector_d/             // Automotive (4 Apps)
├── sector_e/             // Lifestyle (5 Apps)
└── sector_f/             // Security (5 Apps)
```

**App-Template**:
```c
// Jede App folgt diesem Interface
typedef struct {
    const char *app_name;
    const char *app_version;
    
    void (*app_init)(void);
    void (*app_deinit)(void);
    void (*app_start)(void);
    void (*app_stop)(void);
    
    void (*app_render)(void);
    void (*app_update)(void);
    
    void (*app_on_touch)(touch_event_t event);
    void (*app_on_button)(button_event_t event);
} app_t;
```

### Layer 4: Quality & Operations

```c
src/
├── startup_validator.c   // Boot-Diagnose
├── performance_monitor.c // FPS, RAM, CPU
└── config.h              // Globale Konfiguration
```

---

## 🚀 CHAMELEON OS ROADMAP (Ready for Agent)

### Phase 1: Infrastruktur (The "Clean-Up")
- [ ] Umzug von Demo V6 in neue Struktur
- [ ] Implementierung ThemeManager.h
- [ ] Integration WiFi-Credentials (git-ignored)
- [ ] Erste Build & Flash erfolgreich

### Phase 2: Focus-Carousel
- [ ] Implementierung Z-Achsen Animation
- [ ] Hardware-Button (GPIO 0) als Home-Button
- [ ] Carousel mit 3 Test-Apps
- [ ] Swipe-Navigation funktioniert

### Phase 3: Erste 3 Apps (Dummy → Funktionsfähig)
- [ ] Weather Crystal (Dummy → Funktionsfähig)
- [ ] WiFi Analyzer (Dummy → Funktionsfähig)
- [ ] Pomodoro Timer (Dummy → Funktionsfähig)

### Phase 4: Alle 33 Apps (Dummies)
- [ ] Alle 33 App-Grundgerüste
- [ ] App-Registry aktualisieren
- [ ] Carousel mit allen 33 Apps

### Phase 5: Weitere Apps (Funktionsfähig)
- [ ] Iterativ weitere Apps implementieren
- [ ] Performance optimieren (60 FPS)
- [ ] Dokumentation
- [ ] Release v1.0.0

---

## 🤖 MASTER-PROMPT FÜR VS CODE AGENT

**Kopiere diesen Block direkt in deinen Agent (Cursor, Copilot, Claude):**

```
🦎 CHAMELEON OS MASTER BLUEPRINT

Kontext:
Du baust auf dem stabilen Projekt "Demo V6" auf. Hardware-Vorgaben sind nicht verhandelbar:
- GPIO-Belegung: LCD_CS=21, LCD_BL=46, I2C=41/42 (aus V6)
- Display benötigt Gap (0, 34) bei 90° Rotation
- Double-Buffering im internen RAM (320 * 50)
- Non-Fatal Init für optionale Hardware (IMU, Battery)
- WiFi MUSS vor Display initialisiert werden

Deine Aufgaben (in dieser Reihenfolge):

1. INFRASTRUKTUR (Phase 1)
   - Kopiere Demo V6 Code in neue Struktur (lib/esp_bsp/, lib/esp_lcd_*)
   - Erstelle HAL-Wrapper (src/hal/hal.c) um Demo V6 BSP
   - Migriere zu PlatformIO (platformio.ini)
   - Implementiere Golden Path Initialisierung in main.c
   - Erste Build & Flash erfolgreich

2. FRAMEWORK (Phase 2)
   - Implementiere Event Bus (src/framework/event_bus.c)
   - Implementiere Theme Manager (src/framework/theme_manager.c)
   - Implementiere Focus-Carousel (src/framework/carousel.c)
   - Implementiere Gesture Recognition (src/framework/gesture.c)

3. ERSTE 3 APPS (Phase 3)
   - Weather Crystal: Dummy → Funktionsfähig
   - WiFi Analyzer: Dummy → Funktionsfähig
   - Pomodoro Timer: Dummy → Funktionsfähig

4. ALLE 33 APPS (Phase 4+)
   - Alle 33 App-Dummies erstellen
   - Carousel mit allen 33 Apps
   - Iterativ weitere Apps funktionsfähig machen

Anforderungen:
- Strikt fehlertolerant (Non-fatal Init)
- Optimiert auf 60 FPS (Double-Buffering, DMA)
- Semantische Farben verwenden (nicht hardcoded RGB)
- Event Bus statt Timer-Chaos
- Jede App folgt standardisiertem Interface
- Git-Workflow: Dummy → Funktionsfähig → Abgelegt

Ressourcen:
- AGENT_BRIEFING.md: Detaillierte Anleitung
- CHAMELEON_V6_INTEGRATION.md: Code-Übernahme
- CHAMELEON_LESSONS_LEARNED.md: Hardware-Bibel
- BEST_PRACTICES.md: Best Practices
- MASTER_BLUEPRINT.md: Dieses Dokument
```

---

## 📋 REPOSITORY-STRUKTUR (Final)

```
chameleon-os/
├── README.md                          # Repository-Übersicht
├── platformio.ini                     # PlatformIO Konfiguration
├── .gitignore                         # Git Ignore (wifi_credentials.h, etc.)
│
├── src/
│   ├── main.c                         # Golden Path Initialisierung
│   ├── config.h                       # Globale Konfiguration
│   ├── wifi_credentials.h             # WiFi SSID/Pass (git-ignored)
│   ├── startup_validator.c            # Boot-Diagnose
│   │
│   ├── hal/                           # Hardware Abstraction Layer
│   │   ├── hal.h                      # HAL Interface
│   │   ├── hal.c                      # HAL Wrapper
│   │   ├── display.c/h                # Display (aus Demo V6)
│   │   ├── touch.c/h                  # Touch (aus Demo V6)
│   │   ├── i2c.c/h                    # I2C Bus (aus Demo V6)
│   │   ├── wifi.c/h                   # WiFi (aus Demo V6)
│   │   ├── ntp.c/h                    # NTP Integration
│   │   ├── battery.c/h                # Battery ADC (aus Demo V6)
│   │   └── sdcard.c/h                 # SD Card (aus Demo V6)
│   │
│   ├── framework/                     # Framework Layer
│   │   ├── event_bus.c/h              # Event-Driven Architecture
│   │   ├── theme_manager.c/h          # Zentrale Theme-Verwaltung
│   │   ├── carousel.c/h               # Focus-Carousel Navigation
│   │   └── gesture.c/h                # Swipe, Long-Press, Tap
│   │
│   ├── apps/                          # Die 33 Apps
│   │   ├── app_interface.h            # Standard App-Interface
│   │   ├── app_registry.c             # App-Registrierung
│   │   ├── sector_a/                  # Smart Home (8 Apps)
│   │   │   ├── weather_crystal/
│   │   │   │   ├── weather.c/h
│   │   │   │   └── ui.c
│   │   │   ├── thermostat/
│   │   │   └── ...
│   │   ├── sector_b/                  # Desktop & Workflow (6 Apps)
│   │   │   ├── pomodoro_timer/
│   │   │   └── ...
│   │   ├── sector_c/                  # Industrial (5 Apps)
│   │   │   ├── wifi_analyzer/
│   │   │   └── ...
│   │   ├── sector_d/                  # Automotive (4 Apps)
│   │   ├── sector_e/                  # Lifestyle (5 Apps)
│   │   └── sector_f/                  # Security (5 Apps)
│   │
│   └── assets/                        # Bilder, Fonts, Icons
│       ├── fonts/
│       ├── icons/
│       └── themes/
│
├── lib/                               # Externe Libraries & BSP
│   ├── esp_bsp/                       # Board Support Package (aus Demo V6)
│   │   ├── bsp_display.c/h
│   │   ├── bsp_touch.c/h
│   │   ├── bsp_i2c.c/h
│   │   ├── bsp_wifi.c/h
│   │   ├── bsp_qmi8658.c/h
│   │   ├── bsp_battery.c/h
│   │   └── bsp_sdcard.c/h
│   ├── esp_lcd_jd9853/                # Display-Treiber (aus Demo V6)
│   └── esp_lcd_touch_axs5106/         # Touch-Treiber (aus Demo V6)
│
├── docs/                              # Dokumentation
│   ├── ARCHITECTURE.md                # Architektur-Übersicht
│   ├── BEST_PRACTICES.md              # Best Practices & Lessons Learned
│   ├── APP_DEVELOPMENT.md             # Wie man neue Apps entwickelt
│   ├── PROJECT_STATUS.md              # Aktueller Entwicklungs-Status
│   └── PLAYBOOK.md                    # Operationale Anleitung
│
└── .github/
    └── workflows/                     # GitHub Actions (optional)
        └── build.yml                  # Auto-Build & Test
```

---

## ✅ HANDOVER-CHECKLISTE

### Vor dem Start (Für dich):
- [x] Gemini-Feedback integriert
- [x] Demo V6 Code-Übernahme geplant
- [x] Golden Path dokumentiert
- [x] Master-Prompt erstellt
- [x] Repository-Struktur definiert
- [ ] GitHub Repository erstellen
- [ ] Alle Dokumente ins Repo pushen
- [ ] VS Code Agent briefen

### Für VS Code Agent (Nach Handover):
- [ ] MASTER_BLUEPRINT.md lesen
- [ ] AGENT_BRIEFING.md lesen
- [ ] CHAMELEON_V6_INTEGRATION.md lesen
- [ ] Phase 1: Infrastruktur implementieren
- [ ] Phase 2: Focus-Carousel implementieren
- [ ] Phase 3: Erste 3 Apps implementieren
- [ ] Phase 4+: Weitere Apps implementieren

---

## 🎯 ERFOLGS-KRITERIEN

### Nach Phase 1 (Infrastruktur)
- ✅ Build erfolgreich: `pio run`
- ✅ Flash erfolgreich: `pio run -t upload`
- ✅ Display zeigt schwarzen Screen
- ✅ Touch reagiert
- ✅ WiFi verbindet
- ✅ NTP synchronisiert
- ✅ Keine Reset-Loops
- ✅ Heap > 50 KB frei

### Nach Phase 2 (Focus-Carousel)
- ✅ Carousel mit 3 Test-Apps
- ✅ Swipe-Navigation funktioniert
- ✅ Z-Achsen Animation funktioniert
- ✅ Hardware-Button (GPIO 0) funktioniert
- ✅ Performance: 30+ FPS

### Nach Phase 3 (Erste 3 Apps)
- ✅ Weather Crystal funktioniert (Echte Wetterdaten)
- ✅ WiFi Analyzer funktioniert (Live WiFi-Scan)
- ✅ Pomodoro Timer funktioniert (Echtzeit-Countdown)
- ✅ Event Bus funktioniert
- ✅ Theme-Wechsel funktioniert

### Nach Phase 4+ (Alle 33 Apps)
- ✅ Alle 33 Apps als Dummies
- ✅ Carousel mit allen 33 Apps
- ✅ Weitere Apps iterativ funktionsfähig
- ✅ Dokumentation vollständig
- ✅ Tests bestanden
- ✅ Release v1.0.0 vorbereitet

---

## 🚀 NÄCHSTE SCHRITTE (KONKRET)

### Schritt 1: GitHub Repository erstellen
```bash
# Auf GitHub.com:
1. Neues Repository: "chameleon-os"
2. Private oder Public (deine Wahl)
3. Mit README.md initialisieren

# Lokal:
git clone https://github.com/yourusername/chameleon-os.git
cd chameleon-os
```

### Schritt 2: Alle Dokumentationen pushen
```bash
# Alle .md Dateien ins Repo:
cp /home/ubuntu/*.md chameleon-os/docs/
cp /home/ubuntu/README_REPO.md chameleon-os/README.md
cp /home/ubuntu/AGENT_BRIEFING.md chameleon-os/
cp /home/ubuntu/MASTER_BLUEPRINT.md chameleon-os/

git add .
git commit -m "Initial commit: Chameleon OS Master Blueprint & Documentation"
git push origin main
```

### Schritt 3: VS Code Agent briefen
```
Gib dem Agent diesen Block:

"Hier ist dein Chameleon OS Repository: https://github.com/yourusername/chameleon-os

Lese zuerst:
1. README.md (Übersicht)
2. MASTER_BLUEPRINT.md (Dieses Dokument)
3. AGENT_BRIEFING.md (Detaillierte Anleitung)
4. CHAMELEON_V6_INTEGRATION.md (Code-Übernahme)

Dann starte mit Phase 1: Infrastruktur"
```

---

## 📞 ROLLEN & VERANTWORTUNG

| Rolle | Verantwortung | Werkzeuge |
|-------|---------------|----------|
| **System Architect (Manus)** | Gesamtkonzept, Architektur, Qualität | Planung, Dokumentation, Validierung |
| **VS Code Agent** | Implementierung, Code-Entwicklung | VS Code, Git, PlatformIO |
| **Gemini** | Strategische Inputs, Optimierungen | Feedback, Verbesserungsvorschläge |
| **Du** | Entscheidungen, Feedback, Richtung | GitHub, Kommunikation |

---

## 🎓 ZUSAMMENFASSUNG

**Chameleon OS ist jetzt bereit zum Starten:**

✅ **Konzept**: Klar definiert, validiert, dokumentiert  
✅ **Architektur**: 4-Schichten, modular, skalierbar  
✅ **Hardware**: Auf Basis von Demo V6 (funktioniert, getestet)  
✅ **Code-Basis**: Bewährter Code, nicht von Grund auf neu  
✅ **Roadmap**: 5 Phasen, strukturiert, messbar  
✅ **Agent-Briefing**: Detailliert, ausführbar, klar  
✅ **Repository**: Struktur definiert, bereit zum Pushen  

**Das System ist "rund" - kohärent, professionell, produktionsreif.**

---

**Chameleon OS: Ein Gerät. Eine Hardware. 33 verschiedene Möglichkeiten. 🦎**

**Bereit zum Starten?**
