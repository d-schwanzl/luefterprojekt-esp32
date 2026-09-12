#include "GatewaySerial.h"
#include <ArduinoJson.h>

// Speichert den zuletzt gesendeten Wert
static int letzterPwmStufe = -1;

// 1. Initialisierung der seriellen Schnittstelle
void initGatewaySerial(unsigned long baudrate) {
  Serial.begin(baudrate);
}

// 2. Empfang von Befehlen vom Server (Platzhalter für spätere Logik)
void processIncomingSerial() {
  // Hier wird später der Empfang vom Node.js-Server verarbeitet
}

// 3. Deine Test-Funktion für das Senden
void sendePwmStufeBeiAenderung(int pwmStufe) {
  if (pwmStufe != letzterPwmStufe) {
    StaticJsonDocument<64> doc;
    doc["type"] = "pwmStufe";
    doc["value"] = pwmStufe;

    serializeJson(doc, Serial);
    Serial.println(); // Zeilenumbruch als Trennzeichen

    letzterPwmStufe = pwmStufe; // Zustand merken
  }
}