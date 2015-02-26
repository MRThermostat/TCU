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

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RESET);

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300); //Replace 300 with actual resistance

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

  defaults(0); //run the set defaults
}

void loop(){ //Main Screen
  TSPoint p;
  int temp = -10; //temporary for testing
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  
  tft.drawFastVLine(160, 0, 54, ILI9341_WHITE);
  tft.drawFastHLine(0, 54, 320, ILI9341_WHITE);
  tft.drawFastHLine(0, 55, 320, ILI9341_WHITE);
  tft.drawFastHLine(0, 80, 320, ILI9341_WHITE);
  tft.drawFastVLine(160, 80, 49, ILI9341_WHITE);
  tft.drawFastHLine(0, 129, 320, ILI9341_WHITE);
  tft.drawFastHLine(0, 130, 320, ILI9341_WHITE);
  tft.drawFastHLine(0, 209, 320, ILI9341_WHITE);
  tft.drawFastHLine(0, 210, 320, ILI9341_WHITE);
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

  //Weather
  tft.setCursor(40, 4);
  tft.println("High:");
  unitPos(100, 4, temp);
  tft.setCursor(52, 20);
  tft.println("Low:");
  unitPos(100, 20, temp);
  tft.setCursor(4, 36);
  tft.println("Outside:");
  unitPos(100, 36, temp);
  
  //Date/Time
  tft.setCursor(165, 4);
  tft.println("1/20/2014");
  tft.setCursor(165, 20);
  tft.println("12:00 PM");
  
  //Active Profile
  tft.setCursor(4, 60);
  tft.println("Active Profile123456");
  
  //Current Temp
  tft.setCursor(4, 100);
  tft.println("Current:");
  unitPos(100, 100, temp);
  
  //Desired Temp
  tft.setCursor(164, 100);
  tft.println("Desired:");
  unitPos(260, 100, temp);
  
  //HVAC System
  tft.setCursor(0, 150);
  tft.println("Fan: On  Off  Auto");
  tft.setCursor(0, 180);
  tft.println("System: Heat  Cool  Blower");
    //check which setting is active
    tft.drawRect(55, 145, 34, 26, ILI9341_WHITE); //On
    tft.drawRect(103, 145, 46, 26, ILI9341_WHITE); //Off
    tft.drawRect(163, 145, 58, 26, ILI9341_WHITE); //Auto

    //check which setting is active
    tft.drawRect(91, 175, 58, 26, ILI9341_WHITE); //Heat
    tft.drawRect(163, 175, 58, 26, ILI9341_WHITE); //Cool
    tft.drawRect(235, 175, 82, 26, ILI9341_WHITE); //Blower
    
  //Sensor List
  tft.setCursor(20, 219);
  tft.println("Sensor 1:");
  tft.setCursor(140, 219);
  tft.println("69F");
  
  //Settings
  tft.drawFastHLine(290, 215, 25, ILI9341_WHITE);
  tft.drawFastHLine(290, 216, 25, ILI9341_WHITE);
  tft.drawFastHLine(290, 225, 25, ILI9341_WHITE);
  tft.drawFastHLine(290, 226, 25, ILI9341_WHITE);
  tft.drawFastHLine(290, 234, 25, ILI9341_WHITE);
  tft.drawFastHLine(290, 235, 25, ILI9341_WHITE);
  
  do{
    p = ts.getPoint();
    //Check for Android Connection
  }while(p.z < MINPRESSURE || p.z > MAXPRESSURE);
  
  Serial.print("\nX = "); Serial.print(p.x);
  Serial.print("\tY = "); Serial.print(p.y);
  Serial.print("\tPressure = "); Serial.println(p.z);

  if(p.x > 712) {
    if(p.y < 535) {  updateWeather();  } //Force weather update
    else {  dateSettings();  } //Time/Date Settings
  }
  else if(p.x > 640) {  profileSettings();  } //Profile Settings
  else if (p.x > 497) {
    if(p.y < 535) {  /*sensorSettings();*/  } //Sensor Settings
    else {  changeTemp();  } //Change Temperature
  } 
  else if(p.x > 223) {  hvacSettingChange();  } //HVAC System (needs work)
  else if(p.y < 186) {  cycleSensorList(0);  } //Left Arrow for Sensor List
  //else if(p.y < 799) {  sensorSettings();  } //Sensor Settings
  else if(p.y < 824) {  cycleSensorList(1);  } //Right Arrow for Sensor List
  else {  settings();  } //Settings
}

//Settings Screen
void settings(){
  TSPoint p;
  
  do{
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    
    tft.drawFastVLine(70, 0, 25, ILI9341_WHITE);
    tft.drawFastHLine(0, 25, 320, ILI9341_WHITE);
    tft.drawFastHLine(0, 78, 320, ILI9341_WHITE);
    tft.drawFastHLine(0, 131, 320, ILI9341_WHITE);
    tft.drawFastHLine(0, 184, 320, ILI9341_WHITE);
    
    //Back
    tft.setCursor(4, 5);
    tft.println("Back");
    
    //Title
    tft.setCursor(75, 5);
    tft.println("Device Settings");
    
    //Profile
    tft.setCursor(64, 43);
    tft.println("Profile Settings");
    
    //Sensor
    tft.setCursor(70, 96);
    tft.println("Sensor Settings");
    
    //WiFi
    tft.setCursor(82, 149);
    tft.println("WiFi Settings");
    
    //Defaults
    tft.setCursor(112, 202);
    tft.println("Defaults");
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
        else if(p.x > 493) {  /*sensorSettings();*/  } //Sensor Settings
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
    
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    
    tft.drawFastVLine(70, 0, 25, ILI9341_WHITE);
    tft.drawFastHLine(0, 25, 320, ILI9341_WHITE);
    tft.drawFastHLine(0, 65, 320, ILI9341_WHITE);
    tft.drawFastHLine(0, 215, 320, ILI9341_WHITE); 
    
    //Back
    tft.setCursor(4, 5);
    tft.println("Back");
    
    //Title
    tft.setCursor(75, 5);
    tft.println("Profile Settings");
    
    //Active Name
    tft.setCursor(62, 28);
    tft.println("Currently Active");
    centerText(46, nam);
    
    //Profile List
    while(tmp < 8) {  //while list not empty
      centerText(70 + tmp * 18, nam);
      tmp++;
    }
    
    //Edit
    tft.setCursor(124, 220);
    tft.println("Edit");
    do{
      do{
        p = ts.getPoint();
        //Check for Android Connection
      }while(p.z < MINPRESSURE || p.z > MAXPRESSURE);
    
      if(p.x > 798 && p.y < 318) {  return;  } //Back
      else if(p.x > 683 && p.x < 798) {  changeActive();  } //Rules
      else if(p.x < 252) {  editProfile();  } //Edit Profile
    }while(p.x > 798 && p.y > 318);
  }while(true);
}
/*
//Sensor Settings Screen
void sensorSettings(){
  TSPoint p;
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_BLACK);
  tft.setTextSize(1);
  
  tft.fillRect(xb, yb, wb, hb, ILI9341_WHITE); //Back
  tft.setCursor(xb, yb);
  tft.println("Back");
  
  tft.fillRect(xt, yt, wt, ht, ILI9341_WHITE); //Title
  tft.setCursor(xt, yt);
  tft.println("Sensor Settings");
  
  tft.fillRect(xa, ya, wa, ha, ILI9341_WHITE); //Active List
  tft.setCursor(xa, ya);
  tft.println("Active");
  while(listnotempty && yl + i < TS_MAXY) {
    tft.setCursor(xl, yl + i);
    if(sensorNameisactive) {  tft.println(sensorName + i);  }
    i++;
  }
  
  tft.fillRect(xs, ys, ws, hs, ILI9341_WHITE); //Sensor List
  tft.setCursor(xa, ya);
  tft.println("Sensors");
  while(listnotempty && yl + i < TS_MAXY) {
    tft.setCursor(xl, yl + i);
    tft.println(sensorName + i);
    i++;
  }
  
  tft.fillRect(xe, ye, we, he, ILI9341_WHITE); //Edit
  tft.setCursor(xe, ye);
  tft.println("Edit");
  
  do{
    do{
      TSPoint p = ts.getPoint();
      //Check for Android Connection
    }while(p.z < MINPRESSURE || p.z > MAXPRESSURE);
  
    if(p.x < wb && p.y < ht) {  return;  } //Back
    else if(p.x < wa) {  activeList();  } //Active List
    else if(p.y > ye) {
      tmp = (p.y - ye)/i; //Finds Which sensor was pressed(i is size of touch box)
      sensorSettings(tmp);
    }
  }while(true);
}

void activeList(){
  TSPoint p;
  byte tmp;
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_BLACK);
  tft.setTextSize(1);
  
  tft.fillRect(xb, yb, wb, hb, ILI9341_WHITE); //Back
  tft.setCursor(xb, yb);
  tft.println("Back");
  
  tft.fillRect(xt, yt, wt, ht, ILI9341_WHITE); //Title
  tft.setCursor(xt, yt);
  tft.println("Sensor Settings");
  
  tft.fillRect(xa, ya, wa, ha, ILI9341_WHITE); //Title
  tft.setCursor(xa, ya);
  tft.println("Set Active Sensors");
  
  while(listnotempty && yl + i < TS_MAXY) { //Sensor List
    tft.setCursor(xl, yl + i);
    if(sensorNameisactive) {  tft.setTextColor(ILI9341_RED);  }
    else {  tft.setTextColor(ILI9341_BLACK);  }
    tft.println(sensorName + i);
    i++;
  }
  
  do{
    do{
      TSPoint p = ts.getPoint();
      //Check for Android Connection
      }while(p.z < MINPRESSURE || p.z > MAXPRESSURE);
  
    if(p.x < wb && p.y < ht) {  return;  } //Back
    else if(p.y > yl){
      tmp = (p.y - yl)/i; //Finds Which sensor was pressed
      //go to sensor list and change active value using sensorActive ^ 1;
      tft.fillRect(xa, ya, wa, ha, ILI9341_WHITE); //Update List
      while(listnotempty && yl + i < TS_MAXY) {
        tft.setCursor(xl, yl + i);
        if(sensorNameisactive) {  tft.setTextColor(ILI9341_RED);  }
        else {  tft.setTextColor(ILI9341_BLACK);  }
        tft.println(sensorName + i);
        i++;
      }
    }
  }while(true);
}

void sensorEdit(byte sensorNumber){
  //Find correct sensor
  TSPoint p;
  byte tmp;
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_BLACK);
  tft.setTextSize(1);
  
  tft.fillRect(xb, yb, wb, hb, ILI9341_WHITE); //Back
  tft.setCursor(xb, yb);
  tft.println("Back");
  
  tft.fillRect(xt, yt, wt, ht, ILI9341_WHITE); //Title
  tft.setCursor(xt, yt);
  tft.println("Sensor Settings");
  
  tft.fillRect(xi, yi, wi, hi, ILI9341_WHITE); //Info Box
  
  tft.setCursor(xn, yn);
  tft.println("Name: " + sensorName);
  
  tft.setCursor(xl, yl);
  tft.println("Latest Temperature: " + sensorTemp);
  
  tft.setCursor(xa, ya);
  tft.println("Active:");
  
  if(sensorActive) {  tft.drawRect(x, y, w, h, ILI9341_BLACK);  } //Box around Yes
  else {  tft.drawRect(x, y, w, h, ILI9341_BLACK);  } //Box around No
  
  tft.setCursor(xy, yy);
  tft.println("Yes");
  tft.setCursor(xo, yo);
  tft.println("No");
  
  tft.setCursor(xs, ys);
  tft.println("Sensor ID: " + sensorID);
  
  tft.setCursor(xb, yb);
  tft.println("Battery Stats: " + sensorBattery);
  
  do{
    do{
      TSPoint p = ts.getPoint();
      //Check for Android Connection
    }while(p.z < MINPRESSURE || p.z > MAXPRESSURE);
  
    if(p.x < wb && p.y < ht) {  return;  } //Back
    else if(p.y > ht && p.y < yl) {  changeName(0, sensorNumber);  }
    else if(p.y > ya && p.y < ys) {
      //go to sensor list and change active value using sensorActive ^ 1
      if(sensorActive) {
        tft.drawRect(x, y, w, h, ILI9341_BLACK); //Box around Yes
        tft.drawRect(x, y, w, h, ILI9341_WHITE); //Clear Box around No
      } else {
        tft.drawRect(x, y, w, h, ILI9341_BLACK); //Box around No
        tft.drawRect(x, y, w, h, ILI9341_WHITE); //Clear Box around Yes
      }
    }
  }while(true);
}

//Change Sensor Name
void changeName(bool sensorProfile,byte number){ //0 denotes a sensor, 1 denotes a profile
  keyboard();
 
  return;
}*/

//Obtain newest Weather info
void updateWeather(){
  //grab latest weather info
  Serial.print("Weather\n");
}

//Change Desired Temperature Screen
void changeTemp(){
  TSPoint p;
  byte temp = 99; // Must remove '= 100' before final product
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  
  tft.drawFastVLine(70, 0, 25, ILI9341_WHITE);
  tft.drawFastHLine(0, 25, 320, ILI9341_WHITE);
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
  
  //Back
  tft.setCursor(4, 5);
  tft.println("Back");
  //Title
  tft.setCursor(75, 5);
  tft.println("Change Temperature");
  //Accept
  tft.setCursor(124, 215);
  tft.println("Accept");
  //Temperature
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
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  
  tft.drawFastVLine(70, 0, 25, ILI9341_WHITE);
  tft.drawFastHLine(0, 25, 320, ILI9341_WHITE);
  tft.drawFastHLine(0, 107, 320, ILI9341_WHITE);
  
  //Back
  tft.setCursor(4, 5);
  tft.println("Back");
  
  //Title
  tft.setCursor(75, 5);
  tft.println("HVAC Settings");
  
  //Fan Area
  tft.setCursor(146, 35);
  tft.println("Fan");
  tft.setCursor(61, 70);
  tft.println("On");
  tft.setCursor(146, 70);
  tft.println("Off");
  tft.setCursor(216, 70);
  tft.println("Auto");
  /*if(onActive) {  tft.drawRect(56, 65, 34, 26, ILI9341_BLACK);  } //Box around active setting
  if(offActive) {  tft.drawRect(141, 65, 46, 26, ILI9341_BLACK);  }
  if(autoActive) {  tft.drawRect(211, 65, 58, 26, ILI9341_BLACK);  }*/
  
  //System Area
  tft.setCursor(124, 117);
  tft.println("System");
  tft.setCursor(51, 152);
  tft.println("Heat");
  tft.setCursor(136, 152);
  tft.println("Cool");
  tft.setCursor(204, 152);
  tft.println("Blower");
  /*if(heatActive) {  tft.drawRect(46, 147, 58, 26, ILI9341_BLACK);  } //Box around active setting
  if(coolActive) {  tft.drawRect(131, 147, 58, 26, ILI9341_BLACK);  }
  if(blowerActive) {  tft.drawRect(199, 147, 82, 26, ILI9341_BLACK);  }*/
  do{
    do{
      p = ts.getPoint();
      //Check for Android Connection
    }while(p.z < MINPRESSURE || p.z > MAXPRESSURE);
    
    if(p.x > 562) {
      if(p.x > 798 && p.y < 318) {  return;  }
      else if(p.x < 798) {
        tft.drawRect(56, 65, 34, 26, ILI9341_BLACK);
        tft.drawRect(141, 65, 46, 26, ILI9341_BLACK);
        tft.drawRect(211, 65, 58, 26, ILI9341_BLACK);
        if(p.y < 405) {
          tft.drawRect(56, 65, 34, 26, ILI9341_WHITE);
          //Then Change setting
        }
        else if(p.y < 660) {
          tft.drawRect(141, 65, 46, 26, ILI9341_WHITE);
          //Then Change setting
        }
        else {
          tft.drawRect(211, 65, 58, 26, ILI9341_WHITE);
          //Then Change setting
        }
      }
    }
    else {
      tft.drawRect(46, 147, 58, 26, ILI9341_BLACK);
      tft.drawRect(131, 147, 58, 26, ILI9341_BLACK);
      tft.drawRect(199, 147, 82, 26, ILI9341_BLACK);
      if(p.y < 405) {
        tft.drawRect(46, 147, 58, 26, ILI9341_WHITE);
        //Then Change setting
      }
      else if(p.y < 660) {
        tft.drawRect(131, 147, 58, 26, ILI9341_WHITE);
        //Then Change setting
      }
      else {
        tft.drawRect(199, 147, 82, 26, ILI9341_WHITE);
        //Then Change setting
      } 
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
  Serial.println("Edit");}/*
  byte i = 0;
  TSPoint p;
  tft.fillScreen(ILI9341_BLACK);
  
  tft.setTextColor(ILI9341_BLACK);
  tft.setTextSize(1);
  tft.fillRect(xb, yb, wb, hb, ILI9341_WHITE); //Back
  tft.setCursor(xb, yb);
  tft.println("Back");
  
  tft.fillRect(xt, yt, wt, ht, ILI9341_WHITE); //Title
  tft.setCursor(xt, yt);
  tft.println("Profile Settings");
  
  //Might add scroll button
  
  while(listnotempty && yl + i < TS_MAXY) { //Profile List
    tft.fillRect(xl, yl + i, wl, hl, ILI9341_WHITE);
    tft.setCursor(xl, yl + i);
    tft.println(profileName + i);
    i++;
  }
  
  do{
    do{
      TSPoint p = ts.getPoint();
      //Check for Android Connection
      }while(p.z < MINPRESSURE || p.z > MAXPRESSURE);
  
    if(p.x < wb && p.y < ht) {  return;  } //Back
    //more work here
  }while(true);
}

//Rules Edit Screen
void editRules(){
  byte i = 0;
  TSPoint p;
  tft.fillScreen(ILI9341_BLACK);
  
  tft.setTextColor(ILI9341_BLACK);
  tft.setTextSize(1);
  tft.fillRect(xb, yb, wb, hb, ILI9341_WHITE); //Back
  tft.setCursor(xb, yb);
  tft.println("Back");
  
  tft.fillRect(xt, yt, wt, ht, ILI9341_WHITE); //Title
  tft.setCursor(xt, yt);
  tft.println("Profile Settings");
  
  tft.fillRect(xa, ya, wa, ha, ILI9341_WHITE); //Active Name
  tft.setCursor(xa, ya);
  tft.println("Name: ");
  
  tft.setCursor(xr, yr);
  tft.println("Rules:");
  
  while(listnotempty && yl + i < TS_MAXY) { //Rule List
    tft.fillRect(xl, yl + i, wl, hl, ILI9341_WHITE);
    tft.setCursor(xl, yl + i);
    tft.println(profileRules + i);
    i++;
  }  
}
*/
//Change Active Profile Screen
void changeActive(){
  Serial.println("Active");
}

//WiFi Settings
void wifiSettings(){
  Serial.println("WiFi");
  
}
/*
//Touch Keyboard
void keyboard(char *nameArray){
  TSPoint p;
  byte count;
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_BLACK);
  tft.setTextSize(1);
  
  tft.fillRect(xtb, ytb, wtb, htb, ILI9341_WHITE); //Text Box Area
  tft.setCursor(xback, yback); // Back
  tft.println("Back");
  tft.drawFastVLine(xbackline, 0, htb, ILI9341_BLACK);
  tft.setCursor(x, y);
  tft.println("Max 20 Characters");
  tft.fillRect(xka, yka, wka, hka, ILI9341_BLACK); //Keyboard Area

  tft.drawFastHLine(0, yl1, wscreen, ILI9341_WHITE);
  tft.drawFastHLine(0, yl2, wscreen, ILI9341_WHITE);
  tft.drawFastHLine(0, yl3, wscreen, ILI9341_WHITE);
  tft.drawFastHLine(0, yl4, wscreen, ILI9341_WHITE);
  tft.drawFastVLine(xl1, yka, hscreen - htb-rowheight, ILI9341_WHITE);
  tft.drawFastVLine(xl2, yka, hscreen - htb, ILI9341_WHITE);
  tft.drawFastVLine(xl3, yka, hscreen - htb-rowheight, ILI9341_WHITE);
  tft.drawFastVLine(xl4, yka, hscreen - htb-rowheight, ILI9341_WHITE);
  tft.drawFastVLine(xl5, yka, hscreen - htb-rowheight, ILI9341_WHITE);
  tft.drawFastVLine(xl6, yka, hscreen - htb-rowheight, ILI9341_WHITE);
  tft.drawFastVLine(xl7, yka, hscreen - htb-rowheight, ILI9341_WHITE);
  tft.drawFastVLine(xl8, yka, hscreen - htb, ILI9341_WHITE);
  tft.drawFastVLine(xl9, yka, hscreen - htb-rowheight, ILI9341_WHITE);
  
  //Row 1
  tft.setCursor(x1, y1); //1
  tft.println("1");
  tft.setCursor(x2, y2); //2
  tft.println("2");
  tft.setCursor(x3, y3); //3
  tft.println("3");
  tft.setCursor(x4, y4); //4
  tft.println("4");
  tft.setCursor(x5, y5); //5
  tft.println("5");
  tft.setCursor(x6, y6); //6
  tft.println("6");
  tft.setCursor(x7, y7); //7
  tft.println("7");
  tft.setCursor(x8, y8); //8
  tft.println("8");
  tft.setCursor(x9, y9); //9
  tft.println("9");
  tft.setCursor(x0, y0); //0
  tft.println("0");
  
  //Row 2
  tft.setCursor(xq, yq); //q
  tft.println("q");
  tft.setCursor(xw, yw); //w
  tft.println("w");
  tft.setCursor(xe, ye); //e
  tft.println("e");
  tft.setCursor(xr, yr); //r
  tft.println("r");
  tft.setCursor(xt, yt); //t
  tft.println("t");
  tft.setCursor(xy, yy); //y
  tft.println("y");
  tft.setCursor(xu, yu); //u
  tft.println("u");
  tft.setCursor(xi, yi); //i
  tft.println("i");
  tft.setCursor(xo, yo); //o
  tft.println("o");
  tft.setCursor(xp, yp); //p
  tft.println("p");
  
  //Row 3
  tft.setCursor(xa, ya); //a
  tft.println("a");
  tft.setCursor(xs, ys); //s
  tft.println("s");
  tft.setCursor(xd, yd); //d
  tft.println("d");
  tft.setCursor(xf, yf); //f
  tft.println("f");
  tft.setCursor(xg, yg); //g
  tft.println("g");
  tft.setCursor(xh, yh); //h
  tft.println("h");
  tft.setCursor(xj, yj); //j
  tft.println("j");
  tft.setCursor(xk, yk); //k
  tft.println("k");
  tft.setCursor(xl, yl); //l
  tft.println("l");
  tft.setCursor(xap, yap); //'
  tft.println("'");
  
  //Row 4
  tft.setCursor(xz, yz); //z
  tft.println("z");
  tft.setCursor(xx, yx); //x
  tft.println("x");
  tft.setCursor(xc, yc); //c
  tft.println("c");
  tft.setCursor(xv, yv); //v
  tft.println("v");
  tft.setCursor(xb, yb); //b
  tft.println("b");
  tft.setCursor(xn, yn); //n
  tft.println("n");
  tft.setCursor(xm, ym); //m
  tft.println("m");
  tft.setCursor(xcomma, ycomma); //,
  tft.println(",");
  tft.setCursor(xperiod, yperiod); //.
  tft.println(".");
  tft.setCursor(xslash, yslash); ///
  tft.println("/");
  
  //Row 5
  tft.setCursor(xspace, yspace); // space
  tft.println("SPACE");
  tft.setCursor(xenter, yenter); // enter
  tft.println("ENTER");
  tft.setCursor(xbspace, ybspace); // backspace
  tft.println("BACKSPACE");
  count = 0;
  
  do{
    nameArray[count] = keyboardTouch();
    if(nameArray[count] == '`') { //Check for cancel
      nameArray[0] = '\0';
      return;
    }
    else if(nameArray[count] == '-') { //Check for backspace
      if(count == 0) {  break;  } //Does nothing if empty
      else { //Clear most recent value
        nameArray[count] = nameArray[count - 1] = '\0';
        count--;
      }
    }
    else if(nameArray[count] == '+') {
      nameArray[count] = '\0';
      return;
    }
    else {  count++;  }
  }while(count < 20);
}

char keyboardTouch(){
  TSPoint p;
  do{
    do{
      TSPoint p = ts.getPoint();
      //Check for Android Connection
      }while(p.z < MINPRESSURE || p.z > MAXPRESSURE);
    if(p.y < yka && p.x < xbackline) {  return '`';  } //Back send exit command
    else if(p.y > yka && p.y < yl1) { //Row 1
      if(p.x < xl1) {  return '1';  } //Column 1
      else if(p.x < xl2) {  return '2';  } //Column 2
      else if(p.x < xl3) {  return '3';  } //Column 3
      else if(p.x < xl4) {  return '4';  } //Column 4
      else if(p.x < xl5) {  return '5';  } //Column 5
      else if(p.x < xl6) {  return '6';  } //Column 6
      else if(p.x < xl7) {  return '7';  } //Column 7
      else if(p.x < xl8) {  return '8';  } //Column 8
      else if(p.x < xl9) {  return '9';  } //Column 9
      else if(p.x < xl0) {  return '0';  } //Column 10
    }
    else if(p.y < yl2) { //Row 2
      if(p.x < xl1) {  return 'q';  } //Column 1
      else if(p.x < xl2) {  return 'w';  } //Column 2
      else if(p.x < xl3) {  return 'e';  } //Column 3
      else if(p.x < xl4) {  return 'r';  } //Column 4
      else if(p.x < xl5) {  return 't';  } //Column 5
      else if(p.x < xl6) {  return 'y';  } //Column 6
      else if(p.x < xl7) {  return 'u';  } //Column 7
      else if(p.x < xl8) {  return 'i';  } //Column 8
      else if(p.x < xl9) {  return 'o';  } //Column 9
      else if(p.x < xl0) {  return 'p';  } //Column 10
    }
    else if(p.y < yl3) { //Row 3
      if(p.x < xl1) {  return 'a';  } //Column 1
      else if(p.x < xl2) {  return 's';  } //Column 2
      else if(p.x < xl3) {  return 'd';  } //Column 3
      else if(p.x < xl4) {  return 'f';  } //Column 4
      else if(p.x < xl5) {  return 'g';  } //Column 5
      else if(p.x < xl6) {  return 'h';  } //Column 6
      else if(p.x < xl7) {  return 'j';  } //Column 7
      else if(p.x < xl8) {  return 'k';  } //Column 8
      else if(p.x < xl9) {  return 'l';  } //Column 9
      else if(p.x < xl0) {  return '\'';  } //Column 10
    }
    else if(p.y < y14) { //Row 4
      if(p.x < xl1) {  return 'z';  } //Column 1
      else if(p.x < xl2) {  return 'x';  } //Column 2
      else if(p.x < xl3) {  return 'c';  } //Column 3
      else if(p.x < xl4) {  return 'v';  } //Column 4
      else if(p.x < xl5) {  return 'b';  } //Column 5
      else if(p.x < xl6) {  return 'n';  } //Column 6
      else if(p.x < xl7) {  return 'm';  } //Column 7
      else if(p.x < xl8) {  return ',';  } //Column 8
      else if(p.x < xl9) {  return '.';  } //Column 9
      else if(p.x < xl0) {  return '/';  } //Column 10
    }
    else if(p.y > yl4) { //Row 5
      if(p.x < xl2) {  return '-';  } //Column 1 need backspace command
      else if(p.x < xl8) {  return ' ';  } //Column 2
      else if(p.x > xl8) {  return '+';  } //Column 3 need enter command
    }
  }while(true);
}*/

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

//Character Array Length
byte cal(char *array){
  byte count = 0;
  while(array[count] != '\0') {  count++;  };
  return count;
}

void centerText(byte yCoord, char *array){
  byte len;
  len = cal(array);
  if(len == len / 2 *2) {  len = 160 - 12 * len / 2;  } //Even
  else {  len = 154 - 12 * len / 2;  } //Odd
  tft.setCursor(len, yCoord);
  tft.println(array);
}
