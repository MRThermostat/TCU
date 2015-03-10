#include "defines.h"

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "lcd.h"
//#include <TouchScreen.h>

#include "temperature.h"

#include <ESP8266.h>
#include "wifi.h"

void setup(void)
{
  //open up serial comms
  //be careful with this, as this channel also connects to ESP8266
  Serial.begin(115200);
  Serial.setTimeout(100);
  #ifdef DEBUG
    delay(1000);
    while(!Serial){
      //do nothing 
    }
  #endif

  Serial.print("Mr Thermostat code version ");
  Serial.println(CODE_VERSION);

  #if HAS_LCD==1
    //setup LCD hardware
    Serial.print("initializing LCD hardware...");
    do{
      setupLCD();
    }
    while(readDiagnostics());
    Serial.println("done");
  #endif

  //setup temp sensor
  Serial.print("initializing temperature sensor...");
  setupTemp();
  Serial.println("done");

  //setup esp8266 module
  Serial.print("initializing ESP8266...");
  do
  {
    setupWifi();
  }
  while(wifi.kick());
  Serial.println("done");

  /*
  //checking internet connection
   Serial.print("checking network connection...");
   do
   {
   if(wifi.joinAP(ssid, password))
   {
   if(checkNetwork())
   {
   Serial.println("done");
   }
   else
   {
   setupNetwork();
   }
   }
   else
   {
   setupNetwork();
   }
   }while(checkNetwork());
   
   //setup nrf24l01+
   Serial.print("initializing sensors comms...");
   do
   {
   setupNRF();
   }while(testNRF());
   Serial.println("done");  
   */
}

void loop(void)
{  
  
}






