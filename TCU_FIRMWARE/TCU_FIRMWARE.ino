#include "defines.h"

#include <EEPROM.h> //EEPROM.write(addr,val) EEPROM.read(addr)

#include <TouchScreen.h>
#include "touch.h"

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "lcd.h"

#include "temperature.h"

#include <ESP8266.h>
#include "wifi.h"

byte hvac;

//from x = 0, can get 26 characters across

void setup(){
  //open up serial comms
  //be careful with this, as this channel also connects to ESP8266
  Serial.begin(115200);
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
  
  //get profiles and data from eeprom
  Serial.print("restoring user data...");
  while(restoreUserData());
  /*
  hvac = EEPROM.read(HVAC);

  defaults(0); //run the set defaults
  */
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

void loop(){ //Main Screen
  TSPoint p;
  int temp = -10; //temporary for testing
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  
  tft.drawFastVLine(160, 0, 54, ILI9341_WHITE);
  doubleLine(0, 54, 320, ILI9341_WHITE);
  tft.drawFastHLine(0, 80, 320, ILI9341_WHITE);
  tft.drawFastVLine(160, 80, 49, ILI9341_WHITE);
  doubleLine(0, 129, 320, ILI9341_WHITE);
  doubleLine(0, 209, 320, ILI9341_WHITE);
  tft.drawFastVLine(285, 210, 30, ILI9341_WHITE);
  tft.fillTriangle(                 // Left Arrow
                   15, 215,         // peak
                   5, 225,          // bottom left
                   15, 235,         // bottom right
                   ILI9341_WHITE);
  tft.fillTriangle(                 // Right Arrow
                   270, 215,        // peak
                   270, 235,        // bottom left
                   280, 225,        // bottom right
                   ILI9341_WHITE);

  printText(40, 4, "High:");
  unitPos(100, 4, temp);
  printText(52, 20, "Low:");
  unitPos(100, 20, temp);
  printText(4, 36, "Outside:");
  unitPos(100, 36, temp);
  printText(165, 4, "1/20/2014");
  printText(165, 20, "12:00 PM");
  centerText(160, 60, "Active Profile123456");
  printText(4, 100, "Current:");
  unitPos(100, 100, temp);
  printText(164, 100, "Desired:");
  unitPos(260, 100, temp);
  printText(0, 150, "Fan: On  Off  Auto");
  printText(0, 180, "System: Heat  Cool  Blower");
  
  //check which settings are active
  if(bitRead(hvac,0)) {  tft.drawRect(55, 145, 34, 26, ILI9341_WHITE); } //On
  else if(bitRead(hvac,1)) {  tft.drawRect(103, 145, 46, 26, ILI9341_WHITE);  } //Off
  else {  tft.drawRect(163, 145, 58, 26, ILI9341_WHITE);  } //Auto
  if(bitRead(hvac,2)) {  tft.drawRect(91, 175, 58, 26, ILI9341_WHITE);  } //Heat
  else if(bitRead(hvac,3)) {  tft.drawRect(163, 175, 58, 26, ILI9341_WHITE);  } //Cool
  else {  tft.drawRect(235, 175, 82, 26, ILI9341_WHITE);  } //Blower

  printText(20, 219, "Sensor 1:");
  printText(140, 219, "69F");
  
  //Settings
  doubleLine(290, 215, 25, ILI9341_WHITE);
  doubleLine(290, 225, 25, ILI9341_WHITE);
  doubleLine(290, 234, 25, ILI9341_WHITE);
  
  do{
    p = ts.getPoint();
    //Check for Android Connection
  }while(p.z < MINPRESSURE || p.z > MAXPRESSURE);

  if(p.x > 712) {
    if(p.y < 535) {  updateWeather();  } //Force weather update
    else {  dateSettings();  } //Time/Date Settings
  }
  else if(p.x > 640) {  profileSettings();  } //Profile Settings
  else if (p.x > 497) {
    if(p.y < 535) {  sensorSettings();  } //Sensor Settings
    else {  changeTemp();  } //Change Temperature
  }
  else if(p.x > 223) {  hvacSettingChange();  } //HVAC System (needs work)
  else if(p.y < 186) {  cycleSensorList(0);  } //Left Arrow for Sensor List
  else if(p.y < 799) {  sensorSettings();  } //Sensor Settings
  else if(p.y < 824) {  cycleSensorList(1);  } //Right Arrow for Sensor List
  else {  settings();  } //Settings
}



// Default actions
void defaults(byte action){
  if(action == 0) { //Default
    Serial.print("Restore Defaults\n");
    
  }
  else if(action == 1) { //Write
    Serial.print("Write Defaults\n");
    
  }
  else { //Read
    Serial.print("View Defaults\n");
    
  }
}



/* Possible Future Use
void displayList(int xCenter, byte yCoord, char *array){
  byte tmp = 0;
  while(tmp < 8) {  //while list not empty
    centerText(xCenter, yCoord + tmp * 18, array);
    tmp++;
  }
}*/
