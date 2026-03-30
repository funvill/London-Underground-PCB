# Changelog

All notable changes to this project will be documented here.

Firmware versions follow [Semantic Versioning](https://semver.org/): `MAJOR.MINOR.PATCH`
- MAJOR: incompatible changes (e.g. pin reassignment, LED count change)
- MINOR: new features (e.g. new animation mode, new line added)
- PATCH: bug fixes (e.g. corrected station-to-LED mapping)

Hardware versions use the date format `YYYY-Mon-DD`.

---

## [Unreleased]

---

## Hardware v2025-Nov-30

**PCB manufactured by JLCPCB.**

- 600 WS2812B LEDs arranged in London Underground map layout
- Seeed Studio XIAO ESP32C3 microcontroller
- USB-C power input via ESP32C3 board

**Known issues (31 total — see `Issues with version 2025-Nov-30.txt`):**
- 9 stations have no LED on the board (Bow Road, Monument, Barbican, Cannon Street, Eastcote, Euston Square, High Street Kensington, South Hampstead, and others)
- 9 LED positions on board are unlabeled or unknown
- ESP32C3 pads too small for easy hand-soldering; USB-C cable protrudes past board edge
- Several silkscreen labels incorrect or misplaced

---

## Firmware v1.0.0 — 2026-03-29

**Initial versioned release. Corresponds to hardware v2025-Nov-30 test phase.**

- Added `version.h` with `VERSION_MAJOR`, `VERSION_MINOR`, `VERSION_PATCH`, `VERSION_STRING`
- Firmware currently runs `TestEachLine()` mode: cycles through each tube line, illuminates all stations green with a red LED moving station-by-station
- Supports 15 tube lines: Bakerloo, Central, Circle, District, H&C, Jubilee, Metropolitan, Northern, Piccadilly, Victoria, Waterloo & City, DLR, Overground, Elizabeth, Trams
- Station-to-LED mapping in `stations.h`; line sequences in `lines.h`
- 9 stations assigned `UNMAPPED_LED` (0xFFFF) pending hardware fix
- Production animation (moving trains with fade) implemented but disabled pending LED mapping verification
