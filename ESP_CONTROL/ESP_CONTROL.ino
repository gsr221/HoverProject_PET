#include "Consts.h"
#include "Functions.h"

int BOUND_RATE = 115200;

void setup() {
  Serial.begin(BOUND_RATE);
  SetupPins();
  SetupEspNow();

  Serial.println("Setup finalizado");
  Serial.println("---------------------------------");
}

void loop(){}