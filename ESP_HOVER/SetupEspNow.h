#include "Consts.h"
#include <WiFi.h>
#include <esp_now.h>

//Defining the struct skeleton
typedef struct package{
  String info;
}package;

//Creating the struct that will be recieved through the esp-now
package recievedPackage;

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&recievedPackage, incomingData, sizeof(recievedPackage));
}


void SetupEspNow(){
  WiFi.disconnect();                                    //Disconnect the ESP from any device that it was connected
  WiFi.mode(WIFI_STA);                                  //Define the WiFi mode as Station

  //Verify if there is any trouble with the esp-now
  if (esp_now_init() != ESP_OK){
    Serial.println("Não foi possível inicializar o ESP-NOW");
    delay(1000);
    ESP.restart();
  }
  else
    Serial.println("tudo certo");

  esp_now_register_recv_cb(OnDataRecv);                 //Registrate the callback function when any data is sent
}