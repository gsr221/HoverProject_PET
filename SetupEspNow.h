#include "Consts.h"
#include <WiFi.h>
#include "esp_now.h"

uint8_t MacAdrressEscravo[] = { 0x9C, 0x9C, 0x1F, 0x47, 0x56, 0xF1 };

//Defining the struct skeleton
struct Package{
  String info;
};

//Creating the struct that will be sent through the esp-now
Package sentpackage = {
  .info = "Stopped"
};

//Struct that storage infos about a peer that will be added in the esp-now network
esp_now_peer_info ESPPeerInfo;

//void OnDataSent(){}

//Function that setup the esp-now
void SetupEspNow(){
  WiFi.disconnect();                                    //Disconnect the ESP from any device that it was connected
  WiFi.mode(WIFI_STA);                                  //Define the WiFi mode as Station

  //Verify if there is any trouble with the esp-now
  if (esp_now_init() != ESP_OK){
    Serial.println("Não foi possível inicializar o ESP-NOW");
    delay(1000);
    ESP.restart();
  }

  //esp_now_register_send_cb(OnDataSent);               //Registrate the callback function when any data is sent

  memcpy(ESPPeerInfo.peer_addr, MacAdrressEscravo, 6);       //Copy the Mac Address from the slave to the ESPPeerInfo struct
  ESPPeerInfo.channel = 0;                                 //Define the comunication chanel as 0 in the ESPPeerInfo struct
  ESPPeerInfo.encrypt = false;                             //Define the encryption as false in the ESPPeerInfo struct

  //Try to add the slave to the peer comunication list of the esp-now and check it
  if (esp_now_add_peer(&ESPPeerInfo) != ESP_OK){
    Serial.println("Não foi possível adicionar o esp na lista de pares");
    delay(1000);
    ESP.restart();
  }
}


//Function that recieve the signal and send it through the esp-now
void sendPackage(String signal){
  sentpackage.info = signal;
  esp_now_send(MacAdrressEscravo, (uint8_t *)&sentpackage, sizeof(sentpackage));
  delay(10);
}