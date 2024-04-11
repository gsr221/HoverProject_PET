#include "Consts.h"
#include <WiFi.h>
#include <esp_now.h>

//Parameters for the filter
float a0 = 0.0148;
float a1 = 0.0148;
float b0 = 1;
float b1 = -0.9704;

//Variables for the filter
float y = 0, x = 1, old_y = 0, old_x = 1;
unsigned long dt = 100; //millisecs
unsigned long contador_dt = 0;

//Duty Cycle PWM (0-255)
const int PWM_DC = 50; 

//Defining the struct skeleton
typedef struct package{
  String info;
}package;

//Creating the struct that will be recieved through the esp-now
package recievedPackage{
  .info = "Stopped"
};

//Variable to indicate the bool state
bool state = true;


//FUNCTION TO SETUP PWM IN ESP32
void SetupPWM(){
  ledcSetup(PWM_CHAN, PWM_FREQ, PWM_RES);
  ledcAttachPin(PWM_PIN, PWM_CHAN);
}


//Function that declare the pins
void SetupPins(){
  pinMode(LDIR_PIN, OUTPUT);
  pinMode(LSTOP_PIN, OUTPUT);
  pinMode(LBRAKE_PIN, OUTPUT);
  //pinMode(LSPEED_PIN, INPUT);

  pinMode(RDIR_PIN, OUTPUT);
  pinMode(RSTOP_PIN, OUTPUT);
  pinMode(RBRAKE_PIN, OUTPUT);
  //pinMode(RSPEED_PIN, INPUT);
}


//Function that init the esp-now
void InitESPNow(){
  if(esp_now_init() == ESP_OK)
    Serial.println("ESPNow inicizalido :)");
  else{
    Serial.println("Inicialização do ESPNow falhou");
    ESP.restart();
  }
}


//Callback function when the esp recieve data through esp-now
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int len) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
    mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Dados recebidos de: ");
  Serial.println(macStr);
  memcpy(&recievedPackage, incomingData, sizeof(recievedPackage));
  control();
}


//Setup the esp-now
void SetupEspNow(){
  WiFi.disconnect();                                    //Disconnect the ESP from any device that it was connected
  WiFi.mode(WIFI_STA);                                  //Define the WiFi mode as Station
  InitESPNow()
  esp_now_register_recv_cb(OnDataRecv);                 //Registrate the callback function when any data is sent
}


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


void SetupStopBrake(){
  digitalWrite(LSTOP_PIN, !state);
  digitalWrite(RSTOP_PIN, !state);
  digitalWrite(LBRAKE_PIN, !state);
  digitalWrite(RBRAKE_PIN, !state);
}


//Set y to 0
void SetupSpeed(){
  x=-x;
  Filter();
  ledcWrite(PWM_CHAN, y*PWM_DC); 
  if(y==0)
    x=-x;
}


//Move forward function
void Foward(){
  if(y!=0)
    SetupSpeed();

  SetupStopBrake()

  digitalWrite(LDIR_PIN, state);
  digitalWrite(RDIR_PIN, state);
  
  Filter();
  ledcWrite(PWM_CHAN, y*PWM_DC);
  Serial.println("Indo pra frente");
}


//Move Backward function
void Backward(){
  if(y!=0)
    SetupSpeed();
  
  SetupStopBrake()

  digitalWrite(LDIR_PIN, !state);
  digitalWrite(RDIR_PIN, !state);

  Filter();
  ledcWrite(PWM_CHAN, y*PWM_DC);
  Serial.println("Indo pra tras");  
}


//Move Left function
void Left(){
  if(y!=0)
    SetupSpeed();

  SetupStopBrake()

  digitalWrite(LDIR_PIN, state);
  digitalWrite(RDIR_PIN, !state);
 
  Filter();
  ledcWrite(PWM_CHAN, y*PWM_DC);
  Serial.println("Indo pra esquerda");     
}


//Move Right function
void Right(){
  if(y!=0)
    SetupSpeed();

  SetupStopBrake()

  digitalWrite(LDIR_PIN, !state);
  digitalWrite(RDIR_PIN, state);
  
  Filter();
  ledcWrite(PWM_CHAN, y*PWM_DC);
  Serial.println("Indo pra direita");    
}


//Stop driver function
void Stop(){
  digitalWrite(LSTOP_PIN, state);
  digitalWrite(RSTOP_PIN, state);
  Serial.println("Stop ativado");
}


//Brak driver function
void BRAKE(){
  digitalWrite(LBRAKE_PIN, state);
  digitalWrite(RBRAKE_PIN, state);
  Serial.println("Brake ativado");
}

//Makes the motor stop function
void Stopped(){
  SetupSpeed();
  Serial.println("Parado");
}

//Function that interpretate the signal and do any of the functions previously declared
void control(){
  if(recievedPackage.info == "Forward")
    Foward();
  else if(recievedPackage.info == "Backward")
    Backward();
  else if(recievedPackage.info == "Left")
    Left();
  else if(recievedPackage.info == "Right")
    Right();
  else if(recievedPackage.info == "BRAKE")
    BRAKE();
  else if(recievedPackage.info == "Stop")
    Stop();
  else
    Stopped();
}