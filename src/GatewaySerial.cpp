#include "GatewaySerial.h"

static int letzterBalkenWert = -1;    //Nur lokal sichtbar. Wird aber nicht gelöscht wie eine lokale Variable int ... , wenn man aus der Funktion rausspringt. static Variablen bleiben im RAM. Sie behählt cihren Wert und merkt sich diesen für den nächsten Funktionsaufruf
//Varaible wird außerdem gesperrt, kann nicht von uaßen geändert werden, man könnte sie in einer anderen Datei neu definieren

static int letzterSegmentWert = -1;    //Nur lokal sichtbar. Wird aber nicht gelöscht wie eine lokale Variable int ... , wenn man aus der Funktion rausspringt. static Variablen bleiben im RAM. Sie behählt cihren Wert und merkt sich diesen für den nächsten Funktionsaufruf
//Varaible wird außerdem gesperrt, kann nicht von uaßen geändert werden, man könnte sie in einer anderen Datei neu definieren
void initGatewaySerial(unsigned long baudrate) {
  Serial.begin(baudrate);
}

void processIncomingSerial() {
  // Platzhalter
}

void sendeBalkenWertBeiAenderung(int balkenWert) {

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

void sendeSegmentWertbeiAenderung(int SegmentWert) {

    Serial.print("C++ HÖRT IN DER FUNKTION: ");
  Serial.println(SegmentWert);

  if (SegmentWert != letzterSegmentWert ) {
    if (Serial) {
      Serial.print("{\"type\":\"SegmentWert\",\"value\":");
        Serial.print(SegmentWert);
      Serial.println("}");
      letzterSegmentWert = SegmentWert;   // ← das fehlte
    }
  }
}

