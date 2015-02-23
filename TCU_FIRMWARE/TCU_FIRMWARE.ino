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
  while(listnotempty) {  tft.println(profileName);  }
  
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
  TSPoint p;
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_BLACK);
  tft.setTextSize(1);
  
  tft.fillRect(xb, yb, wb, hb, ILI9341_WHITE); //Back
  tft.setCursor(xb, yb);
  tft.println("Back");
  
  tft.fillRect(xt, yt, wt, ht, ILI9341_WHITE); //Title
  tft.setCursor(xt, yt);
  tft.println("HVAC Settings");
  
  tft.fillRect(xf, yf, wf, hf, ILI9341_WHITE); //Fan Area
  tft.setCursor(xon, yon);
  tft.println("On");
  tft.setCursor(xoff, yoff);
  tft.println("Off");
  tft.setCursor(xa, ya);
  tft.println("Auto");
  if(onActive) {  tft.drawRect(x, y, w, h, ILI9341_BLACK);  } //Box around active setting
  if(offActive) {  tft.drawRect(x, y, w, h, ILI9341_BLACK);  } 
  if(autoActive) {  tft.drawRect(x, y, w, h, ILI9341_BLACK);  } 
  
  tft.fillRect(xf, yf, wf, hf, ILI9341_WHITE); //System Area
  tft.setCursor(xh, yh);
  tft.println("Heat");
  tft.setCursor(xc, yc);
  tft.println("Cool");
  tft.setCursor(xbl, ybl);
  tft.println("Blower");
  if(heatActive) {  tft.drawRect(x, y, w, h, ILI9341_BLACK);  } //Box around active setting
  if(coolActive) {  tft.drawRect(x, y, w, h, ILI9341_BLACK);  } 
  if(blowerActive) {  tft.drawRect(x, y, w, h, ILI9341_BLACK);  } 
  
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

//Rotates Sensor List
void cycleSensorList(boolean directions){
  if(directions == 0) { //Left
    
  }
  else if(directions == 1) { //Right
    
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
  
  while(listnotempty && yl + i < TS_MAXY) { //Rule List
    tft.fillRect(xl, yl + i, wl, hl, ILI9341_WHITE);
    tft.setCursor(xl, yl + i);
    tft.println(profileRules + i);
    i++;
  }  
}

//Change Active Profile Screen
void changeActive(){
  
}

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
}
