#include <Arduino.h>
#include <FastLED.h>

#include <line_config.h>
#include <station_names.h>
#include <stations.h>
#include <version.h>

CRGB leds[NUM_LEDS];

static_assert(LINE_COUNT > 0, "kLines[] must contain at least one line");

// ---------------------------------------------------------------------------
// Mode state machine
// ---------------------------------------------------------------------------

enum Mode { RUN, TEST, SERIAL_CONTROL };
static Mode g_mode = RUN;

// ---------------------------------------------------------------------------
// Train animation state
// ---------------------------------------------------------------------------

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

// ---------------------------------------------------------------------------
// Test mode — cycles through lines with green overlay and red position marker
// ---------------------------------------------------------------------------

static size_t   g_testLineIndex    = 0;
static uint16_t g_testStationIndex = 0;
static uint16_t g_testLoopCount    = 0;
static uint32_t g_lastTestMoveMs   = 0;
static bool     g_testPaused       = false;

// Print a single-line serial summary for the current test step.
// Format:  [LineName (lineIdx)] step/total  LED:NNN  CONST_NAME  "Human Name"
static void printTestStep(size_t lineIdx, uint16_t step)
{
  const LineDef &line = kLines[lineIdx];
  const uint16_t ledIndex = line.stations[step];

  // [Central-A (#1)] step  3/35
  Serial.print('[');
  Serial.print(line.name);
  Serial.print(" (#");
  Serial.print(static_cast<uint32_t>(lineIdx));
  Serial.print(")]  step ");
  if (step < 10)  Serial.print(' ');
  if (step < 100) Serial.print(' ');
  Serial.print(static_cast<uint32_t>(step));
  Serial.print('/');
  Serial.print(static_cast<uint32_t>(line.stationCount - 1));

  // LED index (or UNMAPPED)
  Serial.print("  LED:");
  if (ledIndex == UNMAPPED_LED)
  {
    Serial.print("UNMAPPED");
  }
  else
  {
    if (ledIndex < 100) Serial.print(' ');
    if (ledIndex < 10)  Serial.print(' ');
    Serial.print(static_cast<uint32_t>(ledIndex));
  }

  // Station names from lookup table
  if (ledIndex != UNMAPPED_LED)
  {
    const StationNameEntry *e = findStationEntry(ledIndex);
    if (e)
    {
      Serial.print("  ");
      Serial.print(e->constName);
      Serial.print("  \"");
      Serial.print(e->humanName);
      Serial.print('"');
    }
  }
  Serial.println();
}

void TestEachLine(const uint32_t now, const uint16_t timeLedMoveMs, const uint16_t loopsPerTrainLine)
{
  if (g_testPaused)
  {
    return; // paused — LEDs stay as-is until space is pressed again
  }
  if ((now - g_lastTestMoveMs) < timeLedMoveMs)
  {
    return;
  }
  g_lastTestMoveMs = now;

  fill_solid(leds, NUM_LEDS, CRGB::Black);

  const LineDef &line = kLines[g_testLineIndex];

  // All stations on current line → green
  for (uint16_t i = 0; i < line.stationCount; i++)
  {
    const uint16_t ledIndex = line.stations[i];
    if (ledIndex != UNMAPPED_LED && ledIndex < NUM_LEDS)
    {
      leds[ledIndex] = CRGB::Green;
    }
  }

  // Current station → bright red (yellow tint so it reads through green)
  if (g_testStationIndex < line.stationCount)
  {
    const uint16_t ledIndex = line.stations[g_testStationIndex];
    if (ledIndex != UNMAPPED_LED && ledIndex < NUM_LEDS)
    {
      leds[ledIndex] = CRGB(255, 80, 0); // vivid orange-red, distinct from green
    }
  }

  FastLED.show();

  // Serial log for this step
  printTestStep(g_testLineIndex, g_testStationIndex);

  g_testStationIndex++;
  if (g_testStationIndex >= line.stationCount)
  {
    g_testStationIndex = 0;
    g_testLoopCount++;

    if (g_testLoopCount >= loopsPerTrainLine)
    {
      g_testLoopCount = 0;
      g_testLineIndex = (g_testLineIndex + 1) % LINE_COUNT;

      // Announce the next line
      Serial.println();
      Serial.print("--- ");
      Serial.print(kLines[g_testLineIndex].name);
      Serial.print(" (#");
      Serial.print(static_cast<uint32_t>(g_testLineIndex));
      Serial.print(") --- ");
      Serial.print(static_cast<uint32_t>(kLines[g_testLineIndex].stationCount));
      Serial.println(" stations");
    }
    else
    {
      Serial.print("  >> loop ");
      Serial.print(static_cast<uint32_t>(g_testLoopCount + 1));
      Serial.print('/');
      Serial.println(static_cast<uint32_t>(loopsPerTrainLine));
    }
  }
}

// ---------------------------------------------------------------------------
// Serial command handling
// ---------------------------------------------------------------------------

static String g_serialBuf;

void printHelp()
{
  Serial.println("Commands:");
  Serial.println("  L <N>                   Light LED N in white");
  Serial.println("  LC <N> <R> <G> <B>      Light LED N with RGB color");
  Serial.println("  S                       Sequential scan (500ms each)");
  Serial.println("  T <N>                   Show line index N in its color");
  Serial.println("  TRACK <NAME>            Show named line (e.g. TRACK Central-A)");
  Serial.println("  RUN                     Fading-train animation");
  Serial.println("  TEST                    Step through all lines (500ms, 2 loops each)");
  Serial.println("  TEST <NAME>             Step through a specific named line");
  Serial.println("  SPACE                   (in TEST mode) Toggle pause/resume");
  Serial.println("  Q                       (in TEST mode) Quit test and return to RUN");
  Serial.println("  TRACKINFO <NAME>        Dump station list for a line");
  Serial.println("  LINES                   List all configured line names and indices");
  Serial.println("  WHICH <N>               Show which lines reference LED N");
  Serial.println("  REPORT <NAME> <OFF> <N> Report wrong LED: track, offset, actual LED seen");
  Serial.println("  C                       Clear all LEDs");
  Serial.println("  I                       Print firmware info");
  Serial.println("  B <0-255>               Set brightness");
}

// Find line index by name (case-insensitive). Returns -1 if not found.
static int findLineByName(const String &name)
{
  for (size_t i = 0; i < LINE_COUNT; i++)
  {
    if (String(kLines[i].name).equalsIgnoreCase(name))
    {
      return static_cast<int>(i);
    }
  }
  return -1;
}

static void cmdShowLine(int idx)
{
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  const LineDef &line = kLines[idx];
  for (uint16_t i = 0; i < line.stationCount; i++)
  {
    const uint16_t ledIndex = line.stations[i];
    if (ledIndex != UNMAPPED_LED && ledIndex < NUM_LEDS)
    {
      leds[ledIndex] = line.color;
    }
  }
  FastLED.show();
  Serial.print("Line ");
  Serial.print(idx);
  Serial.print(": ");
  Serial.println(line.name);
}

void handleSerialCommand(const String &cmd)
{
  String trimmed = cmd;
  trimmed.trim();
  if (trimmed.length() == 0)
  {
    return;
  }

  // --- RUN ---
  if (trimmed.equalsIgnoreCase("RUN"))
  {
    g_mode = RUN;
    SetTrainStartingPositions();
    Serial.println("Mode: RUN");
    return;
  }

  // --- TEST [optional name] ---
  if (trimmed.equalsIgnoreCase("TEST") ||
      (trimmed.length() > 5 && trimmed.substring(0, 5).equalsIgnoreCase("TEST ")))
  {
    g_mode = TEST;
    g_testStationIndex = 0;
    g_testLoopCount    = 0;
    g_lastTestMoveMs   = 0;

    if (trimmed.length() > 5)
    {
      // TEST <NAME> — jump to a specific named line
      const String name = trimmed.substring(5);
      const int idx = findLineByName(name);
      if (idx < 0)
      {
        Serial.print("Error: unknown line '");
        Serial.print(name);
        Serial.println("'");
        g_mode = SERIAL_CONTROL;
        return;
      }
      g_testLineIndex = static_cast<size_t>(idx);
    }
    else
    {
      g_testLineIndex = 0;
    }

    Serial.println("Mode: TEST (500ms step, 2 loops per line)");
    Serial.print("--- ");
    Serial.print(kLines[g_testLineIndex].name);
    Serial.print(" (#");
    Serial.print(static_cast<uint32_t>(g_testLineIndex));
    Serial.print(") --- ");
    Serial.print(static_cast<uint32_t>(kLines[g_testLineIndex].stationCount));
    Serial.println(" stations");
    return;
  }

  // --- C (clear) ---
  if (trimmed.equalsIgnoreCase("C"))
  {
    g_mode = SERIAL_CONTROL;
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
    Serial.println("Cleared");
    return;
  }

  // --- I (info) ---
  if (trimmed.equalsIgnoreCase("I"))
  {
    Serial.print("Firmware: ");
    Serial.println(VERSION_STRING);
    Serial.print("Lines:    ");
    Serial.println(static_cast<uint32_t>(LINE_COUNT));
    Serial.print("NUM_LEDS: ");
    Serial.println(static_cast<uint32_t>(NUM_LEDS));
    Serial.print("Mode:     ");
    if      (g_mode == RUN)            Serial.println("RUN");
    else if (g_mode == TEST)           Serial.println("TEST");
    else                               Serial.println("SERIAL_CONTROL");
    return;
  }

  // --- S (sequential scan) ---
  if (trimmed.equalsIgnoreCase("S"))
  {
    g_mode = SERIAL_CONTROL;
    Serial.print("Scanning ");
    Serial.print(NUM_LEDS);
    Serial.println(" LEDs (500ms each)...");
    for (uint16_t i = 0; i < NUM_LEDS; i++)
    {
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      leds[i] = CRGB::White;
      FastLED.show();
      Serial.print("LED ");
      Serial.println(i);
      delay(500);
    }
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
    Serial.println("Scan complete");
    return;
  }

  // --- LC <N> <R> <G> <B> ---
  if (trimmed.length() >= 7 && (trimmed[0] == 'L' || trimmed[0] == 'l') &&
      (trimmed[1] == 'C' || trimmed[1] == 'c') && trimmed[2] == ' ')
  {
    // Parse: LC <N> <R> <G> <B>
    const String rest = trimmed.substring(3);
    int sp1 = rest.indexOf(' ');
    int sp2 = (sp1 >= 0) ? rest.indexOf(' ', sp1 + 1) : -1;
    int sp3 = (sp2 >= 0) ? rest.indexOf(' ', sp2 + 1) : -1;
    if (sp1 < 0 || sp2 < 0 || sp3 < 0)
    {
      Serial.println("Error: LC usage: LC <N> <R> <G> <B>");
      return;
    }
    const int n = rest.substring(0, sp1).toInt();
    const int r = rest.substring(sp1 + 1, sp2).toInt();
    const int g = rest.substring(sp2 + 1, sp3).toInt();
    const int b = rest.substring(sp3 + 1).toInt();
    if (n < 0 || n >= static_cast<int>(NUM_LEDS) ||
        r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
    {
      Serial.println("Error: LC index or color value out of range");
      return;
    }
    g_mode = SERIAL_CONTROL;
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    leds[n] = CRGB(r, g, b);
    FastLED.show();
    Serial.print("LC ");
    Serial.print(n);
    Serial.print(" ");
    Serial.print(r);
    Serial.print(" ");
    Serial.print(g);
    Serial.print(" ");
    Serial.println(b);
    return;
  }

  // --- L <N> ---
  if (trimmed.length() >= 3 && (trimmed[0] == 'L' || trimmed[0] == 'l') && trimmed[1] == ' ')
  {
    const int n = trimmed.substring(2).toInt();
    if (n >= 0 && n < static_cast<int>(NUM_LEDS))
    {
      g_mode = SERIAL_CONTROL;
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      leds[n] = CRGB::White;
      FastLED.show();
      Serial.print("LED ");
      Serial.println(n);
    }
    else
    {
      Serial.print("Error: index out of range (0-");
      Serial.print(NUM_LEDS - 1);
      Serial.println(")");
    }
    return;
  }

  // --- T <N> ---
  if (trimmed.length() >= 3 && (trimmed[0] == 'T' || trimmed[0] == 't') && trimmed[1] == ' ')
  {
    const int n = trimmed.substring(2).toInt();
    if (n >= 0 && n < static_cast<int>(LINE_COUNT))
    {
      g_mode = SERIAL_CONTROL;
      cmdShowLine(n);
    }
    else
    {
      Serial.print("Error: line index out of range (0-");
      Serial.print(LINE_COUNT - 1);
      Serial.println(")");
    }
    return;
  }

  // --- TRACK <NAME> ---
  if (trimmed.length() > 6 && trimmed.substring(0, 6).equalsIgnoreCase("TRACK "))
  {
    const String name = trimmed.substring(6);
    const int idx = findLineByName(name);
    if (idx >= 0)
    {
      g_mode = SERIAL_CONTROL;
      cmdShowLine(idx);
    }
    else
    {
      Serial.print("Error: unknown line '");
      Serial.print(name);
      Serial.println("'");
    }
    return;
  }

  // --- B <0-255> ---
  if (trimmed.length() >= 3 && (trimmed[0] == 'B' || trimmed[0] == 'b') && trimmed[1] == ' ')
  {
    const int val = trimmed.substring(2).toInt();
    if (val >= 0 && val <= 255)
    {
      FastLED.setBrightness(static_cast<uint8_t>(val));
      FastLED.show();
      Serial.print("Brightness: ");
      Serial.println(val);
    }
    else
    {
      Serial.println("Error: brightness out of range (0-255)");
    }
    return;
  }

  // --- LINES ---
  if (trimmed.equalsIgnoreCase("LINES"))
  {
    Serial.print("Configured lines (");
    Serial.print(static_cast<uint32_t>(LINE_COUNT));
    Serial.println(")");
    for (size_t i = 0; i < LINE_COUNT; i++)
    {
      Serial.print("  #");
      if (i < 10) Serial.print(' ');
      Serial.print(static_cast<uint32_t>(i));
      Serial.print("  ");
      Serial.print(kLines[i].name);
      Serial.print("  (");
      Serial.print(static_cast<uint32_t>(kLines[i].stationCount));
      Serial.println(" stations)");
    }
    return;
  }

  // --- TRACKINFO <NAME> ---
  if (trimmed.length() > 10 && trimmed.substring(0, 10).equalsIgnoreCase("TRACKINFO "))
  {
    const String name = trimmed.substring(10);
    const int idx = findLineByName(name);
    if (idx < 0)
    {
      Serial.print("Error: unknown line '");
      Serial.print(name);
      Serial.println("'. Use LINES to list all.");
      return;
    }
    const LineDef &line = kLines[idx];
    Serial.print("Track: ");
    Serial.print(line.name);
    Serial.print(" (#");
    Serial.print(idx);
    Serial.print(") — ");
    Serial.print(static_cast<uint32_t>(line.stationCount));
    Serial.println(" stations");
    for (uint16_t i = 0; i < line.stationCount; i++)
    {
      const uint16_t ledIndex = line.stations[i];
      Serial.print("  [");
      if (i < 10)  Serial.print(' ');
      if (i < 100) Serial.print(' ');
      Serial.print(static_cast<uint32_t>(i));
      Serial.print("]  LED:");
      if (ledIndex == UNMAPPED_LED)
      {
        Serial.print("UNMAPPED");
      }
      else
      {
        if (ledIndex < 100) Serial.print(' ');
        if (ledIndex < 10)  Serial.print(' ');
        Serial.print(static_cast<uint32_t>(ledIndex));
        const StationNameEntry *e = findStationEntry(ledIndex);
        if (e)
        {
          Serial.print("  ");
          Serial.print(e->constName);
          Serial.print("  \"");
          Serial.print(e->humanName);
          Serial.print('"');
        }
      }
      Serial.println();
    }
    return;
  }

  // --- WHICH <N> ---
  if (trimmed.length() >= 7 && trimmed.substring(0, 6).equalsIgnoreCase("WHICH "))
  {
    const int targetLed = trimmed.substring(6).toInt();
    if (targetLed < 0 || targetLed >= static_cast<int>(NUM_LEDS))
    {
      Serial.print("Error: LED index out of range (0-");
      Serial.print(NUM_LEDS - 1);
      Serial.println(")");
      return;
    }
    const uint16_t led = static_cast<uint16_t>(targetLed);
    Serial.print("LED ");
    Serial.print(static_cast<uint32_t>(led));
    const StationNameEntry *e = findStationEntry(led);
    if (e)
    {
      Serial.print("  ");
      Serial.print(e->constName);
      Serial.print("  \"");
      Serial.print(e->humanName);
      Serial.print('"');
    }
    Serial.println("  found in:");
    bool found = false;
    for (size_t i = 0; i < LINE_COUNT; i++)
    {
      const LineDef &line = kLines[i];
      for (uint16_t j = 0; j < line.stationCount; j++)
      {
        if (line.stations[j] == led)
        {
          Serial.print("    ");
          Serial.print(line.name);
          Serial.print(" (#");
          Serial.print(static_cast<uint32_t>(i));
          Serial.print(") offset ");
          Serial.println(static_cast<uint32_t>(j));
          found = true;
        }
      }
    }
    if (!found) Serial.println("    (not used in any line)");
    return;
  }

  // --- REPORT <NAME> <OFFSET> <ACTUAL_LED> ---
  // User reports: at <OFFSET> in line <NAME>, LED <ACTUAL_LED> lit instead of expected.
  // Firmware prints a comparison and the suggested lines.h fix.
  if (trimmed.length() > 7 && trimmed.substring(0, 7).equalsIgnoreCase("REPORT "))
  {
    // Parse three tokens after "REPORT "
    const String rest = trimmed.substring(7);
    const int sp1 = rest.indexOf(' ');
    const int sp2 = (sp1 >= 0) ? rest.indexOf(' ', sp1 + 1) : -1;
    if (sp1 < 0 || sp2 < 0)
    {
      Serial.println("Usage: REPORT <track_name> <offset> <actual_led_index>");
      Serial.println("  Example: REPORT Central-A 5 72");
      return;
    }
    const String trackName  = rest.substring(0, sp1);
    const int    offset     = rest.substring(sp1 + 1, sp2).toInt();
    const int    actualLed  = rest.substring(sp2 + 1).toInt();

    const int lineIdx = findLineByName(trackName);
    if (lineIdx < 0)
    {
      Serial.print("Error: unknown line '");
      Serial.print(trackName);
      Serial.println("'");
      return;
    }
    const LineDef &line = kLines[lineIdx];
    if (offset < 0 || offset >= static_cast<int>(line.stationCount))
    {
      Serial.print("Error: offset out of range (0-");
      Serial.print(line.stationCount - 1);
      Serial.println(")");
      return;
    }
    if (actualLed < 0 || actualLed >= static_cast<int>(NUM_LEDS))
    {
      Serial.print("Error: actual LED index out of range (0-");
      Serial.print(NUM_LEDS - 1);
      Serial.println(")");
      return;
    }

    const uint16_t expectedLed = line.stations[offset];
    const uint16_t reportedLed = static_cast<uint16_t>(actualLed);

    Serial.println("----- REPORT -----");
    Serial.print("Track   : ");
    Serial.print(line.name);
    Serial.print(" (#");
    Serial.print(lineIdx);
    Serial.print(")  offset ");
    Serial.print(static_cast<uint32_t>(offset));
    Serial.print('/');
    Serial.println(line.stationCount - 1);

    // Expected (configured)
    Serial.print("Expected: LED ");
    if (expectedLed == UNMAPPED_LED)
    {
      Serial.print("UNMAPPED");
    }
    else
    {
      Serial.print(static_cast<uint32_t>(expectedLed));
      const StationNameEntry *e = findStationEntry(expectedLed);
      if (e)
      {
        Serial.print("  ");
        Serial.print(e->constName);
        Serial.print("  \"");
        Serial.print(e->humanName);
        Serial.print('"');
      }
    }
    Serial.println();

    // Reported (actual)
    Serial.print("Reported: LED ");
    Serial.print(static_cast<uint32_t>(reportedLed));
    {
      const StationNameEntry *e = findStationEntry(reportedLed);
      if (e)
      {
        Serial.print("  ");
        Serial.print(e->constName);
        Serial.print("  \"");
        Serial.print(e->humanName);
        Serial.print('"');
      }
    }
    Serial.println();

    // Suggest fix if different
    if (reportedLed != expectedLed)
    {
      Serial.println("Suggested fix in lines.h:");
      Serial.print("  Change LINE_");
      // Print track name as uppercase array name approximation
      String arrName = String(line.name);
      arrName.replace('-', '_');
      arrName.toUpperCase();
      Serial.print(arrName);
      Serial.print('[');
      Serial.print(static_cast<uint32_t>(offset));
      Serial.print("] from ");
      if (expectedLed == UNMAPPED_LED)
      {
        Serial.print("UNMAPPED_LED");
      }
      else
      {
        const StationNameEntry *exp = findStationEntry(expectedLed);
        Serial.print(exp ? exp->constName : String(expectedLed).c_str());
      }
      Serial.print(" to ");
      {
        const StationNameEntry *rep = findStationEntry(reportedLed);
        Serial.println(rep ? rep->constName : String(reportedLed).c_str());
      }
    }
    else
    {
      Serial.println("(reported LED matches configured LED — no change needed)");
    }
    Serial.println("------------------");
    return;
  }

  Serial.print("Unknown command: ");
  Serial.println(trimmed);
  printHelp();
}

static void checkSerial()
{
  while (Serial.available())
  {
    const char c = static_cast<char>(Serial.read());

    // Space: toggle TEST-mode pause (processed immediately, not buffered)
    if (c == ' ' && g_mode == TEST)
    {
      g_testPaused = !g_testPaused;
      if (g_testPaused)
      {
        Serial.println("[PAUSED]  Press SPACE to resume, Q to quit test mode.");
      }
      else
      {
        Serial.println("[RESUMED]");
        g_lastTestMoveMs = millis(); // reset timer so next step starts fresh
      }
      continue;
    }

    // Q / q: quit TEST mode and return to RUN (processed immediately)
    if ((c == 'q' || c == 'Q') && g_mode == TEST)
    {
      g_mode       = RUN;
      g_testPaused = false;
      Serial.println("[TEST mode ended] Returning to RUN mode.");
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      FastLED.show();
      continue;
    }

    if (c == '\n' || c == '\r')
    {
      if (g_serialBuf.length() > 0)
      {
        handleSerialCommand(g_serialBuf);
        g_serialBuf = "";
      }
    }
    else
    {
      g_serialBuf += c;
    }
  }
}

// ---------------------------------------------------------------------------
// Arduino entry points
// ---------------------------------------------------------------------------

void setup()
{
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  Serial.begin(SERIAL_BAUD);
  Serial.setTxTimeoutMs(0); // non-blocking TX: prevents freeze when USB power-only (no host connected)
  Serial.println("London Underground PCB");
  Serial.print("Firmware: ");
  Serial.println(VERSION_STRING);
  Serial.println("https://github.com/funvill/London-Underground-PCB");
  Serial.print("Lines configured: ");
  Serial.println(static_cast<uint32_t>(LINE_COUNT));
  printHelp();

  SetTrainStartingPositions();
}

void loop()
{
  const uint32_t now = millis();

  checkSerial();

  if (g_mode == RUN)
  {
    // Move trains (each line advances independently)
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
  else if (g_mode == TEST)
  {
    const uint16_t TIME_LED_MOVE          = 500;  // 500 ms per station — readable on console
    const uint16_t LOOPS_PER_TRAIN_LINE   = 2;    // 2 passes per line then advance
    TestEachLine(now, TIME_LED_MOVE, LOOPS_PER_TRAIN_LINE);
  }
  // SERIAL_CONTROL: LEDs are set directly by commands; nothing to do in loop.
}