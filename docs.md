# London Underground PCB — Project Documentation

> This document is updated alongside firmware and hardware changes. See [CHANGELOG.md](CHANGELOG.md) for version history.

---

## What is this?

A decorative PCB shaped like the London Underground map with 600 individually addressable RGB LEDs — one per station. The firmware animates "trains" travelling along each tube line in real time, using the correct line colors from the official TfL palette.

The board is powered and controlled by a **Seeed Studio XIAO ESP32C3** microcontroller soldered directly onto the PCB.

![Preview](https://github.com/funvill/London-Underground-PCB/blob/main/preview.png?raw=true)

[View PCB in browser (KiCanvas)](https://kicanvas.org/?github=https://github.com/funvill/London-Underground-PCB/tree/main/hardware)

Related project: [Washington Metro PCB](https://github.com/funvill/washington-metro-pcb)

Made by: [Steven (@funvill)](https://blog.abluestar.com/about)

---

## Hardware

### Current Version: v2025-Nov-30

Manufactured by JLCPCB, November 2025.

### Specifications

| Item | Value |
|------|-------|
| LEDs | 600× WS2812B (XL-1615RGBC, 1.6×1.5mm) |
| Microcontroller | Seeed Studio XIAO ESP32C3 |
| LED data pin | D7 |
| Power input | USB-C (via XIAO board) |
| LED color order | GRB |
| PCB design tool | KiCAD 7 |

### Components (JLCPCB Part Numbers)

| Component | Description | Part # |
|-----------|-------------|--------|
| WS2812B LED | 1.6×1.5mm RGB addressable | C5349954 |
| Button | SMD 4-pin 5.1×5.1mm | C318884 |
| Resistor | 0Ω 1206 | C17888 |

### Tube Lines on the Board

| Line | Color (RGB) | Stations |
|------|-------------|----------|
| Bakerloo | (120, 70, 0) brown | 25 |
| Central | Red | 45 |
| Circle | Yellow | 27 |
| District | Green | 49 |
| Hammersmith & City | Pink | 18 |
| Jubilee | White | 27 |
| Metropolitan | Purple | 31 |
| Northern | (50, 50, 50) dark gray | 50 |
| Piccadilly | Blue | 51 |
| Victoria | Light Blue | 16 |
| Waterloo & City | (0, 180, 160) teal | 2 |
| DLR | (0, 220, 140) light green | 38 |
| London Overground | Orange | 15 |
| Elizabeth line | (130, 0, 200) purple | 40 |
| London Trams | (0, 200, 60) green | varies |

### Known Hardware Issues (v2025-Nov-30)

See `Issues with version 2025-Nov-30.txt` for the full list of 31 identified issues. Key items:

**Missing LEDs — stations with no physical LED on this board revision:**
- Bow Road
- Monument
- Barbican
- Cannon Street
- Eastcote
- Euston Square
- High Street Kensington
- South Hampstead
- (plus Iver, Taplow, Maidenhead, Hillingdon, Herne Hill, and others)

**Assembly issues:**
- ESP32C3 pads too small for easy hand-soldering (extend pads 2mm in next revision)
- USB-C cable protrudes past board edge, making flush mounting difficult
- Need more GND test points on back of board
- XIAO pin numbers (e.g. D7) not silkscreened on PCB

**Labeling errors:**
- "Acton Town" label appears on both LED 14 and LED 65
- "Edgware Road" has two different locations (LED 134 vs LED 94)
- LED 101–104 location unknown
- "Colliers Wood" label reversed
- LED 480 is an arrow, not a station
- LED 482 "Gallions" should be "Gallions Reach"; "Cyprus Reach" should be "Cyprus"
- LED 552 labeled "Grants Hill" should be "Wanstead" (labels swapped)

---

## Firmware

### Current Version: v1.0.0

### Building & Flashing

Requirements: [PlatformIO](https://platformio.org/) with the `espressif32` platform.

```bash
cd firmware/Londonmap
pio run                   # compile
pio run --target upload   # flash over USB
pio device monitor        # serial monitor (115200 baud)
```

On boot the board prints:
```
London Underground PCB
https://github.com/funvill/London-Underground-PCB
Lines configured: 15
```

### Current Mode: Test Mode

The firmware currently runs **TestEachLine** mode to help verify the physical LED-to-station mapping. This is active while the mapping in `stations.h` / `lines.h` is being corrected.

Test mode behavior:
1. All stations on the active line illuminate **green**
2. A single **red** LED sweeps from the first station to the last (200 ms per step)
3. After 3 complete passes, the next line becomes active
4. After all 15 lines have been tested, the sequence repeats

Production animation mode (trains moving and fading) is implemented in `main.cpp` but disabled by a `return;` statement. Once the station mappings are verified, remove that `return;` to enable it.

### Source File Overview

| File | Role |
|------|------|
| `version.h` | Version macros — update every change |
| `stations.h` | `const uint16 STATION_NAME = LED_INDEX;` for all 570+ stations |
| `lines.h` | Ordered station sequences per line (`LINE_BAKERLOO[]`, etc.) |
| `line_config.h` | Hardware defines, `LineDef` struct, `kLines[]` table |
| `main.cpp` | `setup()`, `loop()`, test and animation logic |

### Station Mapping

Every station is a named constant in `stations.h`:

```cpp
const uint16 OXFORD_CIRCUS = 230;
const uint16 UNMAPPED_LED  = 0xFFFF;  // station exists but no LED on board
```

Stations that share a physical LED (interchange stations with a single dot on the map) use lettered suffixes:

```cpp
const uint16 BAKER_STREET_A = 116;  // Bakerloo/Jubilee platform
const uint16 BAKER_STREET_B = 117;  // Circle/Metropolitan platform
```

Lines reference these constants in order (direction doesn't matter — trains run both ways):

```cpp
const uint16 LINE_BAKERLOO[] = {
    ELEPHANT_CASTLE_A,
    LAMBETH_NORTH,
    WATERLOO_A,
    // ... 22 more stations ...
    HARROW_WEALDSTONE
};
```

### Adding or Fixing a Station Mapping

1. Edit the `const uint16 STATION_NAME = NEW_INDEX;` line in `stations.h`
2. If the station is on multiple lines, check all references in `lines.h`
3. Bump `VERSION_PATCH` in `version.h`
4. Add a `CHANGELOG.md` entry under `[Unreleased]`

### Adding a New Line

1. Add a `const uint16 LINE_NEW_NAME[]` array to `lines.h`
2. Add a `LineDef` entry to `kLines[]` in `line_config.h` (name, array pointer, color, speed)
3. Bump `VERSION_MINOR` in `version.h`
4. Document in `CHANGELOG.md`

---

## Station Mapping Reference Files

These files in the project root are **first-pass drafts with known errors**:

| File | Description |
|------|-------------|
| `station-names-to-led.csv` | Station name → LED index (first attempt, needs correction) |
| `stations-to-leds.txt` | Line name → comma-separated LED list (first attempt, needs correction) |

The **authoritative** mapping lives in `firmware/Londonmap/src/stations.h` and `lines.h`. Update those files, not the CSVs/TXTs.

`london_metro_stations_master_list.md` is the canonical list of all TfL stations. Use it as a reference when verifying or correcting mappings.

---

## Design Files

The `design/` folder contains Affinity Designer source files (`.afdesign`) and SVG/PNG exports used to generate the PCB silkscreen graphics. The London Underground map graphic was adapted into the PCB layout.

---

## Repository Files Quick Reference

| File / Folder | Purpose |
|---------------|---------|
| `README.md` | Project intro and links |
| `CLAUDE.md` | AI assistant guide for making changes |
| `docs.md` | This file — full project documentation |
| `CHANGELOG.md` | Version history |
| `firmware/` | PlatformIO firmware source |
| `hardware/` | KiCAD PCB design |
| `design/` | Affinity Designer graphics |
| `preview.png` | Board render |
| `london_metro_stations_master_list.md` | Canonical station list |
| `station-names-to-led.csv` | Draft mapping (WIP, has errors) |
| `stations-to-leds.txt` | Draft line sequences (WIP, has errors) |
| `Issues with version 2025-Nov-30.txt` | Known hardware issues |
