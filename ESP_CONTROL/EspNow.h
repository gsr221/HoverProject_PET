#include "Consts.h"
#include <WiFi.h>
#include <esp_now.h>

//Defining the struct skeleton
typedef struct package{
  String info;
}package;

//Creating the struct that will be sent through the esp-now
package sentPackage;

//Struct that storage infos about a peer that will be added in the esp-now network
esp_now_peer_info_t peerInfo;

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

  memcpy(peerInfo.peer_addr, slaveMacAddress, 6);       //Copy the Mac Address from the slave to the peerInfo struct
  peerInfo.channel = 0;                                 //Define the comunication chanel as 0 in the peerInfo struct
  peerInfo.encrypt = false;                             //Define the encryption as false in the peerInfo struct

  //Try to add the slave to the peer comunication list of the esp-now and check it
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Não foi possível adicionar o esp na lista de pares");
    delay(1000);
    ESP.restart();
  }
}


//Function that recieve the signal and send it through the esp-now
void sendPackage(String signal){
  sentPackage.info = signal;
  esp_now_send(slaveMacAddress, (uint8_t *)&sentPackage, sizeof(DataStruct));
  delay(10);
}