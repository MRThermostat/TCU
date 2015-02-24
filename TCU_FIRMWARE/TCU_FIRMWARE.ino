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

  Serial.print("FW Version:");
  Serial.println(wifi.getVersion().c_str());

  if (wifi.setOprToStationSoftAP()) {
    Serial.print("to station + softap ok\r\n");
  } 
  else {
    Serial.print("to station + softap err\r\n");
  }

  if (wifi.joinAP("ITEAD", "12345678")) {
    Serial.print("Join AP success\r\n");
    Serial.print("IP: ");
    Serial.println(wifi.getLocalIP().c_str());    
  } 
  else {
    Serial.print("Join AP failure\r\n");
  }

  if (wifi.enableMUX()) {
    Serial.print("multiple ok\r\n");
  } 
  else {
    Serial.print("multiple err\r\n");
  }

  if (wifi.startTCPServer(8090)) {
    Serial.print("start tcp server ok\r\n");
  } 
  else {
    Serial.print("start tcp server err\r\n");
  }

  if (wifi.setTCPServerTimeout(10)) { 
    Serial.print("set tcp server timout 10 seconds\r\n");
  } 
  else {
    Serial.print("set tcp server timout err\r\n");
  }



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
  //Serial.print("Temperature in voltage: ");
  //Serial.println(getTemp(VOLTAGE));


  /*
  digitalWrite(ESP_CH_PD,HIGH);
   //resetWifi();
   char typed_commands[200] = "";
   if(Serial.available() > 0){
   Serial.readBytes(typed_commands,200);
   Serial.print("sent:");
   Serial.println(typed_commands);
   sendCommand(typed_commands); 
   }
   */
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






