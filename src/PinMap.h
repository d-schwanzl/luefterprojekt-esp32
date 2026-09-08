
#ifndef PIN_MAP_H
#define PIN_MAP_H
#include <Arduino.h>


// --- Schieberegister (74HC595) ---
constexpr uint8_t DATA_PIN  = 0;  // DS
constexpr uint8_t LATCH_PIN = 4;  // ST_CP
constexpr uint8_t CLOCK_PIN = 1;  // SH_CP

// --- Status-LEDs ---
constexpr uint8_t LED_EIN     = 3;
constexpr uint8_t LED_AUS     = 11;
constexpr uint8_t LED_STOERUNG = 2;

// --- Lüfter & Tacho ---
constexpr uint8_t TACHO_PIN     = 5;
constexpr uint8_t OUTPUT_PWM_PIN = 12;

// --- Taster ---
constexpr uint8_t Taster_EIN   = 22;
constexpr uint8_t Taster_AUS   = 23;
constexpr uint8_t Taster_Hochschalten_PWM    = 24;
constexpr uint8_t Taster_Runterschalten_PWM  = 25;

#endif

