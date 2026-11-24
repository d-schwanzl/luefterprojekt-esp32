/**
 * GatewaySerial.h
 *
 * Serielle Schnittstelle zwischen ESP32 und dem JavaScript-Gateway auf dem PC.
 *
 * Senderichtung (ESP32 -> Web): sende*-Funktionen. Jeder Wert wird nur
 * bei Änderung gesendet, als JSON-Zeile {"type":"<Name>","value":<Zahl>}.
 *
 * Empfangsrichtung (Web -> ESP32): processIncomingSerial().
 */
#ifndef GATEWAY_SERIAL_H
#define GATEWAY_SERIAL_H

#include <Arduino.h>

// Für die Gesamtanbindung:
void initGatewaySerial(unsigned long baudrate);

// Sendet den Wert der Balkenazeige bei Änderung
void sendeBalkenWertBeiAenderung(int balkenWert);

// Sendet den PWM‑Anzeigewert bei Änderung
void sendeSegmentWertbeiAenderung(int aktuelleZahl);

// Sendet die aktuelle Drehzahl bei einer Änderung
void sendeAktuelleRPM(int currentRpm);

// Statusanzeige: Die drei Funktionen der 3 LEDs:Lüfter Status Anzeige
void sendeWertLedLuefterAus(bool LedLuefterAus);
void sendeWertLedLuefterAn(bool LedLuefterAn);
void sendeWertLedLuefterStoerung(bool LedLuefterStoerung);

// Verarbeitet eingehende Befehle vom Webinterface (Web -> Hardware) in main.ccp
void processIncomingSerial();

#endif
