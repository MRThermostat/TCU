#include <EEPROM.h> //EEPROM.write(addr,val) EEPROM.read(addr)

#include "defines.h"

#include <TouchScreen.h>
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300); //Replace 300 with actual resistance

#include <ESP8266.h>
#include <MemoryFree.h>
#include "wifi.h"

#include <SPI.h>
#if HAS_LCD==1
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "lcd.h"
#endif

#include "temperature.h"

byte hvac;

//from x = 0, can get 26 characters across

void setup(){
  //open up serial comms
  //be careful with this, as this channel also connects to ESP8266
  Serial.begin(115200);
#if DEBUG==1
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

  //get profiles and data from eeprom
  Serial.print("restoring user data...");  
  while(restoreUserData());
  Serial.println("done");

  //setup esp8266 module
  Serial.print("initializing ESP8266...");
  do
  {
    setupWifi();
  }
  while(!wifi.kick());
  Serial.println("done");

  //checking internet connection
  Serial.print("connecting to network...");
  do
  {
    if(wifi.joinAP(ssid, password))
    {
      
      if(checkNetwork())
      {
        Serial.println("done");
        is_connected = 1;
      }
      else
      {
        Serial.println("entering setup");
        setupNetwork();
      }
      
    }
    else
    {
      Serial.println("failed");
      Serial.println("Entering Setup...");
      setupNetwork();
    }
  }
  while(checkNetwork());
  /*
   //setup nrf24l01+
   Serial.print("initializing sensors comms...");
   do
   {
   setupNRF();
   }while(testNRF());
   Serial.println("done");  
   */
}

void loop(){ //Main Screen
  TSPoint p;
  int temp = -10; //temporary for testing
  #if HAS_LCD==1 
  mainMenu();
  #endif
  int tempdelay = 0;
  do{
    p = ts.getPoint();
    Serial.println("main loop");
    //Check for Android Connection
    /*tempdelay++;
    if(tempdelay == 100){
      tempdelay = 0;
      Serial.print("temperature reading:");
      Serial.println(getTemp(3));
    }
    uint8_t buffer[128] = {0};
    uint32_t len = wifi.recv(buffer, sizeof(buffer), 100);
    if (len > 0) {
      Serial.println("WOOT");
    }*/
  }
  while(p.z < MINPRESSURE || p.z > MAXPRESSURE);
  #if HAS_LCD==1
  if(p.x > 712) {
    if(p.y < 535) {  
      updateWeather();  
    } //Force weather update
    else {  
      dateSettings();  
    } //Time/Date Settings
  }
  else if(p.x > 640) {  
    profileSettings();  
  } //Profile Settings
  else if (p.x > 497) {
    if(p.y < 535) {  
      sensorSettings();  
    } //Sensor Settings
    else {  
      changeTemp();  
    } //Change Temperature
  }
  else if(p.x > 223) {  
    hvacSettingChange();  
  } //HVAC System (needs work)
  else if(p.y < 186) {  
    cycleSensorList(0);  
  } //Left Arrow for Sensor List
  else if(p.y < 799) {  
    sensorSettings();  
  } //Sensor Settings
  else if(p.y < 824) {  
    cycleSensorList(1);  
  } //Right Arrow for Sensor List
  else {  
    settings();  
  } //Settings
  #endif
}

