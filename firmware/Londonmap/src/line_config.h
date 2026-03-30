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
constexpr uint8_t TRAIN_COUNT = 2;
constexpr uint32_t SERIAL_BAUD = 115200;
constexpr uint8_t BRIGHTNESS = 33; // 32 = 7% 
constexpr uint16_t FADE_INTERVAL_MS = 20;
constexpr uint8_t FADE_AMOUNT = 20;

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
constexpr uint16_t BASE_TRAIN_PROGRESS_SPEED_MS = 1000;

constexpr LineDef kLines[] = {
    {"Bakerloo", LINE_BAKERLOO, CountOf(LINE_BAKERLOO), CRGB(120, 70, 0), static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 120)},
    {"Central", LINE_CENTRAL, CountOf(LINE_CENTRAL), CRGB::Red, static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 180)},
    {"Circle", LINE_CIRCLE, CountOf(LINE_CIRCLE), CRGB::Yellow, static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 260)},
    {"District", LINE_DISTRICT, CountOf(LINE_DISTRICT), CRGB::Green, static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 220)},
    {"H&C", LINE_HAMMERSMITH_AND_CITY, CountOf(LINE_HAMMERSMITH_AND_CITY), CRGB::Pink, static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 310)},
    {"Jubilee", LINE_JUBILEE, CountOf(LINE_JUBILEE), CRGB::White, static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 150)},
    {"Metropolitan", LINE_METROPOLITAN, CountOf(LINE_METROPOLITAN), CRGB::Purple, static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 210)},
    {"Northern", LINE_NORTHERN, CountOf(LINE_NORTHERN), CRGB(50, 50, 50), static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 330)},
    {"Piccadilly", LINE_PICCADILLY, CountOf(LINE_PICCADILLY), CRGB::Blue, static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 190)},
    {"Victoria", LINE_VICTORIA, CountOf(LINE_VICTORIA), CRGB::LightBlue, static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 140)},
    {"W&C", LINE_WATERLOO_AND_CITY, CountOf(LINE_WATERLOO_AND_CITY), CRGB(0, 180, 160), static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 80)},
    {"DLR", LINE_DOCKLANDS_LIGHT_RAILWAY_DLR, CountOf(LINE_DOCKLANDS_LIGHT_RAILWAY_DLR), CRGB(0, 220, 140), static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 240)},
    {"Overground", LINE_LONDON_OVERGROUND, CountOf(LINE_LONDON_OVERGROUND), CRGB::Orange, static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 200)},
    {"Elizabeth", LINE_ELIZABETH, CountOf(LINE_ELIZABETH), CRGB(130, 0, 200), static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 160)},
    {"Trams", LINE_LONDON_TRAMS, CountOf(LINE_LONDON_TRAMS), CRGB(0, 200, 60), static_cast<uint16_t>(BASE_TRAIN_PROGRESS_SPEED_MS + 280)},
};

constexpr size_t LINE_COUNT = sizeof(kLines) / sizeof(kLines[0]);
