//Including the archieves .h
#include "Consts.h"
#include "PWMSetup.h"
#include "Functions.h"
#include "PinsSetup.h"
#include "EspNow.h"

const int BAUD_RATE = 115200;

void setup() {
  Serial.begin(BAUD_RATE);
  SetupPWM();
  SetupPins();
  SetupEspNow();

  Serial.println("SETUP FINISHED");
  Serial.println("---------------------------------");
}

void loop() {
  if(recievedPackage.info == "Forward")
    Foward();
  else if(recievedPackage.info == "Backward")
    Backward();
  else if(recievedPackage.info == "Left")
    Left();
  else if(recievedPackage.info == "Right")
    Right();
  //else if(recievedPackage.info == "Break")
    //Break();
  //else if(recievedPackage.info == "Stop")
    //Stop();
  else
    Stopped();
}
