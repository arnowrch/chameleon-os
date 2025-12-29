# ESP32-S3 Luxury UI Kit (LVGL 9 + ESP-IDF)

Dieses Projekt dient als **State-of-the-Art** Basis-Framework für die Entwicklung von Touch-UIs auf dem ESP32-S3 mit einem 172x320 Display und LVGL 9.

Das Design folgt einer klaren **Luxury UI** Philosophie (inspiriert von Tesla, Apple Home) mit Fokus auf Dunkel-Theme, Klarheit, Reaktionsgeschwindigkeit und Touch-Optimierung.

## ⚙️ Technische Spezifikationen

*   **MCU:** ESP32-S3
*   **Framework:** ESP-IDF v5.x
*   **UI-Bibliothek:** LVGL v9
*   **Display:** 172x320 (Portrait), ST7789-kompatibel
*   **Touch:** I2C-Touch-Controller (z.B. CST816S, AXS5106)

## ✨ Design-Standards (UI-Theme)

| Merkmal | Spezifikation |
| :--- | :--- |
| **Auflösung** | 172x320 (Portrait) |
| **Farben** | Dunkelbasis (`#0B0B0B`) + Akzentfarben (Blau, Grün, Orange, Rot) |
| **Fonts** | Montserrat (22px Bold, 18px Medium, 12px Regular) |
| **Abstände** | 4px-Raster (UI_SPACE_* Konstanten) |
| **Radius** | 8px (UI_RADIUS_SM) bis 16px (UI_RADIUS_LG) |
| **Animationen** | 200ms `ease-in-out` (UI_ANIM_NORMAL) |

## 🏗️ Projektstruktur

| Verzeichnis | Beschreibung |
| :--- | :--- |
| `main/` | Hauptanwendungscode, UI-Logik, Router, Theme und Komponenten. |
| `components/bsp/` | Board Support Package (BSP) für Display, Touch, WiFi, Batterie etc. |
| `components/lvgl_touch_drivers/` | Spezifische LVGL-Touch-Treiber (z.B. `esp_lcd_jd9853.c`). |
| `sdkconfig` | ESP-IDF Konfiguration (optimiert für LVGL und PSRAM). |

## 🚀 Erste Schritte (Kompilierung)

1.  **Voraussetzungen:** Stellen Sie sicher, dass ESP-IDF v5.x installiert und eingerichtet ist.
2.  **Projekt klonen:** Kopieren Sie dieses Verzeichnis auf Ihren Rechner.
3.  **Konfiguration:**
    ```bash
    cd esp32_luxury_ui
    idf.py menuconfig
    ```
    *   Überprüfen Sie die Einstellungen unter `Component config -> LVGL` und `Component config -> ESP-LCD`.
    *   Stellen Sie sicher, dass die GPIO-Pins in den BSP-Dateien (`components/bsp/bsp_*.c`) mit Ihrer Hardware übereinstimmen.
4.  **Kompilieren und Flashen:**
    ```bash
    idf.py build
    idf.py -p /dev/ttyUSB0 flash monitor
    ```

## 💻 VS Code Integration (Empfohlen)

Für die beste Entwicklungserfahrung mit dem Agenten und VS Code:

1.  Installieren Sie die **"Espressif IDF"** Erweiterung.
2.  Öffnen Sie den Ordner `esp32_luxury_ui` in VS Code.
3.  Die Erweiterung erkennt automatisch das Projekt und bietet Funktionen wie `Build`, `Flash` und `Monitor` in der Statusleiste an.

---

### 💡 Hinweise für Agent-Entwicklung (z.B. Sonnet 4.5)

*   **Nutze die Komponenten:** Verwenden Sie die Funktionen aus `ui_components.h` (z.B. `ui_make_tile()`, `ui_make_button()`) anstelle von direktem `lv_obj_create()`, um das Design-System beizubehalten.
*   **Navigation:** Verwenden Sie `ui_router_go(SCREEN_ID_...)` für den Wechsel zwischen Bildschirmen.
*   **Styling:** Vermeiden Sie `lv_obj_set_style_*` und nutzen Sie stattdessen die Konstanten und Stile aus `ui_theme.h`.

Dieses Framework ist darauf ausgelegt, die Entwicklung zu beschleunigen und die Konsistenz des luxuriösen Designs zu gewährleisten.
