#include "Consts.h"

//Variables for the filter
float y = 0, x = 1, old_y = 0, old_x = 1;
unsigned long contador_dt = 0;

//Function of the filter
float Filter(){
  if(millis()-contador_dt>dt){
    contador_dt=millis();
    y = a0*x + a1*old_x - b1*old_y;
    old_x = x;
    old_y = y;
    return y;
  }
}

//Set y to 0
void SetupSpeed(){
  x=-x;
  Filter();
  if(y==0)
    x=-x;
}

//Move forward function
void Foward(){
  if(y!=0)
    SetupSpeed();

  digitalWrite(LDIR_PIN, true);
  digitalWrite(RDIR_PIN, true);
  
  Filter();
  ledcWrite(LPWM_CHAN, y*PWM_DC);
  ledcWrite(RPWM_CHAN, y*PWM_DC);
}


//Move Backward function
void Backward(){
  if(y!=0)
    SetupSpeed();
  
  digitalWrite(LDIR_PIN, false);
  digitalWrite(RDIR_PIN, false);

  Filter();
  ledcWrite(LPWM_CHAN, y*PWM_DC);
  ledcWrite(RPWM_CHAN, y*PWM_DC);  
}


//Move Left function
void Left(){
  if(y!=0)
    SetupSpeed();

  digitalWrite(LDIR_PIN, true);
  digitalWrite(RDIR_PIN, false);
 
  Filter();
  ledcWrite(LPWM_CHAN, y*PWM_DC);
  ledcWrite(RPWM_CHAN, y*PWM_DC);    
}


//Move Right function
void Right(){
  if(y!=0)
    SetupSpeed();

  digitalWrite(LDIR_PIN, false);
  digitalWrite(RDIR_PIN, true);
  
  Filter();
  ledcWrite(LPWM_CHAN, y*PWM_DC);
  ledcWrite(RPWM_CHAN, y*PWM_DC);    
}


//Stop motor function
void Stop(){
  digitalWrite(LSTOP_PIN, true);
  digitalWrite(RSTOP_PIN, true);
}


//Break motor function
void Break(){
  digitalWrite(LBREAK_PIN, true);
  digitalWrite(RBREAK_PIN, true);
}
