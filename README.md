# 🦎 CHAMELEON OS

> **Ein universelles Betriebssystem für ESP32-S3 mit Touch-Display, das sich in 33 verschiedene spezialisierte Apps verwandelt.**

---

## 🎯 Vision

**Ein Gerät. Eine Hardware. 33 verschiedene Möglichkeiten.**

Chameleon OS ist nicht einfach eine Firmware. Es ist eine **Produktionsplattform**:

1. **Bestellung kommt rein**: "Wir brauchen einen WiFi Analyzer"
2. **Vorkonfigurierter ESP32-S3 wird angeschlossen**
3. **Agent sagt**: "Das ist für WiFi Analyzer"
4. **Agent spielt auf**: Alle notwendigen Apps und Konfigurationen
5. **Gerät ist produktionsreif**: Verbauen im Gehäuse

**Ergebnis**: Jedes Produkt fühlt sich spezialisiert an, obwohl alle die gleiche Hardware nutzen.

---

## 🏗️ ARCHITEKTUR

```
┌─────────────────────────────────────────────────────────────┐
│ CHAMELEON OS: 4-SCHICHTEN-ARCHITEKTUR                       │
├─────────────────────────────────────────────────────────────┤
│                                                              │
│ LAYER 1: HARDWARE-ABSTRACTION (HAL)                        │
│ ├─ Display (ST7789 + JD9853, Panel Gap 34, Rotation 90°)  │
│ ├─ Touch (CST816S, I2C)                                    │
│ ├─ WiFi (Auto-Reconnect, Reentrancy Guards)               │
│ └─ NTP (SNTP, elegante Sync-Animation)                    │
│                                                              │
│ LAYER 2: FRAMEWORK                                          │
│ ├─ Event Bus (Sensoren → Apps, nicht Timer-Chaos)         │
│ ├─ Theme Manager (Semantische Farben + Style-Objekte)     │
│ ├─ Carousel Navigation (Focus-Scaling, Animations)        │
│ └─ Gesture Recognition (Swipe, Long-Press, Tap)           │
│                                                              │
│ LAYER 3: APPS (33 Dummies → Funktionsfähig)              │
│ ├─ Sector A: Smart Home (Thermostat, Weather, etc.)       │
│ ├─ Sector B: Desktop & Workflow (Pomodoro, etc.)          │
│ ├─ Sector C: Industrial (WiFi Analyzer, etc.)             │
│ ├─ Sector D: Automotive                                    │
│ ├─ Sector E: Lifestyle                                     │
│ └─ Sector F: Security                                      │
│                                                              │
│ LAYER 4: QUALITY & OPERATIONS                              │
│ ├─ Startup Validator (Automatische Hardware-Checks)        │
│ ├─ Performance Monitor (FPS, RAM, CPU)                     │
│ ├─ Build Automation (PlatformIO)                           │
│ └─ Git Workflow (Portable Struktur)                        │
│                                                              │
└─────────────────────────────────────────────────────────────┘
```

---

## 📊 DIE 33 APPS

### Sector A: Smart Home & Ambient (8 Apps)
- 🌤️ **Weather Crystal** - Visuelle Wetteranimationen
- 🌡️ Thermostat - Temperaturregelung
- 💡 Light Controller - Beleuchtungskontrolle
- 🎵 Ambient Sound - Soundscape-Player
- 🌙 Sleep Timer - Schlaf-Visualisierung
- 🌬️ Air Quality - Luftqualitäts-Monitor
- 📊 Energy Monitor - Energieverbrauch
- 🔔 Smart Doorbell - Türklingel-Monitor

### Sector B: Desktop & Workflow (6 Apps)
- ⏱️ **Pomodoro Timer** - Fokus-Visualisierung
- 📝 Note Pad - Schnelle Notizen
- 📊 Performance HUD - System-Metriken
- 🎯 Goal Tracker - Ziel-Verfolgung
- 📅 Calendar - Terminkalender
- ⏰ World Clock - Weltzeiten

### Sector C: Industrial & Professional (5 Apps)
- 📡 **WiFi Analyzer** - WiFi-Spektrum-Analyse
- 🖨️ 3D Printer Monitor - Druck-Überwachung
- 🔧 Device Manager - Geräteverwaltung
- 📈 Data Logger - Datenerfassung
- 🔬 Sensor Dashboard - Sensor-Visualisierung

### Sector D: Automotive & Mobility (4 Apps)
- 🚗 OBD2 Dashboard - Fahrzeug-Diagnose
- 🗺️ Navigation HUD - Navigations-Display
- ⛽ Fuel Monitor - Tankstellen-Finder
- 🚦 Traffic Monitor - Verkehrsinformationen

### Sector E: Lifestyle & Gadgets (5 Apps)
- 👓 Cyberpunk Watch - Futuristische Uhr
- 🎮 Game Launcher - Spiele-Hub
- 🎨 Color Picker - Farbauswahl-Tool
- 🎵 Music Player - Audio-Steuerung
- 📸 Photo Viewer - Bildanzeige

### Sector F: Security & Tools (5 Apps)
- 🔐 2FA Token - Zwei-Faktor-Authentifizierung
- 🔑 Password Manager - Passwort-Speicher
- 🛡️ VPN Monitor - VPN-Status
- 📱 Device Tracker - Geräte-Ortung
- 🔒 Encryption Tool - Verschlüsselung

---

## 🚀 QUICK START

### Hardware-Anforderungen
- Waveshare ESP32-S3-Touch-LCD-1.47
- USB-C Kabel
- TF-Card (optional, für Assets)

### Software-Anforderungen
- PlatformIO (VS Code Extension oder CLI)
- Git

### Installation

```bash
# 1. Repository klonen
git clone https://github.com/yourusername/chameleon-os.git
cd chameleon-os

# 2. Abhängigkeiten installieren
pio pkg install

# 3. Build
pio run

# 4. Flash
pio run -t upload

# 5. Monitor
pio run -t monitor
```

---

## 📖 DOKUMENTATION

- **[ARCHITECTURE.md](docs/ARCHITECTURE.md)** - Detaillierte Architektur-Übersicht
- **[BEST_PRACTICES.md](docs/BEST_PRACTICES.md)** - Lessons Learned & Best Practices
- **[APP_DEVELOPMENT.md](docs/APP_DEVELOPMENT.md)** - Wie man neue Apps entwickelt
- **[AGENT_BRIEFING.md](AGENT_BRIEFING.md)** - Anleitung für KI-Agenten
- **[PROJECT_STATUS.md](docs/PROJECT_STATUS.md)** - Aktueller Entwicklungs-Status

---

## 🎯 ENTWICKLUNGS-WORKFLOW

### Für jede App: Dummy → Funktionsfähig → Abgelegt

**Phase 1: DUMMY ERSTELLEN** (Tag 1)
```bash
git checkout -b feature/weather-crystal
# Erstelle App-Grundgerüst mit statischer UI
git commit -m "Weather Crystal: Dummy v1.0"
git push origin feature/weather-crystal
```

**Phase 2: FUNKTIONSFÄHIG MACHEN** (Tag 2-3)
```bash
# Implementiere echte Funktionalität
# Integriere mit Event Bus
# Optimiere Performance
git commit -m "Weather Crystal: Feature Complete v1.0"
```

**Phase 3: ABGELEGT & DOKUMENTIERT** (Tag 4)
```bash
# Code-Review
# Tests
# Dokumentation
git checkout develop && git pull
git merge feature/weather-crystal
git tag weather_crystal/v1.0.0
git push origin develop weather_crystal/v1.0.0
```

---

## 🤖 FÜR KI-AGENTEN

Wenn du ein KI-Agent bist (Claude, Cursor, Copilot, etc.), lese zuerst:

**→ [AGENT_BRIEFING.md](AGENT_BRIEFING.md)**

Dieses Dokument enthält:
- ✅ Komplette Anleitung zur Implementierung
- ✅ Code-Templates (ready-to-use)
- ✅ Checklisten (strukturiert)
- ✅ Best Practices (bewährt)
- ✅ Debugging-Tipps (praktisch)

---

## 📊 PROJECT STATUS

| Component | Status | Version |
|-----------|--------|---------|
| Framework | ✅ Complete | 1.0.0 |
| Weather Crystal | ⏳ In Progress | Dummy |
| WiFi Analyzer | ⏳ In Progress | Dummy |
| Pomodoro Timer | ⏳ In Progress | Dummy |
| Other 30 Apps | ⏳ Planned | - |

**Detaillierter Status**: [PROJECT_STATUS.md](docs/PROJECT_STATUS.md)

---

## 🔧 TOOLCHAIN

- **MCU**: ESP32-S3 Dual-Core (240 MHz)
- **Display**: Waveshare 320×172 RGB (JD9853)
- **Touch**: CST816S (I2C)
- **RAM**: 8 MB PSRAM
- **Build System**: PlatformIO
- **Framework**: LVGL v9
- **Language**: C

---

## 📝 LIZENZ

MIT License - Siehe [LICENSE](LICENSE) für Details

---

## 🤝 BEITRAGEN

Contributions sind willkommen! Bitte:

1. Fork das Repository
2. Erstelle einen Feature-Branch (`git checkout -b feature/amazing-feature`)
3. Commit deine Änderungen (`git commit -m 'Add amazing feature'`)
4. Push zum Branch (`git push origin feature/amazing-feature`)
5. Öffne einen Pull Request

---

## 📞 SUPPORT

- **Issues**: [GitHub Issues](https://github.com/yourusername/chameleon-os/issues)
- **Discussions**: [GitHub Discussions](https://github.com/yourusername/chameleon-os/discussions)
- **Documentation**: [docs/](docs/)

---

## 🎓 LESSONS LEARNED

Dieses Projekt basiert auf umfangreichen Erfahrungen mit ESP32-S3 Entwicklung:

- ✅ Hardware-Initialisierung (Fehlertoleranz ist Pflicht)
- ✅ Display-Konfiguration (Panel Gap nicht vergessen!)
- ✅ WiFi-Integration (Reentrancy Guards essentiell)
- ✅ LVGL Optimierung (Draw Buffer Sizing)
- ✅ Theme-System (semantische Farben)
- ✅ Event-Driven Architecture (statt Timer-Chaos)

Siehe [BEST_PRACTICES.md](docs/BEST_PRACTICES.md) für Details.

---

## 🚀 ROADMAP

### Phase 1: Foundation ✅
- Projekt-Struktur
- HAL (Display, Touch, WiFi, NTP)
- Framework (Event Bus, Theme Manager, Carousel)
- Startup Validator

### Phase 2: Erste 3 Apps
- Weather Crystal (Dummy → Funktionsfähig)
- WiFi Analyzer (Dummy → Funktionsfähig)
- Pomodoro Timer (Dummy → Funktionsfähig)

### Phase 3: Alle 33 Apps (Dummies)
- Alle App-Grundgerüste
- App-Registry
- Carousel mit allen Apps

### Phase 4: Weitere Apps (Funktionsfähig)
- Iterativ weitere Apps implementieren
- Performance optimieren
- Dokumentation

### Phase 5: Release v1.0.0
- Stabilitäts-Tests
- Performance-Optimierung
- Finale Dokumentation

---

**Chameleon OS: Ein Gerät. Eine Hardware. 33 verschiedene Möglichkeiten. 🦎**

*Viel Erfolg!*
