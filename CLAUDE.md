# CLAUDE.md — London Underground PCB

This file helps AI assistants understand the project and make correct changes.

---

## Project Overview

An interactive London Underground map PCB with 600 individually addressable RGB LEDs. Each LED represents a station. Animated "trains" travel along tube lines in real time. Built with a Seeed Studio XIAO ESP32C3 and FastLED.

Related prior project: [Washington Metro PCB](https://github.com/funvill/washington-metro-pcb)

---

## Repository Layout

```
London-Underground-PCB/
├── firmware/Londonmap/         # PlatformIO Arduino project
│   └── src/
│       ├── version.h           # Version numbers (update on every firmware change)
│       ├── stations.h          # Station name → LED index constants
│       ├── lines.h             # Ordered station arrays per tube line
│       ├── line_config.h       # Hardware settings, LineDef structs, kLines[] table
│       ├── main.cpp            # Arduino setup/loop, animation engine, test mode
│       └── utilities.cpp       # Utility stubs
├── hardware/                   # KiCAD 7 PCB design files
│   └── london-underground-pcb.kicad_*
├── design/                     # Affinity Designer source graphics and SVG exports
├── station-names-to-led.csv    # First-pass station→LED mapping (has errors, WIP)
├── stations-to-leds.txt        # First-pass line→LED-list mapping (has errors, WIP)
├── london_metro_stations_master_list.md  # Authoritative station list
├── Issues with version 2025-Nov-30.txt   # Known hardware issues
├── CLAUDE.md                   # This file
├── docs.md                     # Human/robot documentation
├── CHANGELOG.md                # Version history
└── README.md
```

---

## Firmware Architecture

### Key Source Files

| File | Purpose |
|------|---------|
| `version.h` | `VERSION_MAJOR/MINOR/PATCH` and `VERSION_STRING` macros |
| `stations.h` | `const uint16 STATION_NAME = LED_INDEX;` for every station |
| `lines.h` | Ordered `const uint16 LINE_X[]` arrays — one per tube line |
| `line_config.h` | `LineDef` struct, `kLines[]` table, hardware `#define`s |
| `main.cpp` | `setup()`, `loop()`, animation and test logic |

### Data Flow

```
stations.h  ──► lines.h  ──► line_config.h (kLines[])  ──► main.cpp
```

`stations.h` maps human-readable names to LED strip indices. `lines.h` references those names to define station sequences. `line_config.h` bundles each sequence with a color and speed. `main.cpp` consumes `kLines[]` to drive animations.

### Hardware Constants (line_config.h)

| Constant | Value | Notes |
|----------|-------|-------|
| `DATA_PIN` | D7 | XIAO ESP32C3 GPIO |
| `LED_TYPE` | WS2811 | Strip is actually WS2812B |
| `COLOR_ORDER` | GRB | |
| `NUM_LEDS` | 600 | Total LEDs on strip |
| `TRAIN_COUNT` | 2 | Trains per line in demo mode |
| `BRIGHTNESS` | 33 | ~7% — keep low to reduce heat/current |
| `FADE_INTERVAL_MS` | 20 | Fade tick period |
| `FADE_AMOUNT` | 20 | Fade-to-black amount per tick |
| `BASE_TRAIN_PROGRESS_SPEED_MS` | 1000 | Base move interval for all lines |

### Current Loop Mode

`loop()` currently calls `TestEachLine()` and returns early — the production animation (fading trains) is implemented but **disabled**. To switch to production mode, remove or comment out the `TestEachLine(...)` call and the `return;` that follows it.

`TestEachLine()` cycles through every tube line sequentially:
1. All stations on the current line illuminate green
2. A single red LED sweeps from first to last station (200 ms per step)
3. After 3 complete passes, moves to the next line

---

## How to Make Changes

### Add a new station LED mapping

1. In `stations.h`, add: `const uint16 NEW_STATION = LED_INDEX;`
   - Use `UNMAPPED_LED` (0xFFFF) if the physical LED does not exist on this board version
   - For stations with multiple LEDs (e.g. interchange points), use suffixes `_A`, `_B`, etc.

2. Reference the constant in the appropriate line array(s) in `lines.h`

3. Bump `VERSION_PATCH` in `version.h` and add an entry to `CHANGELOG.md`

### Fix a station-to-LED mapping error

1. Update the LED index in `stations.h`
2. Bump `VERSION_PATCH` in `version.h`
3. Document in `CHANGELOG.md`

### Add a new tube line

1. Add a `const uint16 LINE_NEW_LINE[]` array in `lines.h` with stations in order
2. Add a `LineDef` entry to `kLines[]` in `line_config.h` with name, array, color, speed
3. Bump `VERSION_MINOR` in `version.h`
4. Add to `CHANGELOG.md`

### Change animation behavior

1. Edit `main.cpp` (animation logic) or `line_config.h` (speeds/colors)
2. Bump `VERSION_MINOR` (new feature) or `VERSION_PATCH` (tweak/fix)
3. Update `CHANGELOG.md`

### Version numbering rules

- **PATCH** (x.x.+1): Bug fixes, corrected station mappings, label corrections
- **MINOR** (x.+1.0): New features, new lines, new animation modes
- **MAJOR** (+1.0.0): Breaking changes — pin reassignment, LED count change, new board revision

Always update `VERSION_STRING` to match `MAJOR.MINOR.PATCH`.

---

## Hardware

### PCB Versions

| Version | Date | Notes |
|---------|------|-------|
| v2025-Nov-30 | 2025-11-30 | First manufactured version; 31 known issues |

Hardware is hard to update (requires PCB fab). Track hardware issues in `Issues with version YYYY-Mon-DD.txt`.

### Components

| Part | Value | JLCPCB # |
|------|-------|-----------|
| LED | WS2812B 1.6×1.5mm (XL-1615RGBC) | C5349954 |
| MCU | Seeed Studio XIAO ESP32C3 | — |
| Button | SMD 4-pin 5.1×5.1mm | C318884 |
| Resistor | 0Ω 1206 (data line) | C17888 |

### Known Hardware Limitations (v2025-Nov-30)

- 9 stations have no physical LED: Bow Road, Monument, Barbican, Cannon Street, Eastcote, Euston Square, High Street Kensington, South Hampstead, and a few others
- See `Issues with version 2025-Nov-30.txt` for full list

---

## Station Mapping Files (WIP — Contains Errors)

`station-names-to-led.csv` and `stations-to-leds.txt` are **first-pass drafts** with known errors. Do not treat them as authoritative. The authoritative mapping is in `firmware/Londonmap/src/stations.h` and `lines.h`.

`london_metro_stations_master_list.md` is the canonical human-readable station list. Cross-reference it when correcting mappings.

---

## Build & Flash

```bash
cd firmware/Londonmap
pio run                  # Build
pio run --target upload  # Flash to XIAO ESP32C3 over USB
pio device monitor       # Serial monitor at 115200 baud
```

Requires [PlatformIO](https://platformio.org/) and the `espressif32` platform.
