#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include <lines.h>

// -----------------------------------------------------------------------------
// Hardware / animation settings
// -----------------------------------------------------------------------------

// FastLED non-type template parameters are simplest as macros.
#define DATA_PIN D7
#define LED_TYPE WS2811
#define COLOR_ORDER GRB

// LEDs on the strip.
constexpr uint16_t NUM_LEDS = 600;

// Demo behavior.
constexpr uint8_t TRAIN_COUNT = 1;
constexpr uint32_t SERIAL_BAUD = 115200;
constexpr uint8_t BRIGHTNESS = 33; // 32 = 7% 
constexpr uint16_t FADE_INTERVAL_MS = 20;
constexpr uint8_t FADE_AMOUNT = 10;

struct LineDef
{
  const char *name;
  const uint16 *stations;
  uint16_t stationCount;
  CRGB color;
  uint16_t moveEveryMs;
};

template <size_t N>
constexpr uint16_t CountOf(const uint16 (&)[N])
{
  static_assert(N <= 65535, "Line too long");
  return static_cast<uint16_t>(N);
}

// Add/remove lines here. The animation engine in main.cpp is data-driven.
// Colors are simple approximations; adjust to taste.
constexpr uint16_t BASE_TRAIN_PROGRESS_SPEED_MS = 1500;

// clang-format off
constexpr LineDef kLines[] = {
    // Bakerloo — single trunk, brown
    {"Bakerloo",    LINE_BAKERLOO,          CountOf(LINE_BAKERLOO),          CRGB(120, 70, 0),   static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 120)},

    // Central — three segments, red
    {"Central-A",   LINE_CENTRAL_A,         CountOf(LINE_CENTRAL_A),         CRGB::Red,          static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 180)},
    {"Central-B",   LINE_CENTRAL_B,         CountOf(LINE_CENTRAL_B),         CRGB::Red,          static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 180)},
    {"Central-C",   LINE_CENTRAL_C,         CountOf(LINE_CENTRAL_C),         CRGB::Red,          static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 180)},

    // Circle — complete loop, yellow
    {"Circle",      LINE_CIRCLE,            CountOf(LINE_CIRCLE),            CRGB::Yellow,       static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 260)},

    // District — three branches, green
    {"District-A",  LINE_DISTRICT_A,        CountOf(LINE_DISTRICT_A),        CRGB(0, 150, 0),    static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 220)},
    {"District-B",  LINE_DISTRICT_B,        CountOf(LINE_DISTRICT_B),        CRGB(0, 150, 0),    static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 220)},
    {"District-C",  LINE_DISTRICT_C,        CountOf(LINE_DISTRICT_C),        CRGB(0, 150, 0),    static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 220)},

    // Hammersmith & City — single trunk, pink
    {"H&C",         LINE_HAMMERSMITH_AND_CITY, CountOf(LINE_HAMMERSMITH_AND_CITY), CRGB(220, 90, 150), static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 310)},

    // Jubilee — single trunk, silver/white
    {"Jubilee",     LINE_JUBILEE,           CountOf(LINE_JUBILEE),           CRGB(160, 160, 160),static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 150)},

    // Metropolitan — three segments, purple
    {"Met-A",       LINE_METROPOLITAN_A,    CountOf(LINE_METROPOLITAN_A),    CRGB(130, 0, 180),  static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 210)},
    {"Met-B",       LINE_METROPOLITAN_B,    CountOf(LINE_METROPOLITAN_B),    CRGB(130, 0, 180),  static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 210)},
    {"Met-C",       LINE_METROPOLITAN_C,    CountOf(LINE_METROPOLITAN_C),    CRGB(130, 0, 180),  static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 210)},

    // Northern — three segments, near-black/dark grey
    {"Northern-A",  LINE_NORTHERN_A,        CountOf(LINE_NORTHERN_A),        CRGB(50, 50, 50),   static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 330)},
    {"Northern-B",  LINE_NORTHERN_B,        CountOf(LINE_NORTHERN_B),        CRGB(50, 50, 50),   static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 330)},
    {"Northern-C",  LINE_NORTHERN_C,        CountOf(LINE_NORTHERN_C),        CRGB(50, 50, 50),   static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 330)},

    // Piccadilly — three segments, dark blue
    {"Picca-A",     LINE_PICCADILLY_A,      CountOf(LINE_PICCADILLY_A),      CRGB(0, 0, 200),    static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 190)},
    {"Picca-B",     LINE_PICCADILLY_B,      CountOf(LINE_PICCADILLY_B),      CRGB(0, 0, 200),    static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 190)},
    {"Picca-C",     LINE_PICCADILLY_C,      CountOf(LINE_PICCADILLY_C),      CRGB(0, 0, 200),    static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 190)},

    // Victoria — single trunk, light blue
    {"Victoria",    LINE_VICTORIA,          CountOf(LINE_VICTORIA),          CRGB(0, 160, 220),  static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 140)},

    // Waterloo & City — single trunk, teal
    {"W&C",         LINE_WATERLOO_AND_CITY, CountOf(LINE_WATERLOO_AND_CITY), CRGB(0, 180, 160),  static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 80)},

    // DLR — four segments, teal-green
    {"DLR-A",       LINE_DLR_A,             CountOf(LINE_DLR_A),             CRGB(0, 200, 140),  static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 240)},
    {"DLR-B",       LINE_DLR_B,             CountOf(LINE_DLR_B),             CRGB(0, 200, 140),  static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 240)},
    {"DLR-C",       LINE_DLR_C,             CountOf(LINE_DLR_C),             CRGB(0, 200, 140),  static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 240)},
    {"DLR-D",       LINE_DLR_D,             CountOf(LINE_DLR_D),             CRGB(0, 200, 140),  static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 240)},

    // Overground — four routes, orange
    {"Overground-A",LINE_OVERGROUND_A,      CountOf(LINE_OVERGROUND_A),      CRGB(220, 100, 0),  static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 200)},
    {"Overground-B",LINE_OVERGROUND_B,      CountOf(LINE_OVERGROUND_B),      CRGB(220, 100, 0),  static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 200)},
    {"Overground-C",LINE_OVERGROUND_C,      CountOf(LINE_OVERGROUND_C),      CRGB(220, 100, 0),  static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 200)},
    {"Overground-D",LINE_OVERGROUND_D,      CountOf(LINE_OVERGROUND_D),      CRGB(220, 100, 0),  static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 200)},

    // Elizabeth — two segments, purple/magenta
    {"Elizabeth-A", LINE_ELIZABETH_A,       CountOf(LINE_ELIZABETH_A),       CRGB(130, 0, 200),  static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 160)},
    {"Elizabeth-B", LINE_ELIZABETH_B,       CountOf(LINE_ELIZABETH_B),       CRGB(130, 0, 200),  static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 160)},

    // Tramlink — three routes, bright green
    {"Trams-A",     LINE_TRAMS_A,           CountOf(LINE_TRAMS_A),           CRGB(0, 200, 60),   static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 280)},
    {"Trams-B",     LINE_TRAMS_B,           CountOf(LINE_TRAMS_B),           CRGB(0, 200, 60),   static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 280)},
    {"Trams-C",     LINE_TRAMS_C,           CountOf(LINE_TRAMS_C),           CRGB(0, 200, 60),   static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 280)},
};
// clang-format on

constexpr size_t LINE_COUNT = sizeof(kLines) / sizeof(kLines[0]);
