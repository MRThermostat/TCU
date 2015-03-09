#include <EEPROM.h> //EEPROM.write(addr,val) EEPROM.read(addr)
#include <TouchScreen.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>
#include "pin_definitions.h"
#include <SPI.h>

#define TS_MINX 180
#define TS_MINY 150
#define TS_MAXX 870
#define TS_MAXY 920

#define MINPRESSURE 10
#define MAXPRESSURE 1000

#define HVAC 0 //HVAC Settings Address Location

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RESET);

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300); //Replace 300 with actual resistance
byte hvac;

//from x = 0, can get 26 characters across

void setup(){
  //open up serial comms
  //be careful with this, as this channel also connects to ESP8266
  Serial.begin(9600);
  Serial.println("Mr Thermostat");
  
  pinMode(11, OUTPUT);
  analogWrite(11,255);
  
  tft.begin();
  tft.setRotation(3);
  
  hvac = EEPROM.read(HVAC);

  defaults(0); //run the set defaults
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

//Settings Screen
void settings(){
  TSPoint p;
  
  do{  
    makeTitle("Device Settings");    
    tft.drawFastHLine(0, 78, 320, ILI9341_WHITE);
    tft.drawFastHLine(0, 131, 320, ILI9341_WHITE);
    tft.drawFastHLine(0, 184, 320, ILI9341_WHITE);
    
    centerText(160, 43, "Profile Settings");
    centerText(160, 96, "Sensor Settings");
    centerText(160, 149, "WiFi Settings");
    centerText(160, 202, "Defaults");
    
    do{
      do{
        p = ts.getPoint();
        //Check for Android Connection
      }while(p.z < MINPRESSURE || p.z > MAXPRESSURE);
    
      if(p.x > 798) {
        if(p.y < 318) {  return;  } //Back
      }
      else {
        if(p.x > 645) {  profileSettings();  } //Profile Settings
        else if(p.x > 493) {  sensorSettings();  } //Sensor Settings
        else if(p.x > 252) {  wifiSettings();  } //WiFi Settings
        else {  defaults(2);  } //Default Settings
      }
    }while(p.x > 798 && p.y > 318);
  }while(true);
}

//Date/Time Setting Screen
void dateSettings(){
  Serial.print("Date/Time\n");
}

//Profile Settings Screen
void profileSettings(){
  TSPoint p;
  char nam[21] = "Active Profile123456";
  do{
    byte tmp = 0;
    makeTitle("Profile Settings");
    tft.drawFastHLine(0, 65, 320, ILI9341_WHITE);
    tft.drawFastHLine(0, 215, 320, ILI9341_WHITE);
    
    printText(62, 28, "Currently Active");
    centerText(160, 46, nam);
    //Profile List
    while(tmp < 8) {  //while list not empty
      centerText(160, 70 + tmp * 18, nam);
      tmp++;
    }
    do{
      do{
        p = ts.getPoint();
        //Check for Android Connection
      }while(p.z < MINPRESSURE || p.z > MAXPRESSURE);
    
      if(p.x > 798 && p.y < 318) {  return;  } //Back
      else if(p.x > 683 && p.x < 798) {  changeActive();  } //Rules
      //else if(p.x < 252) {  editProfile();  } //Edit Profile
    }while(p.x > 798);
  }while(true);
}

//Sensor Settings Screen
void sensorSettings(){
  TSPoint p;
  char nam[13] = "Sensor Name1";
  do{
    byte tmp = 0;
    makeTitle("Sensor Settings");
    tft.drawFastVLine(160, 25, 215, ILI9341_WHITE);
    
    printText(50, 30, "Active");
    while(tmp < 8) {  //while list not empty
      centerText(80, 70 + tmp * 18, nam);
      tmp++;
    }
    
    tmp = 0;
    printText(198, 30, "Sensors");
    while(tmp < 8) {  //while list not empty
      centerText(240, 70 + tmp * 18, nam);
      tmp++;
    }
    do{
      do{
        p = ts.getPoint();
        //Check for Android Connection
      }while(p.z < MINPRESSURE || p.z > MAXPRESSURE);

      if(p.x > 798 && p.y < 318) {  return;  } //Back
      else if(p.y > 535) {
        tmp = (668 - p.x)/52; //Finds which sensor was pressed
        sensorEdit(tmp);
      }
    }while(p.x > 798 || p.y < 535);
  }while(true);
}

void sensorEdit(byte sensorNumber){
  //Find correct sensor
  TSPoint p;
  char nam[13] = "Sensor Name1";
  int temp = 100;
  makeTitle("Sensor Settings");
  
  printText(4, 30, "Name:");  
  printText(62, 28, nam); //Grab sensor name

  printText(4, 60, "Latest Temperature:");
  unitPos(244 , 60, temp); //Grab latest Temperature

  printText(4, 90, "Active:");
  printText(100, 90, "Yes");
  printText(160, 90, "No");
  /*if(sensorActive) {  tft.drawRect(95, 85, 46, 26, ILI9341_WHITE);  } //Box around Yes
  else {  tft.drawRect(155, 85, 34, 26, ILI9341_WHITE);  } //Box around No*/
  printText(4, 120, "Sensor ID:");
  printText(130, 120, "1234567890"); //Grab sensor ID
  
  printText(4, 150, "Battery Status:");
  printText(184, 150, "Replace"); //Grab sensor battery status
  
  do{
    do{
      p = ts.getPoint();
      //Check for Android Connection
    }while(p.z < MINPRESSURE || p.z > MAXPRESSURE);
    if(p.x > 717) {
      if(p.y < 318) {  return;  } //Back
      else {  /*changeName(0, sensorNumber);  */}
    }
    else{ //go to sensor list and change active value using sensorActive ^ 1
      if(p.x > 545 && p.x < 631) {
        if(p.y > 361 && p.y < 506) {
          tft.drawRect(95, 85, 46, 26, ILI9341_WHITE);
          tft.drawRect(155, 85, 34, 26, ILI9341_BLACK);
        }
        else if(p.y < 593){
          tft.drawRect(95, 85, 46, 26, ILI9341_BLACK);
          tft.drawRect(155, 85, 34, 26, ILI9341_WHITE);
        }
      } 
    }
  }while(true);
}

//Obtain newest Weather info
void updateWeather(){
  //grab latest weather info
  Serial.print("Weather\n");
}

//Change Desired Temperature Screen
void changeTemp(){
  TSPoint p;
  byte temp = 99; // Must remove '= 99' before final product  
  makeTitle("Change Temperature");
  tft.drawFastHLine(0, 210, 320, ILI9341_WHITE);
  tft.fillTriangle(                 // Up Arrow
                   160, 30,         // peak
                   100, 60,         // bottom left
                   220, 60,         // bottom right
                   ILI9341_WHITE);
  tft.fillTriangle(                 // DownArrow
                   160, 200,        // peak
                   100, 170,        // bottom left
                   220, 170,        // bottom right
                   ILI9341_WHITE);

  printText(124, 215, "Accept");
  tft.setTextSize(10);
  //grab current desired temperature temp = ;
  tft.setCursor(105, 80);
  tft.println(temp);

  do{
    do{
      p = ts.getPoint();
      //Check for Android Connection
    }while(p.z < MINPRESSURE || p.z > MAXPRESSURE);
    
    //Cycle temp with arrow presses
    if(p.x > 798 && p.y < 318) {  return;  }
    else if(p.x > 697 && p.x < 798) {  if(temp + 1 < 100) {  temp++;  }  }
    else if(p.x > 266 && p.x < 381) {  if(temp - 1 > 9) {  temp--;  }  }
    else if(p.x < 266) {
      //change desired temp value
      return;
    }
    tft.fillRect(70, 80, 170, 70, ILI9341_BLACK);
    if(temp > 99) {  tft.setCursor(70, 80);  }
    else {  tft.setCursor(105, 80);  }
    tft.println(temp);
  }while(true);
}

//HVAC Settings Screen
void hvacSettingChange(){
  TSPoint p;  
  makeTitle("HVAC Settings");
  tft.drawFastHLine(0, 107, 320, ILI9341_WHITE);
  
  //Fan Area
  printText(146, 35, "Fan");
  printText(61, 70, "On");
  printText(146, 70, "Off");
  printText(216, 70, "Auto");
  if(bitRead(hvac,0)) {  tft.drawRect(56, 65, 34, 26, ILI9341_BLACK);  } //On
  else if(bitRead(hvac,1)) {  tft.drawRect(141, 65, 46, 26, ILI9341_BLACK);  } //Off
  else {  tft.drawRect(211, 65, 58, 26, ILI9341_BLACK);  } //Auto
  
  //System Area
  printText(124, 117, "System");
  printText(51, 152, "Heat");
  printText(136, 152, "Cool");
  printText(204, 152, "Blower");
  if(bitRead(hvac,2)) {  tft.drawRect(46, 147, 58, 26, ILI9341_BLACK);  } //Heat
  else if(bitRead(hvac,3)) {  tft.drawRect(131, 147, 58, 26, ILI9341_BLACK);  } //Cool
  else {  tft.drawRect(199, 147, 82, 26, ILI9341_BLACK);  } //Blower
  
  do{
    do{
      p = ts.getPoint();
      //Check for Android Connection
    }while(p.z < MINPRESSURE || p.z > MAXPRESSURE);
    
    if(p.x > 562) {
      if(p.x > 798 && p.y < 318) {
        //if(hvac != EEPROM.read(HVAC);) {  EEPROM.write(HVAC,hvac);  }
        return;
      } //Back
      else if(p.x < 723) {
        tft.drawRect(56, 65, 34, 26, ILI9341_BLACK);
        tft.drawRect(141, 65, 46, 26, ILI9341_BLACK);
        tft.drawRect(211, 65, 58, 26, ILI9341_BLACK);
        if(p.y < 405) {
          tft.drawRect(56, 65, 34, 26, ILI9341_WHITE);
          bitWrite(hvac, 0, bitRead(hvac, 0) ^ 1);
        }
        else if(p.y < 660) {
          tft.drawRect(141, 65, 46, 26, ILI9341_WHITE);
          bitWrite(hvac, 1, bitRead(hvac, 1) ^ 1);
        }
        else {  tft.drawRect(211, 65, 58, 26, ILI9341_WHITE);  }
      }
    }
    else if(p.x < 487){
      tft.drawRect(46, 147, 58, 26, ILI9341_BLACK);
      tft.drawRect(131, 147, 58, 26, ILI9341_BLACK);
      tft.drawRect(199, 147, 82, 26, ILI9341_BLACK);
      if(p.y < 405) {
        tft.drawRect(46, 147, 58, 26, ILI9341_WHITE);
        bitWrite(hvac, 2, bitRead(hvac, 2) ^ 1);
      }
      else if(p.y < 660) {
        tft.drawRect(131, 147, 58, 26, ILI9341_WHITE);
        bitWrite(hvac, 3, bitRead(hvac, 3) ^ 1);
      }
      else {  tft.drawRect(199, 147, 82, 26, ILI9341_WHITE);  } 
    }
  }while(true);
}

//Rotates Sensor List
void cycleSensorList(boolean directions){
  if(directions) { //Right
    Serial.print("Right\n");
  }
  else { //Left
    Serial.print("Left\n");
  }
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

//Profile Edit Screen
void editProfile(){
  Serial.println("Edit");
}

//Change Active Profile Screen
void changeActive(){
  Serial.println("Active");
}

//WiFi Settings
void wifiSettings(){
  TSPoint p;  
  makeTitle("WiFi Settings");
  tft.drawFastHLine(0, 200, 320, ILI9341_WHITE);

  printText(4, 130, "SSID:");
  printText(76, 130, "SSID1234567890.");
  printText(4, 180, "Connected:");
  printText(136, 180, "Yes");
  printText(52, 215, "Access Point Setup");

    do{
      do{
        p = ts.getPoint();
        //Check for Android Connection
      }while(p.z < MINPRESSURE || p.z > MAXPRESSURE);

      if(p.x > 798 && p.y < 318) {  return;  } //Back
      else if(p.y < 295) {  accessPointSetup();  }
    }while(true);
}

void unitPos(int xCoord, byte yCoord, int temp) {
  tft.setCursor(xCoord, yCoord);
  tft.println(temp); //Replace with actual variable
  if(temp < 100) {
    if(temp > 9) {  tft.setCursor(xCoord + 24, yCoord);  }
    else if(temp < 0) {
      if(temp > -10) {  tft.setCursor(xCoord + 24, yCoord);  }
      else {  tft.setCursor(xCoord + 36, yCoord);  } 
    }
    else {  tft.setCursor(xCoord + 12, yCoord);  }
  }
  else {  tft.setCursor(xCoord + 36, yCoord);  }
  tft.println("F"); //Replace with unit
}

void accessPointSetup() {
  Serial.println("Access Point Setup");  
}

//Character Array Length
byte cal(char *array){
  byte count = 0;
  while(array[count] != '\0') {  count++;  };
  return count;
}

void centerText(int centerPoint, byte yCoord, char *array){
  byte len;
  len = cal(array);
  if(len == len / 2 * 2) {  len = centerPoint - 12 * len / 2;  } //Even
  else {  len = centerPoint - 6 - 12 * len / 2;  } //Odd
  tft.setCursor(len, yCoord);
  tft.println(array);
}

void printText(int xCoord, byte yCoord, char *array){
  tft.setCursor(xCoord, yCoord);
  tft.println(array);
}

void makeTitle(char *title){
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);

  tft.drawFastVLine(70, 0, 25, ILI9341_WHITE);
  tft.drawFastHLine(0, 25, 320, ILI9341_WHITE);

  printText(4, 5, "Back");
  centerText(195, 5, title);
}

void doubleLine(int xCoord, byte yCoord, int width, uint16_t color){
  tft.drawFastHLine(xCoord, yCoord, width, color);
  tft.drawFastHLine(xCoord, yCoord + 1, width, color);
}
/* Possible Future Use
void displayList(int xCenter, byte yCoord, char *array){
  byte tmp = 0;
  while(tmp < 8) {  //while list not empty
    centerText(xCenter, yCoord + tmp * 18, array);
    tmp++;
  }
}*/
