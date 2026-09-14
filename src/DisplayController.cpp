#include "DisplayController.h"
#include "PinMap.h"


// Variablen für das 1-Hz-Blinken (Störung)
unsigned long letzteBlinkZeit = 0;
bool signalZustand = false;

// Globaler Speicher für die aktuellen Indizes beider Register
byte aktuellesSegment = 0; // Index 0..9 für 7-Segment
byte aktuellesLED     = 0; // Index 0..4 für Balkenanzeige



// Baustein 2: Balkenanzeige (2. Register)
const byte barPatterns[10] = {
  0b00000000, // S0: Aus
  0b00000001, // S1: 1 LED
  0b00000011, // S2: 2 LEDs
  0b00000111, // S3: 3 LEDs
  0b00001111, // S4: 4 LEDs
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000 
};

// Baustein 3: 7-Segment-Anzeige (1. Register)
const byte digitPatterns[10] = {
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111  // 9
};


void displayBegin() {
  // Pins 7-Segment / Balken
  pinMode(DATA_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);

  digitalWrite(DATA_PIN, LOW);
  digitalWrite(LATCH_PIN, LOW);
  digitalWrite(CLOCK_PIN, LOW);

  // Initialwert setzen (Display '0', Balken 'S0')
  sendeDaten(0, 0);

  // Pins Baustein 1
  pinMode(LED_EIN, OUTPUT);
  pinMode(LED_AUS, OUTPUT);
  pinMode(LED_STOERUNG, OUTPUT);

  digitalWrite(LED_EIN, LOW);
  digitalWrite(LED_AUS, LOW);
  digitalWrite(LED_STOERUNG, LOW);


}


// Eine kleine Funktion, damit die .ino den Zustand von außen ändern kann:
void setzeSignalZustand(bool zustand) {
  signalZustand = zustand;
}




//Hier stehen die Funktionsdefinitionen der Datei display-controller.cpp.
void blinken(int8_t zustand) {
  if (zustand == 2) {
    if (millis() - letzteBlinkZeit >= 500) {
      letzteBlinkZeit = millis();
      signalZustand = !signalZustand;
      digitalWrite(LED_STOERUNG, signalZustand);
    }
  }
}






// Hauptfunktion: Sendet beide Bytes synchron an die Kaskade
void sendeDaten(byte segmentIndex, byte ledIndex) {
  aktuellesSegment = segmentIndex; // Index merken
  aktuellesLED     = ledIndex;     // Index merken

  digitalWrite(LATCH_PIN, LOW);
  

  // 2. shiftOut -> Bleibt im 1. Register stehen (7-Segment)
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, digitPatterns[aktuellesSegment]);

  // 1. shiftOut -> Wandert durch das 1. IC durch ins 2. Register (Balken)
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, barPatterns[aktuellesLED]);
  

  
  digitalWrite(LATCH_PIN, HIGH);




}

// Hilfsfunktion: Nur 7-Segment ändern
void sendeSegment(byte segmentIndex) {
  sendeDaten(segmentIndex, aktuellesLED);

}

// Hilfsfunktion: Nur Balkenanzeige ändern
void sendeBalkenAnzeige(byte ledIndex) {
  sendeDaten(aktuellesSegment, ledIndex);
}