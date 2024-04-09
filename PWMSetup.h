#include "Consts.h"

//FUNCTION TO SETUP PWM IN ESP32
void SetupPWM(){
  ledcSetup(PWM_CHAN, PWM_FREQ, PWM_RES);
  ledcAttachPin(PWM_PIN, PWM_CHAN);
}
