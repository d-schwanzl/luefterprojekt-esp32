
#ifndef DISPLAY_CONTROLLER_H
#define DISPLAY_CONTROLLER_H
#include <Arduino.h>
#include "PinMap.h"



// Initialisiert alle Pins für Schieberegister und Status-LEDs
// (entspricht der tatsächlichen Funktion in DisplayController.cpp)
void displayBegin();

// Schieberegister: Sendet Segment- und Balkenmuster an die Kaskade
void sendeDaten(uint8_t segmentIndex, uint8_t ledIndex);

// Hilfsfunktion: Nur das 7-Segment-Muster ändern (Balken bleibt wie er war)
void sendeSegment(uint8_t segmentIndex);

// Hilfsfunktion: Nur die Balkenanzeige ändern (Segment bleibt wie es war)
void sendeBalkenAnzeige(uint8_t ledIndex);

// Muss regelmäßig im loop() aufgerufen werden für das 1-Hz-Blinken der Störung-LED
void blinken(int8_t zustand);

// Erlaubt es der .ino, den Blink-Zustand von außen zu setzen/zurückzusetzen
void setzeSignalZustand(bool zustand);

#endif

