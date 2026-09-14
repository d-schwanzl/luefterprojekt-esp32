#include <Arduino.h>
#include "GatewaySerial.h"
#include "PinMap.h"
#include "DisplayController.h"
#include "TachoStage.h"

//Funktioniert!
//Alles wesentliche für die Kommunikation zwischen Node.js und dem ESP32 über die serielle Schnittstelle (USB) ist in dieser Datei getan

bool ledStatus = false;
int luefterSpeed = 0;

//Wichtig!!!
int8_t aktueller_Zustand_B1 = 1;     // Startzustand 1 (AUS)


uint16_t currentRpm = 0;
unsigned long lastDisplayUpdate = 0;
const unsigned long DISPLAY_INTERVAL = 100; // ms
bool var_falsche_Stoerung = false; //Verhindert, dass die LED‑Störung Wird beim Boost angezeigt, Die Störung kann erst angegangen werden, wenn der Boost‑Zustand nicht mehr besteht

 uint8_t var_RPM    = 0; // Tatsächliche Drehzahl



int8_t letzterZustand   = -1;



// //Modul 1-3
// volatile uint16_t currentRpm = 0;



//Modul 3
uint8_t aktuelleZahl = 0;  //Ausgabe Zahl auf Anzeige
//Modul 3
int stabileZahl = 0;   //Das ist die stabile Zahl die übergeben wird
int lasttimeSegment = 0;
int lastSegment = 0;

//Variablen LEDs der Lüfterstatusanzeige im Webinterface
bool LedLuefterAn = false;
bool LedLuefterAus = true;
bool LedLuefterStoerung = false;






uint8_t stage = 0; //Variable zur Ausgabe der jeweiligen Stufe (nur für Setup-Initialwert).

//Modul 1
//Tasterentprellung 

const uint32_t MIN_HIGH_MS = 100;
const uint32_t DEBOUNCE_MS = 50;

//Variablendeklaration Tachomessung



// Getrennte Entprell-Zeitstempel pro Taster
volatile uint32_t lastLuefterOnTime   = 0;
volatile uint32_t lastLuefterOffTime  = 0;


//ISR und loop-PWM

volatile uint32_t lastTriggerTime = 0;
int button_PWM_stage = -1;

unsigned long actionStartTime2 = 0;
unsigned long actionStartTime3 = 0;

bool boostAktiv = false;
volatile uint8_t currentDutyCycle = 0; // aktueller PWM-Wert zum Vergleichen, 0-255

//Die Softwarezustände
enum luefter_zustand_t {
LUEFTER_AUS          = 0,
LUEFTER_EIN           =1,
LUEFTER_STOERUNG =     3,
Tasternichtgedrueckt = 4

};

enum luefter_pwm_stage {
LUEFTER_STAGE_0        =0,  
LUEFTER_STAGE_UP        =1,
LUEFTER_STAGE_DOWN      =2

};



volatile luefter_zustand_t aktueller_zustandx = LUEFTER_AUS;
volatile luefter_pwm_stage aktueller_zustandy=  LUEFTER_STAGE_0;



//Die Interrupts der ersten Stufe: PWM-Regulierung und Tachoauslesung

void IRAM_ATTR LuefterOffISR() {
  uint32_t now = millis();
    if (now - lastLuefterOffTime < DEBOUNCE_MS) return;
    lastLuefterOffTime = now;
   aktueller_zustandx = LUEFTER_AUS;
}

//Lüfter einschalten
void IRAM_ATTR LuefterOnISR() {
uint32_t now = millis();
    if (now - lastLuefterOnTime < DEBOUNCE_MS) {
        lastLuefterOnTime = now; // Prell-Timer bei jedem Impuls neu starten
        return;
    }
    lastLuefterOnTime = now;
    if(aktueller_zustandx != LUEFTER_STOERUNG){
    aktueller_zustandx = LUEFTER_EIN;
    }
}


//Tasterentprellung
void tasterHochRunterVerarbeiten() {
  static bool upArmed = true, downArmed = true;
  static uint32_t upReleaseTime = 0, downReleaseTime = 0;
  const uint32_t RELEASE_STABLE_MS = 30;

  bool upGedrueckt   = (digitalRead(Taster_Hochschalten_PWM) == LOW);
  bool downGedrueckt = (digitalRead(Taster_Runterschalten_PWM) == LOW);

  if (upGedrueckt) {
    upReleaseTime = 0;
    if (upArmed) {
      aktueller_zustandy = LUEFTER_STAGE_UP;
      upArmed = false;
    }
  } else {
    if (upReleaseTime == 0) upReleaseTime = millis();
    else if (millis() - upReleaseTime >= RELEASE_STABLE_MS) upArmed = true;
  }

  if (downGedrueckt) {
    downReleaseTime = 0;
    if (downArmed) {
      aktueller_zustandy = LUEFTER_STAGE_DOWN;
      downArmed = false;
    }
  } else {
    if (downReleaseTime == 0) downReleaseTime = millis();
    else if (millis() - downReleaseTime >= RELEASE_STABLE_MS) downArmed = true;
  }
}


//Setup-Teil
void setup() {

Serial.begin(115200);

initGatewaySerial(115200);  //Startet die Serielle Kommunikation mit dem Gateway.

pinMode(TACHO_PIN, INPUT_PULLUP);
pinMode(Taster_AUS,INPUT_PULLUP);
pinMode(Taster_EIN,INPUT_PULLUP);
pinMode(Taster_Runterschalten_PWM,INPUT_PULLUP);
pinMode(Taster_Hochschalten_PWM,INPUT_PULLUP);
pinMode(OUTPUT_PWM_PIN, OUTPUT);

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






attachInterrupt(
  digitalPinToInterrupt(Taster_AUS),
  LuefterOffISR,
  FALLING
    );

attachInterrupt(
  digitalPinToInterrupt(Taster_EIN),
  LuefterOnISR,
  FALLING
    );



// Tacho-Interrupt auslagern & starten
initTacho(TACHO_PIN);



//PWM
ledcAttach(OUTPUT_PWM_PIN, 25000, 8);
ledcWrite(OUTPUT_PWM_PIN, 0);






}


void loop() {
 
 
  tasterHochRunterVerarbeiten();
 if(aktueller_zustandx == LUEFTER_EIN) {


//-------------------------------------
/*Erklärung zu Hoch/Runterschalten*/
//-------------------------------------
//LUEFTER_STAGE_0   : Es ist die Bedingung wenn Taster Hoch oder Runter nicht gedrückt wurde
//LUEFTER_STAGE_UP  : Steht für das Hochschalten einer Lüfterstufe 
//LUEFTER_STAGE_DOWN: Steht für das Runtnerschalten einer Lüfterstufe
//aktueller_zustandy: Beschreibt den aktuellen Zustand einer Lüfterstufe: Diese Variable kann drei Werte annehmen. Sie wird in den ISR gesetzt und in der If-Abfrage auf 0 zurückgesetzt
//button_PWM_stage  : Das ist die Laufvariable von 0 bis 4 sie bildet alle Lüfterstufen ab

// Hochschalten (Maximal bis Stufe 4)
if (aktueller_zustandy == LUEFTER_STAGE_UP) {
  if(button_PWM_stage == -1){
    button_PWM_stage = 0;
     boostAktiv = false;

  }
    if (button_PWM_stage < 4) { // Erlaubt das Hochzählen nur bis max. 4
        button_PWM_stage++;
        Serial.print("[TASTER] HOCH -> Neue Soll-Stufe: ");
    }
    aktueller_zustandy = LUEFTER_STAGE_0;
}


// Runterschalten (Minimal bis Stufe 0)
if (aktueller_zustandy == LUEFTER_STAGE_DOWN) {
    if (button_PWM_stage > 0) { // Erlaubt das Runterzählen nur bis min. 0
        button_PWM_stage--;
        Serial.print("[TASTER] RUNTER -> Neue Soll-Stufe: ");
    }
    aktueller_zustandy = LUEFTER_STAGE_0;
}

   

//Abfrage der Button-Stufe 
   switch (button_PWM_stage) {
    case 0:                                                                         //Stufe Null Lüfter AUS
        ledcWrite(OUTPUT_PWM_PIN, 255); // Lüfter aus (invertiert)
        currentDutyCycle = 255;
        boostAktiv = false;             // Scharfschalten für den nächsten Start in Stufe 1
        break;
        var_falsche_Stoerung = false;

    case 1:                                                                         //Erste Langsame Stufe mit 800ms Anlaufboost
        if (!boostAktiv) {
            actionStartTime2 = millis();   // Zeitstempel einmalig sichern
            ledcWrite(OUTPUT_PWM_PIN, 0);  // Vollgas für Anlauf (invers 0 = 100%)
            currentDutyCycle = 0;
            boostAktiv = true;             // Verriegeln, damit der Boost nur einmal läuft
            Serial.println("[PWM] Stufe 1: BOOST gestartet (100%)");
            var_falsche_Stoerung = true;
        } 
        else if (millis() - actionStartTime2 >= 1500) {
            ledcWrite(OUTPUT_PWM_PIN, 235); // Zielwert für Stufe 1
            currentDutyCycle = 235;
            boostAktiv = true;
            var_falsche_Stoerung = false;
        }
        break;

    case 2:                                                                         //Zweite Stufe
        ledcWrite(OUTPUT_PWM_PIN, 220);
        currentDutyCycle = 220;
        boostAktiv = true;              // Verriegelt lassen
        var_falsche_Stoerung = false;
        break;

    case 3:                                                                       //Dritte Stufe
        ledcWrite(OUTPUT_PWM_PIN, 175);
        currentDutyCycle = 175;
        boostAktiv = true;              // Verriegelt lassen
        var_falsche_Stoerung = false;
        break;

    case 4:                                                                         // Vierte Drehstufe (Vollgas)
        ledcWrite(OUTPUT_PWM_PIN, 0);
        currentDutyCycle = 0;
        boostAktiv = true;              // Verriegelt lassen
        var_falsche_Stoerung = false;
        break;
}
     

 }



 if (aktueller_zustandx == LUEFTER_AUS) {

ledcWrite(OUTPUT_PWM_PIN, 255);   //Das ist die nullte Stuffe des Lüfters also Lüfter aus
currentDutyCycle = 255;
button_PWM_stage = -1;

 }

// 1. Tacho-Daten verarbeiten (berechnet RPM & fängt Stillstand ab)
  currentRpm = processTachoRpm();

  // 2. Anzeigelogik & Stufenberechnung
  if (millis() - lastDisplayUpdate >= DISPLAY_INTERVAL) {
    lastDisplayUpdate = millis();

    uint8_t currentStage = getStageFromRpm(currentRpm);

    // Serial.print("RPM: ");
    // Serial.print(currentRpm);
    // Serial.print(" | Stage: ");
    // Serial.println(currentStage);

    aktuelleZahl = currentStage;
  }

var_RPM = aktuelleZahl;


//-----------------------------------------
// Baustein 1: Zustand auswerten : AN/AUS/STÖRUNG-Abfrage
//------------------------------------------
  if (var_RPM == 0 && button_PWM_stage > 0) { 
    //Bei Booststufe
    if(!var_falsche_Stoerung){
     aktueller_zustandx = LUEFTER_STOERUNG;
      aktueller_Zustand_B1 = 2;//STOERUNG  Anschließend die Funktion „Lüfter aus“ aufrufen. Lüfter‑Ein‑Funktion aufrufenButton PWM Stage Größe 1
      
      ledcWrite(OUTPUT_PWM_PIN, 255);
      currentDutyCycle = 255;
      button_PWM_stage = -1;
      
    }   
  }

  else if (currentDutyCycle  < 255 && var_RPM >= 1) {    
    aktueller_Zustand_B1 = 0; // EIN  
  }
  else {      
    if( aktueller_zustandx != LUEFTER_STOERUNG ) {                  
    aktueller_Zustand_B1 = 1; // AUS
  }
  }

  // Zustandswechsel verarbeiten Baustein 1: LED an Ausstörung.
  if (aktueller_Zustand_B1 != letzterZustand) {
    digitalWrite(LED_EIN, LOW);
    digitalWrite(LED_AUS, LOW);
    digitalWrite(LED_STOERUNG, LOW);
    setzeSignalZustand(false);

    Serial.print("[STATUS-WECHSEL]: ZUSTAND ");
    
    switch (aktueller_Zustand_B1) {
      case 0:
        digitalWrite(LED_EIN, HIGH);
        Serial.println("0 -> [BETRIEB / EIN] | LED_EIN aktiv");
        LedLuefterAn = true;
        LedLuefterAus = false;
        LedLuefterStoerung = false;
        break;

      case 1:
        digitalWrite(LED_AUS, HIGH);
        Serial.println("1 -> [AUS] | LED_AUS aktiv");
        LedLuefterAus = true;
        LedLuefterAn = false;
        LedLuefterStoerung = false;

        break;

      case 2:
        Serial.println("2 -> [STÖRUNG] | LED_STÖRUNG blinkt mit 1 Hz");
        LedLuefterStoerung = true;
        LedLuefterAn = false;
        LedLuefterAus = false;
        break;
    }



    letzterZustand = aktueller_Zustand_B1;
  }


/*An Quelldatei des C++‑Display‑Controllers zur Auswertung senden*/
  int balkenWert = (aktueller_zustandx == LUEFTER_AUS || button_PWM_stage < 0) ? 0 : button_PWM_stage;
// Zur Sicherheit auf den Bereich 0..4 begrenzen
if (balkenWert > 4) balkenWert = 4;
if (balkenWert < 0) balkenWert = 0;


  sendeBalkenAnzeige(balkenWert);  //Sende die Pulsweitenmodulationsstufe: 0-4
  sendeSegment(aktuelleZahl);  //Sende den Wert Drehzahlstufe, Geschwindigkeit des Lüfters
  blinken(aktueller_Zustand_B1);   //Sende den Zustand der drei LEDs an die Steuerung
  



// Sende die aktuelle PWM-Stufe an das Gateway, wenn sie sich geändert hat
sendeBalkenWertBeiAenderung(balkenWert);
//Sendet den Wert der 7‑Segmentanzeige




//Sendet die aktuelle Lüfterdrehzahl
sendeAktuelleRPM(currentRpm);




//Keinen falschen Segmentwert-Ausreißer an Web-Interface übergeben. Stufe muss mindestens 100 ms stabil sein.


if (aktuelleZahl != lastSegment) {
    lastSegment = aktuelleZahl;
    lasttimeSegment = millis();
}

if (millis() - lasttimeSegment > 100) {
    stabileZahl = lastSegment;
    sendeSegmentWertbeiAenderung(stabileZahl);  //Bitte sende an die serielle Schnittstelle, wenn der Zustand stabil ist, Den Wert der Segmentanzeige.

}



//Senden der Werte der drei LEDs des Panels Lüfter Statusanzeige
sendeWertLedLuefterAus(LedLuefterAus);

sendeWertLedLuefterAn(LedLuefterAn);

sendeWertLedLuefterStoerung(LedLuefterStoerung);





delay(1);

}




