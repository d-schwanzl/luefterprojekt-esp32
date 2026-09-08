#ifndef GATEWAY_SERIAL_H
#define GATEWAY_SERIAL_H

#include <Arduino.h>
#include <ArduinoJson.h>

// Initialisiert die serielle Schnittstelle
void initGatewaySerial(unsigned long baudrate);

// Verarbeitet eingehende Befehle vom Webinterface (Web -> Hardware)
void processIncomingSerial();

// Sendet den aktuellen Zustand der Hardware an das Webinterface (Hardware -> Web)
void sendSystemState();

#endif