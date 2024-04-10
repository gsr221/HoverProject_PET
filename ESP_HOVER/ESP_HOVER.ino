//Including the archieves .h
#include "Consts.h"
#include "PWMSetup.h"
#include "Functions.h"
#include "PinsSetup.h"

void setup() {
  Serial.begin(BAUD_RATE);
  SetupPWM();
  SetupPins();

  Serial.println("SETUP FINISHED");
  Serial.println("---------------------------------");
}

void loop() {
  

}
