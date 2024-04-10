//CONSTANTES
//PWM CONFIG
#define RPWM_PIN 13
#define LPWM_PIN 15
#define LPWM_CHAN 0
#define RPWM_CHAN 1
#define PWM_FREQ 500
#define PWM_RES 8
  //Os valores de resolução e de frequência acima foram escolhidos para poder
  //facilitar a utilização do código tanto em arduino quanto em ESP32

//LEFT MOTOR BLDC DRIVER PINS
#define LDIR_PIN 4
//#define LSTOP_PIN 5
//#define LBREAK_PIN 18
//#define LSPEED_PIN = 

//RIGHT MOTOR BLDC DRIVER PINS
#define RDIR_PIN 12
//#define RSTOP_PIN 14
//#define RBREAK_PIN 27
//#define RSPEED_PIN = 