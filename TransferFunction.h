//Declaring the parameters of the transfer function that comands the motor
float y = 0, x = 1, old_y = 0, old_x = 1;
float a0 = 0.1667, a1 = 0.1667, b0 = 1, b1 = -0.6667;//valores do filtro (Equação de diferenças)
unsigned long dt = 100;
unsigned long contador_dt = 0;

float TransferFunction(){
  if(millis()-contador_dt>dt){
    contador_dt=millis();
    y = a0*x + a1*old_x - b1*old_y;
    old_x = x;
    old_y = y;
  }
  return y;
}