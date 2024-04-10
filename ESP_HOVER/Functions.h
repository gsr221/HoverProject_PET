#include "Consts.h"

float a0 = 0.0148;
float a1 = 0.0148;
float b0 = 1;
float b1 = -0.9704;
unsigned long dt = 100; //millisecs

//Variables for the filter
float y = 0, x = 1, old_y = 0, old_x = 1;
unsigned long contador_dt = 0;
bool dir = true;

const int PWM_DC = 50; 

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
  ledcWrite(LPWM_CHAN, y*PWM_DC);
  ledcWrite(RPWM_CHAN, y*PWM_DC); 
  if(y==0)
    x=-x;
}

//Move forward function
void Foward(){
  if(y!=0)
    SetupSpeed();

  //digitalWrite(LSTOP_PIN, !dir);
  //digitalWrite(RSTOP_PIN, !dir);
  //digitalWrite(LBREAK_PIN, !dir);
  //digitalWrite(RBREAK_PIN, !dir);

  digitalWrite(LDIR_PIN, dir);
  digitalWrite(RDIR_PIN, dir);
  
  Filter();
  ledcWrite(LPWM_CHAN, y*PWM_DC);
  ledcWrite(RPWM_CHAN, y*PWM_DC);
}


//Move Backward function
void Backward(){
  if(y!=0)
    SetupSpeed();
  
  //digitalWrite(LSTOP_PIN, !dir);
  //digitalWrite(RSTOP_PIN, !dir);
  //digitalWrite(LBREAK_PIN, !dir);
  //digitalWrite(RBREAK_PIN, !dir);

  digitalWrite(LDIR_PIN, !dir);
  digitalWrite(RDIR_PIN, !dir);

  Filter();
  ledcWrite(LPWM_CHAN, y*PWM_DC);
  ledcWrite(RPWM_CHAN, y*PWM_DC);  
}


//Move Left function
void Left(){
  if(y!=0)
    SetupSpeed();

  //digitalWrite(LSTOP_PIN, !dir);
  //digitalWrite(RSTOP_PIN, !dir);
  //digitalWrite(LBREAK_PIN, !dir);
  //digitalWrite(RBREAK_PIN, !dir);

  digitalWrite(LDIR_PIN, dir);
  digitalWrite(RDIR_PIN, !dir);
 
  Filter();
  ledcWrite(LPWM_CHAN, y*PWM_DC);
  ledcWrite(RPWM_CHAN, y*PWM_DC);    
}


//Move Right function
void Right(){
  if(y!=0)
    SetupSpeed();

  //digitalWrite(LSTOP_PIN, !dir);
  //digitalWrite(RSTOP_PIN, !dir);
  //digitalWrite(LBREAK_PIN, !dir);
  //digitalWrite(RBREAK_PIN, !dir);

  digitalWrite(LDIR_PIN, !dir);
  digitalWrite(RDIR_PIN, dir);
  
  Filter();
  ledcWrite(LPWM_CHAN, y*PWM_DC);
  ledcWrite(RPWM_CHAN, y*PWM_DC);    
}


//Stop motor function
//void Stop(){
//  digitalWrite(LSTOP_PIN, dir);
//  digitalWrite(RSTOP_PIN, dir);
//}


//Break motor function
//void Break(){
//  digitalWrite(LBREAK_PIN, dir);
//  digitalWrite(RBREAK_PIN, dir);
//}


void Stopped(){
  SetupSpeed();
}