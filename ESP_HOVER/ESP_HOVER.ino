//Including the archieves .h
#include "Consts.h"
#include "Functions.h"

const int BAUD_RATE = 115200;

void setup() {
  Serial.begin(BAUD_RATE);
  SetupPWM();
  SetupPins();
  SetupEspNow();

  Serial.println("Setup finalizado");
  Serial.println("---------------------------------");
}

void loop(){}
