#include "Consts.h"
#include <WiFi.h>
#include <esp_now.h>

//Endereço MAC do Slave
uint8_t SlaveMacAddress[][6] = { {0xF0, 0x08, 0xD1, 0x6B, 0xF0, 0x40} };

//Definindo a struct
typedef struct package{
  String info;
}package;


//Criando a Struct que será enviada via esp-now
package sentPackage = {
  .info = "Stopped"
};


//Função para declarar os pinos
void SetupPins(){
  pinMode(BUTTON_FORWARD, INPUT_PULLUP);
  pinMode(BUTTON_BACKWARD, INPUT_PULLUP);
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(BUTTON_BREAK, INPUT_PULLUP);
  pinMode(BUTTON_STOP, INPUT_PULLUP);
}


//Função que inicia o esp-now
void InitESPNow(){
  if(esp_now_init() == ESP_OK)
    Serial.println("ESPNow inicizalido :)");
  else{
    Serial.println("Inicialização do ESPNow falhou");
    ESP.restart();
  }
}


//Função que recebe o sinal do botão pressionado e envia via esp now
void sendPackage(){
  //Verifica se algum botão foi pressionado
  if(digitalRead(BUTTON_FORWARD) == 0)
    sentPackage.info = "Forward";
  else if(digitalRead(BUTTON_BACKWARD) == 0)
    sentPackage.info = "Backward";
  else if(digitalRead(BUTTON_LEFT) == 0)
    sentPackage.info = "Left";
  else if(digitalRead(BUTTON_RIGHT) == 0)
    sentPackage.info = "Right";
  else if(digitalRead(BUTTON_STOP) == 0)
    sentPackage.info = "Stop";
  else if(digitalRead(BUTTON_BREAK) == 0)
    sentPackage.info = "BRAKE";
  else
    sentPackage.info = "Stopped";

  esp_err_t result = esp_now_send(SlaveMacAddress[1], (uint8_t *)&sentPackage, sizeof(sentPackage));
  Serial.println("Status de envio: ");
  if(result == ESP_OK){                               //Verifica se o dado foi enviado
    Serial.print("Sucesso, sinal enviado: ");
    Serial.println(sentPackage.info);
  }else{
    Serial.println("Erro");
  }
}


//Função de callback, chamada ao enviar um dado
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status){
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
    mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);      //Printa o endereço MAC do dispositivo que foi enviado o dado
  Serial.print("Pacote enviado para: ");
  Serial.println(macStr);

  Serial.print("Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Sucesso" : "Falhou");                //Verifica se o dado foi enviado corretamente
 
  sendPackage();                                                                      //Envia um novo sinal
}

//Strutura que armazena as informações do par que sera conectado no esp now
esp_now_peer_info_t ESPHover;

//Função que configura o esp now
void SetupEspNow(){
  WiFi.disconnect();                                                                   //Desconecta o esp de qualquer dispositivo que ele esteja previamente conectado
  WiFi.mode(WIFI_STA);                                                                 //Define o WIFI do esp como Station

  InitESPNow();

  ESPHover.channel = CHANNEL;                                                           //Define o canal de comunicação como 0
  ESPHover.encrypt = 0;                                                                 //Define a encripitação como falsa
  
  memcpy(ESPHover.peer_addr, SlaveMacAddress[1], sizeof(SlaveMacAddress[1]));           //Copia o endereço MAC do par na struct ESPHover
  esp_now_add_peer(&ESPHover);

  esp_now_register_send_cb(OnDataSent);                                                 //Regisra a função de callback

  sendPackage();                                                                      //Envia um dado
}
