#ifndef GATEWAY_SERIAL_H
#define GATEWAY_SERIAL_H

#include <Arduino.h>

// Für die Gesamtanbindung:
void initGatewaySerial(unsigned long baudrate);


// Sendet den Wert der Balkenazeige bei Änderung
void  sendeBalkenWertBeiAenderung(int balkenWert);

//Sendet den PWM‑Anzeigewert bei Änderung
void sendeSegmentWertbeiAenderung(int aktuelleZahl);

//Sendet die aktuelle Drehzahl bei einer Änderung
void sendeAktuelleRPM(int currentRpm);


#endif