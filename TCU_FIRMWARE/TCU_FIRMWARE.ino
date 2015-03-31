#include <EEPROM.h> //EEPROM.write(addr,val) EEPROM.read(addr)

#include <TouchScreen.h>
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300); //Replace 300 with actual resistance

#include "defines.h"

#include <ESP8266.h>
#include "wifi.h"

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "draw.h"
#include "sensors.h"
#include "profiles.h"
#include "lcd.h"

#include "temperature.h"

void setup(){
  //open up serial comms
  //be careful with this, as this channel also connects to ESP8266
  Serial.begin(115200);
#if DEBUG == 1
  delay(2000);
  while(!Serial){
    //do nothing 
  }
#endif
  Serial.print("Mr Thermostat code version ");
  Serial.println(CODE_VERSION);
setupLCD();
#if HAS_LCD==1
  //setup LCD hardware
  delay (1000);
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

#if RUN_WIFI
  //setup esp8266 module
  Serial.print("initializing ESP8266...");
  do
  {
    setupWifi();
  }
  while(!wifi.kick());
  Serial.println("done");

  //checking internet connection
  Serial.print("checking network connection...");
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
#endif

  sn = 0;
}

void loop(){ //Main Screen
  TSPoint p;
  int average, temp;
  byte count, i, wait;
  hvac = EEPROM.read(HVAC);
  //for(i = 0; i < MAXSENSORS; i++) {  removeSensor(i);  } //Erase all sensors
  /*
  addSensor("Thermostat", "THERMOSTAT", 90, 128); //Add a sensor
  addSensor("Sensor 1", "1", 100, 128); //Add a sensor
  addSensor("Sensor 2", "22", 40, 0); //Add a sensor
  addSensor("Sensor 3", "333", 76, 128); //Add a sensor
  */
  
  //for(i = 0; i < MAXPROFILES; i++) {  removeProfile(i);  } //Erase all profiles
  
  //addProfile("Long ass name of ass", "1");
  //addProfile("Profile 2", "22");
  //addProfile("Profile 3", "333", 2, 6000, 7000);
  //addProfile("Profile 4", "4444", 3, 7000, 8000);
  //addProfile("Profile 5", "55555", 4, 8000, 9000);
  
  
  //addRule(0, 50, 1, 2560, 3102); //Time
  //addRule(0, 50, 2, 0, 80); //Weather
  //addRule(0, 50, 3, 49320, 51330); // Proximity
  //editRule(0, 0, 100, 1, 2560, 3102);
  temp = -10; //temporary for testing
  tft.fillScreen(BACKGROUND_COLOR);
  tft.setTextColor(FOREGROUND_COLOR);
  tft.setTextSize(2);

  doubleVLine(160, 0, 54, FOREGROUND_COLOR);
  doubleHLine(0, 54, 320, FOREGROUND_COLOR);
  doubleHLine(0, 80, 320, FOREGROUND_COLOR);
  doubleVLine(160, 80, 49, FOREGROUND_COLOR);
  doubleHLine(0, 129, 320, FOREGROUND_COLOR);
  doubleHLine(0, 209, 320, FOREGROUND_COLOR);
  doubleVLine(285, 210, 30, FOREGROUND_COLOR);
  tft.drawTriangle(                 // Left Arrow
  15, 215,         // peak
  5, 225,          // bottom left
  15, 235,         // bottom right
  FOREGROUND_COLOR);
  tft.drawTriangle(                 // Right Arrow
  270, 215,        // peak
  270, 235,        // bottom left
  280, 225,        // bottom right
  FOREGROUND_COLOR);

  printText(40, 4, "High:");
  unitPos(100, 4, temp);
  printText(52, 20, "Low:");
  unitPos(100, 20, temp);
  printText(4, 36, "Outside:");
  unitPos(100, 36, temp);
  printText(165, 4, "1/20/2014");
  printText(165, 20, "12:00 PM");
  i = findActiveProfile();
  if(i != MAXPROFILES) {
    readEEPROMBytes(nam, PROFILES + i * PROFILEBLOCK, 20);
    centerText(160, 60, nam);
  }
  else {  centerText(160, 60, "No Profiles Active");  }
  printText(4, 100, "Current:");
  count = average = i = 0;
  while(i < MAXSENSORS) {
    if(bitRead(EEPROM.read(SENSORS + i * SENSORBLOCK + 26), 7) == 1 && EEPROM.read(SENSORS + i * SENSORBLOCK) != '\0') {
      average += EEPROM.read(SENSORS + i * SENSORBLOCK + 25);
      count++;
    }
    i++;
  }
  if(count) {  average = average / count;  } //If a sensor is active
  else { average = EEPROM.read(SENSORS + 25);  } //If no sensors are active, use THERMOSTATS
  unitPos(100, 100, average);
  printText(164, 100, "Desired:");
  unitPos(260, 100, EEPROM.read(HVAC + 1));
  printText(4, 150, "Fan: On  Off  Auto");
  printText(4, 180, "System: Heat  Cool  Blower");

  //check which settings are active
  if(bitRead(hvac, 0)) {  tft.drawRect(59, 145, 34, 26, FOREGROUND_COLOR);  } //On
  else if(bitRead(hvac, 1)) {  tft.drawRect(107, 145, 46, 26, FOREGROUND_COLOR);  } //Off
  else {  tft.drawRect(167, 145, 58, 26, FOREGROUND_COLOR);  } //Auto
  if(bitRead(hvac, 2)) {  tft.drawRect(95, 175, 58, 26, FOREGROUND_COLOR);  } //Heat
  else if(bitRead(hvac, 3)) {  tft.drawRect(167, 175, 58, 26, FOREGROUND_COLOR);  } //Cool
  else {  tft.drawRect(239, 175, 80, 26, FOREGROUND_COLOR);  } //Blower

  cycleSensorList(sn);

  //Settings
  doubleHLine(290, 215, 25, FOREGROUND_COLOR);
  doubleHLine(290, 225, 25, FOREGROUND_COLOR);
  doubleHLine(290, 234, 25, FOREGROUND_COLOR);
  
  do{
    average = i = count = 0;
    temp = sensorBytes();
    while(i < MAXSENSORS) { //Finds total sensors and current sensor
      if(bitRead(temp, i) != 0) {  count++;  }
      if(i < sn + 1 && bitRead(temp,i)) { average++;  }
      i++;
    }
    
    wait = 0;
    do{
      p = ts.getPoint();
      androidConnect();
    }while(p.z < MINPRESSURE);
    if(p.x > 712) {
      if(p.y < 535) {  updateWeather();  } //Force weather update
      else {  dateSettings();  } //Time/Date Settings
    }
    else if(p.x > 640) {  profileSettings();  } //Profile Settings
    else if (p.x > 497) {
      if(p.y < 535) {  sensorSettings();  } //Sensor Settings
      else {  changeTemp();  } //Change Temperature
    }
    else if(p.x > 265) {  hvacSettingChange(hvac);  }
    else if(p.y < 186) {
      if(sn > 0 && average > 1) {
        sn--;
        while(EEPROM.read(SENSORS + SENSORBLOCK * sn) == 0) {  sn--;  };
        cycleSensorList(sn);
      }
      else {  delay(100);  }
      wait = 1;
    } //Left Arrow for Sensor List
    else if(p.y < 799) {  sensorSettings();  } //Sensor Settings
    else if(p.y < 835) {
      if(sn < MAXSENSORS && average < count) {
        sn++;
        while(EEPROM.read(SENSORS + SENSORBLOCK * sn) == 0) {  sn++;  };
        cycleSensorList(sn);
      }
      else {  delay(100);  }
      wait = 2;
    } //Right Arrow for Sensor List
    else {  settings();  } //Settings
  }while(wait);
}

