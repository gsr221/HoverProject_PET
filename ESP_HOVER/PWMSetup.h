#include "Consts.h"

//FUNCTION TO SETUP PWM IN ESP32
void SetupPWM(){
  ledcSetup(RPWM_CHAN, PWM_FREQ, PWM_RES);
  ledcSetup(LPWM_CHAN, PWM_FREQ, PWM_RES);
  ledcAttachPin(RPWM_PIN, RPWM_CHAN);
  ledcAttachPin(LPWM_PIN, LPWM_CHAN);
}
