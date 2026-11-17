#include "DisplayController.h"
#include "PinMap.h"

// Variablen für das 1-Hz-Blinken (Störung)
unsigned long letzteBlinkZeit = 0;
bool signalZustand = false;

// Globaler Speicher für die aktuellen Indizes beider Register
byte aktuellesSegment = 0; // Index 0..9 für 7-Segment
byte aktuellesLED = 0;     // Index 0..4 für Balkenanzeige

// Baustein 2: Balkenanzeige (2. Register)
const byte barPatterns[10] = {
  0b11111111, // 0 LEDs aktiv        
  0b11111110, // 1 LED  aktiv
  0b11111100, // 2 LEDs aktiv
  0b11111000, // 3 LEDs aktiv   
  0b11110000, // 4 LEDs aktiv  
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111
  };

// Baustein 3: 7-Segment-Anzeige (1. Register)
const byte digitPatterns[10] = {
  0b01000000, // 0: Zahl 0 aktiv
  0b01111001, // 1  Zahl 1 aktiv
  0b00100100, // 2  Zahl 2 aktiv
  0b00110000, // 3  Zahl 3 aktiv
  0b00011001, // 4  Zahl 4 aktiv
  0b00010010, // 5  Zahl 5 aktiv
  0b00000010, // 6  Zahl 6 aktiv
  0b01111000, // 7  Zahl 7 aktiv
  0b00000000, // 8  Zahl 8 aktiv
  0b00010000  // 9  Zahl 9 aktiv
};

void displayBegin()
{
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
void setzeSignalZustand(bool zustand)
{
  signalZustand = zustand;
}

// Hier stehen die Funktionsdefinitionen der Datei display-controller.cpp.
void blinken(int8_t zustand)
{
  if (zustand == 2)
  {
    if (millis() - letzteBlinkZeit >= 500)
    {
      letzteBlinkZeit = millis();
      signalZustand = !signalZustand;
      digitalWrite(LED_STOERUNG, signalZustand);
    }
  }
}

// Hauptfunktion: Sendet beide Bytes synchron an die Kaskade
void sendeDaten(byte segmentIndex, byte ledIndex)
{
  aktuellesSegment = segmentIndex; // Index merken
  aktuellesLED = ledIndex;         // Index merken

  // Serial.print("sendeDaten: Segment=");
  // Serial.print(segmentIndex);
  // Serial.print(" LED=");
  // Serial.println(ledIndex);

  digitalWrite(LATCH_PIN, LOW);

  //1. shiftOut -> Daten werden ins zweite Register geschoben, nach zweitem shiftout.
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, digitPatterns[aktuellesSegment]);

  // 2. shiftOut -> Daten von Segment werden an Segment übergben und die Daten von Balkenanzeige werden an Balkenregister übergeben
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, barPatterns[aktuellesLED]);

  //Aktiviert die Ausgabe für beide Register
  digitalWrite(LATCH_PIN, HIGH);
}

// Hilfsfunktion: Nur 7-Segment ändern
void sendeSegment(byte segmentIndex)
{
  sendeDaten(segmentIndex, aktuellesLED);
}

// Hilfsfunktion: Nur Balkenanzeige ändern
void sendeBalkenAnzeige(byte ledIndex)
{ 
  sendeDaten(aktuellesSegment, ledIndex);
}


// void testSerialEingabe()
// {
//   while (Serial.available() > 0)
//   {
//     char c = Serial.read();
//     if (c >= '0' && c <= '9')
//     {
//       uint8_t stufe = c - '0';
//       sendeDaten(stufe, stufe);
//     }
//   }
// }