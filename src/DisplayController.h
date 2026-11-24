/**
 * DisplayController.h
 *
 * Ansteuerung der Balkenanzeige und der 7-Segment-Anzeige über kaskadierte
 * Schieberegister sowie des Blinkens der Störungs-LED.
 */

#ifndef DISPLAY_CONTROLLER_H
#define DISPLAY_CONTROLLER_H

#include <Arduino.h>

// Richtet die Pins von Schieberegistern und Status-LEDs ein und setzt die
// Anzeigen auf 0. Aktuell wird die Funktion von main.cpp nicht aufgerufen.
void displayBegin();

// Sendet beide Anzeigen gemeinsam an die Schieberegister-Kaskade.
// segmentIndex: Ziffer 0..9, ledIndex: Anzahl leuchtender Balken 0..4
void sendeDaten(uint8_t segmentIndex, uint8_t ledIndex);

// Ändert nur die 7-Segment-Anzeige (Ziffer 0..9); die Balkenanzeige bleibt
void sendeSegment(uint8_t segmentIndex);

// Ändert nur die Balkenanzeige (0..4 LEDs); die 7-Segment-Anzeige bleibt
void sendeBalkenAnzeige(uint8_t ledIndex);

// Lässt die Störungs-LED mit 1 Hz blinken, solange Zustand == 2 (STÖRUNG).
// Muss regelmäßig in loop() aufgerufen werden.
void blinken(int8_t zustand);

// Setzt den Blinkzustand zurück. main.cpp ruft das bei jedem Zustandswechsel
// auf, damit das Blinken sauber von vorn beginnt.
void setzeSignalZustand(bool zustand);

#endif

