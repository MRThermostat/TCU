#include <TouchScreen.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>
#include "pin_definitions.h"
#include <SPI.h>

#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

#define MINPRESSURE 10
#define MAXPRESSURE 1000

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300); //Replace 300 with actual resistance

void setup(void) {
  //open up serial comms
  //be careful with this, as this channel also connects to ESP8266
  Serial.begin(9600);
  Serial.println("Mr Thermostat");
  
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);

  defaults(0); //run the set defaults
}

void loop(void) {
  TSPoint p;
  mainScreen();
  do{
  TSPoint p = ts.getPoint();
  //Check for Android Connection
  }while(p.z < MINPRESSURE || p.z > MAXPRESSURE);
  
  if(p.x > xs && p.y > ys) {  settings();  } //Settings
  else if(p.x > rx && p.y > yl) {  cycleSensorList(1);  } //Right arrow for Sensor List
  else if(p.x > xl && p.y > yl) {  sensorSettings();  } //Sensor List
  else if(p.x < xl && p.y > yl) {  cycleSensorList(0);  } //Left arrow for Sensor List
  else if(p.y > yh) {  hvacSettingChange();  } //HVAC System (needs work)
  else if(p.x > xd && p.y > yd) {  changeTemp();  } //Desired Temp
  else if(p.x < ww && p.y > yw) {  updateWeather();  } //Weather
  else if(p.x > xc && p.y > yc) {  sensorSettings();  } //Current Temp
  else if(p.x > xp && p.y > yp) {  profileSettings();  } //Active Profile
  else if(p.x < wt) {  dateSettings();  } //Date/Time
  
  return;
}

//Main Screen
void mainScreen(){
  tft.setTextColor(ILI9341_BLACK);
  tft.setTextSize(1);
  tft.fillRect(xt, yt, wt, ht, ILI9341_WHITE); //Date/Time
  tft.setCursor(xt, yt);
  tft.println("Hello World!");
  tft.println("Hello World!");
  
  tft.fillRect(xp, yp, wp, hp, ILI9341_WHITE); //Active Profile
  tft.setCursor(xt, yt);
  tft.println("Hello World!");
  
  tft.fillRect(xc, yc, wc, hc, ILI9341_WHITE); //Current Temp
  tft.setCursor(xt, yt);
  tft.println("Hello World!");
  
  tft.fillRect(xw, yw, ww, hw, ILI9341_WHITE); //Weather
  tft.setCursor(xt, yt);
  tft.println("Hello World!");
  tft.println("Hello World!");
  tft.println("Hello World!");
  
  tft.fillRect(xd, yd, wd, hd, ILI9341_WHITE); //Desired Temp
  tft.setCursor(xt, yt);
  tft.println("Hello World!");
  
  tft.fillRect(xh, yh, wh, hh, ILI9341_WHITE); //HVAC System
  tft.setCursor(xt, yt);
  tft.println("Hello World!");
  tft.println("Hello World!");
  
  tft.drawTriangle(                        // Left Arrow
                   lx, ly - i,             // peak
                   lx - i, ly + i,         // bottom left
                   lx, ly + 2 * i,         // bottom right
                   ILI9341_WHITE);
  tft.drawTriangle(                        // Right Arrow
                   rx, ry - i,             // peak
                   rx, ry + 2 * i,         // bottom left
                   rx + i, ry + i,         // bottom right
                   ILI9341_WHITE);
                   
  tft.fillRect(xl, yl, wl, hl, ILI9341_WHITE); //Sensor List
  tft.setCursor(xt, yt);
  tft.println("Hello World!");
  
  tft.fillRect(xs, ys, ws, hs, ILI9341_WHITE); //Settings
  tft.setCursor(xt, yt);
  tft.println("Hello World!");
  
  return;
}

//Date/Time Setting Screen
void dateSettings(){
  tft.setCursor(xt, yt);
  tft.println("Hello World!");
  
  
}

//Profile Settings Screen
void profileSettings(){
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
  tft.println("Active: ");
  
  tft.fillRect(xl, yl, wl, hl, ILI9341_WHITE); //Profile List
  tft.setCursor(xl, yl);
  while(listnotempty) {  tft.println(someName);  }
  
  tft.fillRect(xe, ye, we, he, ILI9341_WHITE); //Edit
  tft.setCursor(xe, ye);
  tft.println("Edit");
  do{
    do{
      TSPoint p = ts.getPoint();
      //Check for Android Connection
    }while(p.z < MINPRESSURE || p.z > MAXPRESSURE);
  
    if(p.y > ye) {  editProfile();  } //Edit Profile
    else if(p.y > yl) {  changeActive();  } //Rules
    else if(p.x < wb && p.y < hb) {  return;  } //Back
  }while(true);
}

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
  
  tft.fillRect(xs, ys, ws, hs, ILI9341_WHITE); //Sensor List
  tft.setCursor(xa, ya);
  tft.println("Sensors");
  
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
    else if(p.x > xe) {  sensorEdit();  } //Edit    
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

void sensorEdit(){
  
}

//Obtain newest Weather info
void updateWeather(){
  //grab latest weather info
}

//Change Desired Temperature Screen
void changeTemp(){
  
}

//HVAC Settings Screen
void hvacSettingChange(){
  
}

//Rotates Sensor List
void cycleSensorList(boolean directions){
  if(action == 0) { //Left
    
  }
  else if(action == 1) { //Right
    
  }
}

//Settings Screen
void settings(){
  
}

// Default actions
void defaults(byte action){
  if(action == 0) { //Default
    
  }
  else if(action == 1) { //Write
    
  }
  else if(action == 2) { //Read
    
  }
  
}

//Profile Edit Screen
void editProfile(){
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
  
  while(listnotempty && yl + i < TS_MAXY) { //Profile List
    tft.fillRect(xl, yl + i, wl, hl, ILI9341_WHITE);
    tft.setCursor(xl, yl + i);
    tft.println(profileRules + i);
    i++;
  }  
}

//Change Active Profile Screen
void changeActive(){
  
}
