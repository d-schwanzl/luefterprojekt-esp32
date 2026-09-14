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


//Die drei Funktionen der 3 LEDs:Lüfter Status Anzeige  

void sendeWertLedLuefterAus(bool LedLuefterAus);
void sendeWertLedLuefterAn(bool LedLuefterAn);
void sendeWertLedLuefterStoerung(bool LedLuefterStoerung);



// Verarbeitet eingehende Befehle vom Webinterface (Web -> Hardware)
void processIncomingSerial();

#endif