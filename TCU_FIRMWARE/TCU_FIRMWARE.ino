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
  
  if(p.x < wt && p.y > yt) {  dateSettings();  } //Date/Time
  else if(p.x > xp && p.y > yp) {  profileSettings();  } //Active Profile
  else if(p.x > xc && p.y > yc) {  sensorSettings();  } //Current Temp
  else if(p.x < ww && p.y > yw) {  updateWeather();  } //Weather
  else if(p.x > xd && p.y > yd) {  changeTemp();  } //Desired Temp
  else if(p.y > yh) {  hvacSettingChange();  } //HVAC System (needs work)
  else if(p.x < xd) {  cycleSensorList(0);  } //Left arrow for Sensor List
  else if(p.x < xl + wl) {  sensorSettings();  } //Sensor List
  else if(p.x < xd + i) {  cycleSensorList(1);  } //Right arrow for Sensor List
  else if(p.x > xs) {  settings();  } //Settings
  
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
  tft.println("Active: " + someName);
  
  tft.fillRect(xl, yl, wl, hl, ILI9341_WHITE); //Profile List
  tft.setCursor(xl, yl);
  tft.println(someName);
  
}

//Profile Settings Screen
void sensorSettings(){
  
}

//Profile Settings Screen
void updateWeather(){
  //grab latest weather info
}

//Profile Settings Screen
void changeTemp(){
  
}

//Profile Settings Screen
void hvacSettingChange(){
  
}

//Profile Settings Screen
void cycleSensorList(boolean directions){
  
}

//Profile Settings Screen
void settings(){
  
}

//Profile Settings Screen
void defaults(byte action){
  if(action == 0) { //Default
    
  }
  else if(action == 1) { //Write
    
  }
  else if(action == 2) { //Read
    
  }
  
}
