#include "defines.h"
#include "pin_definitions.h"
#include <SPI.h>
#include "temperature.h"
#include "wifi.h"
//#include <Adafruit_ILI9341.h>
//#include <Adafruit_GFX.h>
//#include <TouchScreen.h>

void setup(void)
{
  //open up serial comms
  //be careful with this, as this channel also connects to ESP8266
  Serial.begin(115200);
  #ifdef DEBUG
  delay(1000);
  while(!Serial){
    ; 
  }
  #endif
  
  
  Serial.print("Mr Thermostat code version ");
  Serial.println(CODE_VERSION);
  
  Serial.println("beginning setup function");

  
  pinMode(TEMP_PIN,INPUT);
  digitalWrite(TEMP_PIN,LOW);
  //analogReference(DEFAULT);
  
  setupWifi();
  
  /*while(1){
    resetWifi();
    delay(1000);  
  }*/
}

void loop(void)
{  
  Serial.print("Temperature in voltage: ");
  Serial.println(getTemp(VOLTAGE));
  
  
  //resetWifi();
  
  if(Serial.available() > 0){
    Serial.readBytes(typed_commands,200);
    Serial.print("sent:");
    Serial.println(typed_commands);
    sendCommand(typed_commands); 
  }
  //delay(1000);
  /*Serial1.println("AT");
  delay(100);
  char resp[20] = "";
  int i = 0;
  while((Serial1.available() > 0) && (i < 20)){
    resp[i] = Serial1.read();
    i++;
  }
  Serial.print("received from ESP8266:");
  Serial.println(resp);*/
}



