#include "Consts.h"
#include <WiFi.h>
#include <esp_now.h>

//Slave Mac Address
uint8_t SlaveMacAddress[][6] = { {0xF0, 0x08, 0xD1, 0x6B, 0xF0, 0x40} };

//Defining the struct
struct Package{
  String info;
};


//Creating the struct that will be sent through the esp-now
Package sentPackage = {
  .info = "Stopped"
};


//Function that declare the pins
void SetupPins(){
  pinMode(BUTTON_FORWARD, INPUT_PULLUP);
  pinMode(BUTTON_BACKWARD, INPUT_PULLUP);
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(BUTTON_BREAK, INPUT_PULLUP);
  pinMode(BUTTON_STOP, INPUT_PULLUP);
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


//Function that recieve the signal and send it through the esp-now
void sendPackage(){
  //Verify if any button is pressed, and if not, send to the hover to stop
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
  if(result == ESP_OK){                               //Verify if the data was sent
    Serial.print("Sucesso, sinal enviado: ");
    Serial.println(sentPackage.info);
  }else{
    Serial.println("Erro");
  }
}


//Callback function when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status){
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
    mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);      //Prints the Mac Address that was sent
  Serial.print("Pacote enviado para: ");
  Serial.println(macStr);

  Serial.print("Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Sucesso" : "Falhou");                //Verify if the data was sent, if yes print "Sucesso", else print 
 
  sendPackage();                                                                      //Send a new data
}

esp_now_peer_info_t ESPHover;

//Function that setup the esp-now
void SetupEspNow(){
  WiFi.disconnect();                                                                   //Disconnect the ESP from any device that it was connected
  WiFi.mode(WIFI_STA);                                                                 //Define the WiFi mode as Station

  InitESPNow();

  //Struct that storage infos about a peer that will be added in the esp-now network

  ESPHover.channel = CHANNEL;                                                           //Define the comunication chanel as 0 in the ESPHover struct
  ESPHover.encrypt = 0;                                                                 //Define the encryption as false in the ESPHover struct
  
  memcpy(ESPHover.peer_addr, SlaveMacAddress[1], sizeof(SlaveMacAddress[1]));           //Copy the Mac Address from the slave to the ESPHover struct
  esp_now_add_peer(&ESPHover);

  esp_now_register_send_cb(OnDataSent);                                                 //Registrate the callback function when any data is sent

  sendPackage();                                                                      //Send a new data
}