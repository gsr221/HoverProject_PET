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

//Move forward function
void Foward(){
  
}

//Move Backward function
void Backward(){
  
}

//Move Left function
void Left(){
  
}

//Move Right function
void Right(){
  
}

//Stop motor function
void Stop(){
  
}

//Break motor function
void Break(){
  
}
