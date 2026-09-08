#include "GatewaySerial.h"

// Zugriff auf deine bestehenden globalen Variablen aus dem Hauptcode (.ino / PinMap)
extern bool ledStatus;       // Zustand deiner Bedienteil-LED
extern int luefterSpeed;     // Zustand deiner Lüfterdrehzahl (PWM)

// 1. Serielle Schnittstelle starten
void initGatewaySerial(unsigned long baudrate) {
    Serial.begin(baudrate);
}

// 2. Empfangen (Web -> Hardware): Schaltet Ausgänge & aktualisiert Anzeige
void processIncomingSerial() {
    if (Serial.available() > 0) {
        String input = Serial.readStringUntil('\n');
        
        StaticJsonDocument<200> doc;
        DeserializationError error = deserializeJson(doc, input);

        if (!error) {
            // Befehl vom Webinterface für die LED
            if (doc.containsKey("led")) {
                ledStatus = doc["led"];
                digitalWrite(2, ledStatus ? HIGH : LOW); // Pin anpassen
            }

            // Befehl vom Webinterface für die Lüfterdrehzahl
            if (doc.containsKey("speed")) {
                luefterSpeed = doc["speed"];
                analogWrite(9, luefterSpeed);            // Pin anpassen
            }

            // Sofortige Rückmeldung an das Webinterface zur Bestätigung
            sendSystemState();
        }
    }
}

// 3. Senden (Hardware -> Web): Sendet den aktuellen Zustand an das Webinterface
void sendSystemState() {
    StaticJsonDocument<200> doc;
    doc["led"] = ledStatus;
    doc["speed"] = luefterSpeed;

    serializeJson(doc, Serial);
    Serial.println(); // Ende-Signal (\n) für Node.js
}




