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

## Firmware v1.3.3 — 2026-03-30

**TEST mode: pause and quit controls.**

- Feature: Press `SPACE` while in TEST mode to toggle pause/resume. LEDs hold their current state while paused, making it easy to read and copy serial output. Console prints `[PAUSED]` / `[RESUMED]`.
- Feature: Press `Q` while in TEST mode to quit immediately and return to RUN mode. Console prints `[TEST mode ended] Returning to RUN mode.`
- Both keys are processed immediately (not line-buffered), so no Enter key is needed.
- Updated `printHelp()` to document both keys.

---

## Firmware v1.3.2 — 2026-03-29

**Bug fixes.**

- Fix: Added `Serial.setTxTimeoutMs(0)` after `Serial.begin()` to prevent device freeze when USB is connected for power only (no serial host). ESP32C3 USB-CDC blocked indefinitely on TX queue without this.
- Fix: Removed duplicate `NOTTING_HILL_GATE` from `LINE_CENTRAL_A` in `lines.h`. `NOTTING_HILL_GATE` and `LANCASTER_GATE` both map to LED 230 (same physical PCB LED). Having both in the array caused LED 230 to appear at two non-adjacent steps. Correct Central line order is now `QUEENSWAY (229) → LANCASTER_GATE (230)`.

---

## Firmware v1.3.1 — 2026-03-29

**Diagnostic improvements: TEST mode serial logging and new audit commands.**

- Added `tools/generate_station_names.py` — reads `stations.h` and generates
  `firmware/Londonmap/src/station_names.h`: a 552-entry sorted lookup table
  mapping LED strip indices to station constant names and human-readable names.
- TEST mode now prints a serial log line for every step:
  `[TrackName (#N)]  step X/Y  LED:NNN  CONST_NAME  "Human Name"`
- TEST mode slowed to 500 ms/step (was 200 ms) and 2 loops per line (was 3)
  so the console log is readable in real time.
- Current station marker changed from pure red to orange-red (255,80,0)
  for better contrast against green background.
- New command `TEST <name>` — jump directly to a specific named line in TEST mode.
- New command `LINES` — list all configured line names, indices and station counts.
- New command `TRACKINFO <name>` — dump every station in a line with LED index,
  constant name, and human-readable name.
- New command `WHICH <N>` — show which lines and offsets reference a given LED index.
- New command `REPORT <track> <offset> <actual_led>` — user reports that at a given
  offset in a track, a different LED lit up; firmware prints a comparison and a
  suggested `lines.h` fix (e.g. `Change LINE_CENTRAL_A[5] from EAST_ACTON to ...`).
- Updated `printHelp()` to reflect all new commands.
- Build: SUCCESS — 21.9% Flash, 5.0% RAM.

---

## Firmware v1.3.0 — 2026-03-29

**Comprehensive line definitions rewrite (Phase 5): branch splitting and route corrections.**

- Added `FARRINGDON = UNMAPPED_LED` to `stations.h` (was omitted; no physical LED on board)
- Rewrote `lines.h` completely — all lines with multiple destinations are now split into
  named variants (A/B/C) rather than a single mangled array:
  - `LINE_CENTRAL_A/B/C` — main trunk + Ealing Broadway branch + Hainault loop
  - `LINE_CIRCLE` — now the complete 27-station clockwise loop (previously truncated)
  - `LINE_DISTRICT_A/B/C` — main trunk (Upminster→Wimbledon) + Richmond branch + Ealing Broadway branch
  - `LINE_HAMMERSMITH_AND_CITY` — FARRINGDON now included (UNMAPPED_LED)
  - `LINE_METROPOLITAN_A/B/C` — main trunk (Aldgate→Amersham) + Chesham spur + Watford branch;
    Uxbridge/Ruislip/Eastcote/Rayners Lane removed (Piccadilly-only since 2003)
  - `LINE_NORTHERN_A/B/C` — Charing Cross branch (Morden→Edgware) + Bank branch
    (Battersea→High Barnet, now includes BOROUGH/LONDON_BRIDGE/OLD_STREET/ANGEL) + Mill Hill East spur
  - `LINE_PICCADILLY_A/B/C` — main trunk (Cockfosters→Heathrow T5) + Terminal 4 loop + Uxbridge branch
  - `LINE_DLR_A/B/C/D` — Beckton trunk + Woolwich Arsenal branch + Stratford→Lewisham + Stratford High St spur
  - `LINE_OVERGROUND_A/B/C/D` — Mildmay (NLL) + Windrush (ELL) + Suffragette (Gospel Oak→Barking) + Lioness (Watford)
  - `LINE_ELIZABETH_A/B` — main trunk (Reading→Shenfield) + Heathrow branch
  - `LINE_TRAMS_A/B/C` — New Addington branch + Beckenham Junction branch + Elmers End branch
- Updated `kLines[]` in `line_config.h` — 36 entries (up from 15), one per route variant
- Build: SUCCESS — 19.7% Flash, 5.0% RAM

---

## Firmware v1.2.0 — 2026-03-29

**Corrected station-to-LED mapping (Phase 4).**

- Fixed root bug: `stations.h` previously stored 1-based KiCAD reference numbers
  (e.g. `ACTON_TOWN_A = 14` meaning LED14) instead of 0-based strip indices
  (correct: `ACTON_TOWN_A = 13`, the 14th LED at strip position 13)
- All 559 station constants updated to 0-based indices via `tools/generate_stations_h.py`
- 11 stations remain `UNMAPPED_LED` (no physical LED on this board version):
  Barbican, Bow Road, Cannon Street, Eastcote, Euston Square, High Street
  Kensington, Monument, New Barnet, South Hampstead, and 2 others
- Station mapping sourced from `tools/led_map_verified.csv` (webcam-verified positions)
- Added `tools/generate_stations_h.py`: regenerates `stations.h` from verified CSV;
  handles suffix constants (`_A`/`_B`/`_C`/`_D`), shared-LED aliases, manual overrides,
  and fuzzy name matching between C++ constants and CSV labels
- Added `tools/stations_h_report.txt`: change log produced by the generator

---

## Firmware v1.1.0 — 2026-03-29

**Serial control + RUN mode default.**

- Default boot mode is now `RUN` (fading train animation) instead of `TestEachLine`
- Added mode state machine: `enum Mode { RUN, TEST, SERIAL_CONTROL }`
- Added serial command set (115200 baud, newline-terminated):
  - `L <N>` — light LED index N in white
  - `S` — sequential scan, LED 0 to NUM_LEDS-1, 500 ms each
  - `T <N>` — show all stations on line index N in that line's color
  - `TRACK <NAME>` — same as `T` but addressed by line name (case-insensitive)
  - `RUN` — switch to production fading-train animation
  - `TEST` — switch to `TestEachLine` mode
  - `C` — clear all LEDs
  - `I` — print firmware version, line count, NUM_LEDS, current mode
  - `B <0-255>` — set brightness
- Firmware version and command list printed to serial on boot
- Added `#include <version.h>` to `main.cpp`

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
