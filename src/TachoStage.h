#include <Arduino.h>
#ifndef TACHO_STAGE_H
#define TACHO_STAGE_H

// Configuration Constants
#define NUM_STAGES 10
#define HYSTERESIS 50
const int PULSES_PER_REV = 2;
const unsigned long STALL_TIMEOUT_MS = 1000;

// Function Declarations
void IRAM_ATTR onTachoPulse();
void initTacho(uint8_t tachoPin);
uint16_t processTachoRpm();
uint8_t getStageFromRpm(uint16_t rpm);

#endif // TACHO_STAGE_H
