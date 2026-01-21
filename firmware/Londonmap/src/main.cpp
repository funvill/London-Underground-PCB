#include <Arduino.h>
#include <FastLED.h>

#include <line_config.h>
#include <stations.h>

CRGB leds[NUM_LEDS];

static_assert(LINE_COUNT > 0, "kLines[] must contain at least one line");

static int16_t g_trainOffset[LINE_COUNT][TRAIN_COUNT];
static uint32_t g_lastMoveMs[LINE_COUNT];
static uint32_t g_lastFadeMs = 0;

void SetTrainStartingPositions()
{
  for (size_t lineIndex = 0; lineIndex < LINE_COUNT; lineIndex++)
  {
    const uint16_t stationCount = kLines[lineIndex].stationCount;
    for (uint8_t trainIndex = 0; trainIndex < TRAIN_COUNT; trainIndex++)
    {
      const uint16_t spacing = (TRAIN_COUNT == 0) ? 0 : static_cast<uint16_t>(stationCount / TRAIN_COUNT);
      g_trainOffset[lineIndex][trainIndex] = static_cast<int16_t>(spacing * trainIndex);
    }
    g_lastMoveMs[lineIndex] = 0;
  }
}

void ProgressDemoTrain(const size_t lineIndex)
{
  const uint16_t stationCount = kLines[lineIndex].stationCount;
  if (stationCount == 0)
  {
    return;
  }

  for (uint8_t trainIndex = 0; trainIndex < TRAIN_COUNT; trainIndex++)
  {
    // Even trains move forwards, odd trains move backwards.
    if ((trainIndex % 2) == 0)
    {
      g_trainOffset[lineIndex][trainIndex]++;
    }
    else
    {
      g_trainOffset[lineIndex][trainIndex]--;
    }

    if (g_trainOffset[lineIndex][trainIndex] >= static_cast<int16_t>(stationCount))
    {
      g_trainOffset[lineIndex][trainIndex] = 0;
    }
    else if (g_trainOffset[lineIndex][trainIndex] < 0)
    {
      g_trainOffset[lineIndex][trainIndex] = static_cast<int16_t>(stationCount - 1);
    }
  }
}

void DrawTrains()
{
  for (size_t lineIndex = 0; lineIndex < LINE_COUNT; lineIndex++)
  {
    const LineDef &line = kLines[lineIndex];
    if (line.stationCount == 0)
    {
      continue;
    }

    for (uint8_t trainIndex = 0; trainIndex < TRAIN_COUNT; trainIndex++)
    {
      const uint16_t stationIndex = static_cast<uint16_t>(g_trainOffset[lineIndex][trainIndex]);
      const uint16 ledIndex = line.stations[stationIndex];

      if (ledIndex != UNMAPPED_LED && ledIndex < NUM_LEDS)
      {
        leds[ledIndex] = line.color;
      }
    }
  }
}

void setup()
{
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  Serial.begin(SERIAL_BAUD);
  Serial.println("London Underground PCB");
  Serial.println("https://github.com/funvill/London-Underground-PCB");
  Serial.print("Lines configured: ");
  Serial.println(static_cast<uint32_t>(LINE_COUNT));

  SetTrainStartingPositions();
}

// In this test
// Only one train line is illuminated at a time. All other train lines are dark.
// All the LEDS of that train line are illuminated GREEN
// A single RED LED moves from one end of the train line to the other with a TIME_LED_MOVE ms delay
//
// This allows me to check each train line to ensure that all the correct LEDS are illuminated, 
// and that they are in the correct order
//
// The sequence should loop LOOPS_PER_TRAIN_LINE_COUNT then move on to the next train line.

static size_t g_testLineIndex = 0;
static uint16_t g_testStationIndex = 0;
static uint16_t g_testLoopCount = 0;
static uint32_t g_lastTestMoveMs = 0;

void TestEachLine(const uint32_t now, const uint16_t timeLedMoveMs, const uint16_t loopsPerTrainLine) {
  if ((now - g_lastTestMoveMs) < timeLedMoveMs) {
    return;
  }
  g_lastTestMoveMs = now;

  // Clear all LEDs
  fill_solid(leds, NUM_LEDS, CRGB::Black);

  const LineDef &line = kLines[g_testLineIndex];
  
  // Set all stations on current line to GREEN
  for (uint16_t i = 0; i < line.stationCount; i++) {
    const uint16_t ledIndex = line.stations[i];
    if (ledIndex != UNMAPPED_LED && ledIndex < NUM_LEDS) {
      leds[ledIndex] = CRGB::Green;
    }
  }

  // Set current station to RED
  if (g_testStationIndex < line.stationCount) {
    const uint16_t ledIndex = line.stations[g_testStationIndex];
    if (ledIndex != UNMAPPED_LED && ledIndex < NUM_LEDS) {
      leds[ledIndex] = CRGB::Red;
    }
  }

  FastLED.show();

  // Progress to next station
  g_testStationIndex++;
  if (g_testStationIndex >= line.stationCount) {
    g_testStationIndex = 0;
    g_testLoopCount++;
    
    // Move to next line after completing loops
    if (g_testLoopCount >= loopsPerTrainLine) {
      g_testLoopCount = 0;
      g_testLineIndex++;
      
      if (g_testLineIndex >= LINE_COUNT) {
        g_testLineIndex = 0;
      }
    }
  }
}

void loop()
{
  const uint32_t now = millis();

  const uint32_t TIME_LED_MOVE = 200;
  const uint16_t LOOPS_PER_TRAIN_LINE_COUNT = 3;
  TestEachLine( now, TIME_LED_MOVE, LOOPS_PER_TRAIN_LINE_COUNT);
  return; 

  // Move trains (each line independently)
  for (size_t lineIndex = 0; lineIndex < LINE_COUNT; lineIndex++)
  {
    const uint16_t interval = kLines[lineIndex].moveEveryMs;
    if ((now - g_lastMoveMs[lineIndex]) >= interval)
    {
      g_lastMoveMs[lineIndex] = now;
      ProgressDemoTrain(lineIndex);
    }
  }

  // Fade + draw + show
  if ((now - g_lastFadeMs) >= FADE_INTERVAL_MS)
  {
    g_lastFadeMs = now;
    fadeToBlackBy(leds, NUM_LEDS, FADE_AMOUNT);
    DrawTrains();
    FastLED.show();
  }
}