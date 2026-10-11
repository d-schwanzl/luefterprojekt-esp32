#include "TachoStage.h"

#define MIN_LOW_US 2000 // Low-Phase muss mindestens 2 ms dauern

// Thresholds
static uint16_t stageThresholds[NUM_STAGES] = {
    0,    // Stufe 0: Stand
    1,    // Stufe 1: Drehung ab 1 RPM
    200,  // Stufe 2
    400,  // Stufe 3
    600,  // Stufe 4
    800,  // Stufe 5
    1000, // Stufe 6
    1200, // Stufe 7
    1400, // Stufe 8
    1550  // Stufe 9
};

// ISR-Variablen
volatile uint32_t pulseCount = 0;
volatile uint32_t fallTime = 0;
static uint8_t tachoPinNr = 0;

// Interrupt Service Routine: reagiert auf beide Flanken
void IRAM_ATTR onTachoPulse()
{
  uint32_t now = micros();
  if (digitalRead(tachoPinNr) == LOW)
  {
    fallTime = now; // Low-Phase beginnt
  }
  else if (fallTime != 0)
  {
    if (now - fallTime >= MIN_LOW_US) // Low-Phase lang genug?
      pulseCount++;
    fallTime = 0;
  }
}

// Initialisiere den Tacho-Pin
void initTacho(uint8_t tachoPin)
{
  tachoPinNr = tachoPin;
  pinMode(tachoPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(tachoPin), onTachoPulse, CHANGE);
}

// Berechnet die aktuelle RPM aus der Impulszahl in einem 500-ms-Fenster
uint16_t processTachoRpm()
{
  static uint32_t windowStart = 0;
  static uint16_t rpm = 0;

  uint32_t now = millis();
  uint32_t dt = now - windowStart;
  if (dt >= 500)
  {
    noInterrupts();
    uint32_t c = pulseCount;
    pulseCount = 0;
    interrupts();

    rpm = (uint16_t)((c * 60000UL / dt) / PULSES_PER_REV);
    windowStart = now;
    // Serial.printf("t=%lu dt=%lu impulse=%lu rpm=%u\n", now, dt, c, rpm);
  }
  return rpm;
}

// Interne Hilfsfunktion ohne Hysterese
static uint8_t rawRpmToStage(uint16_t rpm)
{
  for (int8_t i = NUM_STAGES - 1; i >= 0; i--)
  {
    if (rpm >= stageThresholds[i])
    {
      return i;
    }
  }
  return 0;
}

// Stufenberechnung inklusive Hysterese
uint8_t getStageFromRpm(uint16_t rpm)
{
  static uint8_t currentStage = 0;

  if (rpm == 0)
  {
    currentStage = 0;
    return 0;
  }

  uint8_t rawStage = rawRpmToStage(rpm);
  if (rawStage < 1)
    rawStage = 1;

  if (rawStage > currentStage)
  {
    if (rawStage == 1 || rpm >= stageThresholds[rawStage] + HYSTERESIS)
    {
      currentStage = rawStage;
    }
  }
  else if (rawStage < currentStage)
  {
    if (rpm < stageThresholds[currentStage] - HYSTERESIS)
    {
      currentStage = rawStage;
    }
  }

  return currentStage;
}