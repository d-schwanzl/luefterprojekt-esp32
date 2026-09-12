#include "GatewaySerial.h"

static int letzterBalkenWert = -1;

void initGatewaySerial(unsigned long baudrate) {
  Serial.begin(baudrate);
}

void processIncomingSerial() {
  // Platzhalter
}

void sendeBalkenWertBeiAenderung(int balkenWert) {
  // --- ABSOLUTER DIREKT-BEWEIS ---
  // Gibt den Wert aus, BEVOR irgendeine Bedingung greift!
  Serial.print("C++ HÖRT IN DER FUNKTION: ");
  Serial.println(balkenWert);
  // -------------------------------

  if (balkenWert != letzterBalkenWert) {
    if (Serial) { 
      Serial.print("{\"type\":\"balkenWert\",\"value\":");
      Serial.print(balkenWert);
      Serial.println("}");
      letzterBalkenWert = balkenWert; 
    }
  }
}