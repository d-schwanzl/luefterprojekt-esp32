#include "GatewaySerial.h"

static int letzterBalkenWert = -1;
static int letzterSegmentWert = -1;
static int letzteCurrentRpm = -1;

static bool letzterZustandLedLuefterAus = 1;
static bool letzterZustandLedLuefterAn = 1;
static bool letzterZustandLedLuefterStoerung = 1;

void initGatewaySerial(unsigned long baudrate)
{
  Serial.begin(baudrate);
}

void sendeBalkenWertBeiAenderung(int balkenWert)
{

  // Serial.print("C++ HÖRT IN DER FUNKTION: ");
  // Serial.println(balkenWert);
  // -------------------------------

  if (balkenWert != letzterBalkenWert)
  {
    if (Serial)
    {
      Serial.println(balkenWert);

      Serial.print("{\"type\":\"balkenWert\",\"value\":");
      Serial.print(balkenWert);
      Serial.println("}");
      letzterBalkenWert = balkenWert;
    }
  }
}

void sendeSegmentWertbeiAenderung(int SegmentWert)
{
  if (SegmentWert != letzterSegmentWert)
  {

    if (Serial)
    {
      Serial.println(SegmentWert);
      Serial.print("{\"type\":\"SegmentWert\",\"value\":");
      Serial.print(SegmentWert);
      Serial.println("}");
      letzterSegmentWert = SegmentWert; // ← das fehlte
    }
  }
}

void sendeAktuelleRPM(int currentRpm)
{

  if (letzteCurrentRpm != currentRpm)
  {
    if (Serial)
    {

      Serial.println(currentRpm);
      Serial.print("{\"type\":\"currentRpm\",\"value\":");
      Serial.print(currentRpm);
      Serial.println("}");
      letzteCurrentRpm = currentRpm; // ← das fehlte
    }
  }
}

void sendeWertLedLuefterAus(bool LedLuefterAus)
{

  if (letzterZustandLedLuefterAus != LedLuefterAus)
  {
    if (Serial)
    {

      Serial.println(LedLuefterAus);
      Serial.print("{\"type\":\"LedLuefterAus\",\"value\":");
      Serial.print(LedLuefterAus);
      Serial.println("}");
      letzterZustandLedLuefterAus = LedLuefterAus; // ← das fehlte
    }
  }
}

void sendeWertLedLuefterAn(bool LedLuefterAn)
{

  if (letzterZustandLedLuefterAn != LedLuefterAn)
  {
    if (Serial)
    {

      Serial.println(LedLuefterAn);
      Serial.print("{\"type\":\"LedLuefterAn\",\"value\":");
      Serial.print(LedLuefterAn);
      Serial.println("}");
      letzterZustandLedLuefterAn = LedLuefterAn; // ← das fehlte
    }
  }
}

void sendeWertLedLuefterStoerung(bool LedLuefterStoerung)
{

  if (letzterZustandLedLuefterStoerung != LedLuefterStoerung)
  {
    if (Serial)
    {

      Serial.println(LedLuefterStoerung);
      Serial.print("{\"type\":\"LedLuefterStoerung\",\"value\":");
      Serial.print(LedLuefterStoerung);
      Serial.println("}");
      letzterZustandLedLuefterStoerung = LedLuefterStoerung; // ← das fehlte
    }
  }
}
