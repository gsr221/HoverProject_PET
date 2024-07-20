#include "Consts.h"
#include <WiFi.h>
#include <esp_now.h>

//Parametros do filtro
float a0 = 0.0148;
float a1 = 0.0148;
float b0 = 1;
float b1 = -0.9704;

//Variáveis para o filtro
float y = 0, x = 1, old_y = 0, old_x = 1;
unsigned long dt = 100; //millisecs
unsigned long contador_dt = 0;

//Duty Cycle do PWM (0-255)
int PWM_DC = 10; 

bool brakeActivate = false;

//Definição do esqueleto da estrutura
typedef struct package{
  String info;
}package;

//Criando a estrutura q irá receber a informação recebida pelo ESP-NOW
package recievedPackage{
  .info = "Stopped"
};

//Variável que indica o estado boleano
bool state = true;


//------------Função do filtro------------//

//Função
float Filter(){
  if(millis()-contador_dt>dt){
    contador_dt=millis();
    y = a0*x + a1*old_x - b1*old_y;
    old_x = x;
    old_y = y;
    return y;
  }
}


//------------Funções de Setup------------//

//Função para realizar o setup do PWM
void SetupPWM(){
  ledcAttachPin(PWM_PIN, PWM_CHAN);
  ledcSetup(PWM_CHAN, PWM_FREQ, PWM_RES);
}


//Função que declara os pinos
void SetupPins(){
  pinMode(LDIR_PIN, OUTPUT);
  pinMode(LBRAKE_PIN, OUTPUT);

  pinMode(RDIR_PIN, OUTPUT);
  pinMode(RBRAKE_PIN, OUTPUT);

  pinMode(RELAY_PIN, OUTPUT);
}

//Função para inicializar os pinos do Brake em False
void SetupBrake(){
  digitalWrite(LBRAKE_PIN, !state);
  digitalWrite(RBRAKE_PIN, !state);
}


//------------Funções de movimento------------//

//Reseta o valor de Y para 0
void ResetSpeed(){
  x=-x;
  while(y>0){
    Filter();
    ledcWrite(PWM_CHAN, y*PWM_DC);
  }  
  if(y<0)
    y=0; 
}

//Função para mover para frente
void Foward(){
  if (brakeActivate)
    SetupBrake();

  digitalWrite(LDIR_PIN, !state);
  digitalWrite(RDIR_PIN, state);
  
  Filter();
  ledcWrite(PWM_CHAN, y*PWM_DC);
  Serial.println("Indo pra frente"); 
}

//Função para mover para trás
void Backward(){
  if (brakeActivate)
    SetupBrake();

  digitalWrite(LDIR_PIN, state);
  digitalWrite(RDIR_PIN, !state);

  Filter();
  ledcWrite(PWM_CHAN, y*PWM_DC);
  Serial.println("Indo pra tras");  
}

//Função para mover para esquerda
void Left(){
  if (brakeActivate)
      SetupBrake();

  digitalWrite(LDIR_PIN, !state);
  digitalWrite(RDIR_PIN, !state);
 
  Filter();
  ledcWrite(PWM_CHAN, y*PWM_DC);
  Serial.println("Indo pra esquerda");     
}

//Função para mover para direita
void Right(){
  if (brakeActivate)
    SetupBrake();

  digitalWrite(LDIR_PIN, state);
  digitalWrite(RDIR_PIN, state);
  
  Filter();
  ledcWrite(PWM_CHAN, y*PWM_DC);
  Serial.println("Indo pra direita");    
}

//Função para ativar o Brake
void Brake(){
  brakeActivate = true;
  digitalWrite(LBRAKE_PIN, state);
  digitalWrite(RBRAKE_PIN, state);
  Serial.println("Brake ativado");
}

//Função que faz o motor parar
void Stopped(){
  //Reset the value of y and the speed of the hover gradually
  ResetSpeed();
  Serial.println("Parado");
}


//------------Funções para o ESP NOW------------//

//Função para interpretar o sinal recebido e executar alguma das funções declaradas anteriormente
void control(){
  if(recievedPackage.info == "Forward"){
    Foward();
  }
  else if(recievedPackage.info == "Backward"){
    Backward();
  }    
  else if(recievedPackage.info == "Left"){
    Left();
  }    
  else if(recievedPackage.info == "Right"){
    Right();
  }   
  else if(recievedPackage.info == "BRAKE"){
    Brake();
  }    
  else{
    Stopped();
  }    
}

//Função que inicializa o ESP-NOW
void InitESPNow(){
  if(esp_now_init() == ESP_OK)
    Serial.println("ESPNow inicizalido :)");
  else{
    Serial.println("Inicialização do ESPNow falhou");
    ESP.restart();
  }
}

//Função que será executada quando for recebido algum dadopelo ESP-NOW
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int len) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
    mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Dados recebidos de: ");
  Serial.println(macStr);
  memcpy(&recievedPackage, incomingData, sizeof(recievedPackage));
  control();
}

//Função que faz o setup do ESP-NOW
void SetupEspNow(){
  WiFi.disconnect();                                    //Disconecta o ESP de qualquer dispositivo que eles esteja previamente conectado
  WiFi.mode(WIFI_STA);                                  //Define modo do WiFi como State
  InitESPNow();
  esp_now_register_recv_cb(OnDataRecv);                 //Registra a função de callback quando algum dado é recebido
  control();
}

//Função que ativa o relé, ligando os drivers
void TurnDriversOn(){
  digitalWrite(RELAY_PIN, state);
}
