
#ifndef DISPLAY_CONTROLLER_H
#define DISPLAY_CONTROLLER_H
#include <Arduino.h>
#include "PinMap.h"

// Initialisiert alle Pins für Schieberegister und Status-LEDs
void displayBegin();

// Sendet Segment- und Balkenmuster gemeinsam an die Schieberegister-Kaskade
void sendeDaten(uint8_t segmentIndex, uint8_t ledIndex);

// Ändert nur das 7-Segment-Muster; die Balkenanzeige bleibt unverändert
void sendeSegment(uint8_t segmentIndex);

// Ändert nur die Balkenanzeige; das 7-Segment-Muster bleibt unverändert
void sendeBalkenAnzeige(uint8_t ledIndex);

// Steuert das 1-Hz-Blinken der Störungs-LED
// Muss regelmäßig in loop() aufgerufen werden
void blinken(int8_t zustand);

// Setzt den Blinkzustand von main.cpp aus zurück bzw. neu
void setzeSignalZustand(bool zustand);

// Debug: Test der Anzeige über die serielle Eingabe
void testSerialEingabe();

#endif
