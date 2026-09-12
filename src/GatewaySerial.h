#ifndef GATEWAY_SERIAL_H
#define GATEWAY_SERIAL_H

#include <Arduino.h>

// Für die Gesamtanbindung:
void initGatewaySerial(unsigned long baudrate);
void processIncomingSerial();

// Für deinen aktuellen Test (Stufen-Variable bei Änderung senden):
void  sendeBalkenWertBeiAenderung(int balkenWert);

#endif