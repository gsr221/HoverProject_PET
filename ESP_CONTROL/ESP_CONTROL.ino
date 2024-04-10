#include "Consts.h"
#include "Functions.h"
#include "PinsSetup.h"

int BOUND_RATE = 115200;

void setup() {
  Serial.begin(BOUND_RATE);
  SetupPins();
  SetupEspNow();

  Serial.println("Setup finalizado");
  Serial.println("---------------------");
}

void loop() {
  //Verify if any button is pressed, and if not, send to the hover to stop
  if(digitalRead(BUTTON_FORWARD) == HIGH)
    sendForward();
  else if(digitalRead(BUTTON_BACKWARD) == HIGH)
    sendBackward();
  else if(digitalRead(BUTTON_LEFT) == HIGH)
    sendLeft();
  else if(digitalRead(BUTTON_RIGHT) == HIGH)
    sendRight();
  else if(digitalRead(BUTTON_STOP) == HIGH)
    sendStop();
  else if(digitalRead(BUTTON_BREAK) == HIGH)
    sendBreak();
  else
    sendStopped();
}