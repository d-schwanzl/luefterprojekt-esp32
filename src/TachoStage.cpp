#include "TachoStage.h"

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

// Internal volatile ISR variables
volatile uint32_t lastPulseTime = 0;
volatile uint32_t pulsePeriod = 0;
volatile bool newPulseFlag = false;

// Interrupt Service Routine
void IRAM_ATTR onTachoPulse()
{
  uint32_t now = micros();
  pulsePeriod = now - lastPulseTime;
  lastPulseTime = now;
  newPulseFlag = true;
}

// Initialisiere den Tacho-Pin
void initTacho(uint8_t tachoPin)
{
  pinMode(tachoPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(tachoPin), onTachoPulse, FALLING);
}

// Berechnet die aktuelle RPM inkl. Stall-Detection (Stillstand)
uint16_t processTachoRpm()
{
  static uint16_t currentRpm = 0;

  // Neuer Impuls verarbeiten
  if (newPulseFlag)
  {
    noInterrupts();
    uint32_t period = pulsePeriod;
    newPulseFlag = false;
    interrupts();

    if (period > 0)
    {
      float freqHz = 1000000.0 / period;
      currentRpm = (uint16_t)((freqHz * 60.0) / PULSES_PER_REV);
    }
  }

  // Stillstandserkennung prüfen
  noInterrupts();
  uint32_t timeSinceLast = micros() - lastPulseTime;
  interrupts();

  if (timeSinceLast > (STALL_TIMEOUT_MS * 1000UL))
  {
    currentRpm = 0;
  }

  return currentRpm;
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
