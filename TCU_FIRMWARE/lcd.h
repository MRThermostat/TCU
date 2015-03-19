/*#define DISPLAY_POWER_MODE 1
#define MADCTL_MODE 1
#define PIXEL_FORMAT 1
#define IMAGE_FORMAT 1*/
#define SELF_DIAGNOSTIC 1

#include "defines.h"

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RESET);

void setupLCD() {
  //setup backlight
  pinMode(TFT_BACKLIGHT, OUTPUT);
  analogWrite(TFT_BACKLIGHT, 255);

  //initialize lcd
  tft.begin();
  tft.setRotation(3);
}

bool readDiagnostics() {
  // read diagnostics (optional but can help debug problems)
  uint8_t x = tft.readcommand8(ILI9341_RDMODE);
  //Serial.print("Display Power Mode: 0x"); Serial.println(x, HEX);
 /* if (x == DISPLAY_POWER_MODE)
  {
    x = tft.readcommand8(ILI9341_RDMADCTL);
    //Serial.print("MADCTL Mode: 0x"); Serial.println(x, HEX);
    if (x == MADCTL_MODE)
    {
      x = tft.readcommand8(ILI9341_RDPIXFMT);
      //Serial.print("Pixel Format: 0x"); Serial.println(x, HEX);
      if (x == PIXEL_FORMAT)
      {
        x = tft.readcommand8(ILI9341_RDIMGFMT);
        //Serial.print("Image Format: 0x"); Serial.println(x, HEX);
        if (x == IMAGE_FORMAT)
        {*/
          x = tft.readcommand8(ILI9341_RDSELFDIAG);
          //Serial.print("Self Diagnostic: 0x"); Serial.println(x, HEX);
          if (x == SELF_DIAGNOSTIC)
          {
            return (1);
          }/*
        }
      }
    }
  }*/

  return (0);
}

void printText(int xCoord, byte yCoord, char *array) {
  tft.setCursor(xCoord, yCoord);
  tft.println(array);
}

//Character Array Length
byte cal(char *array) {
  byte count = 0;
  while (array[count] != '\0') {  count++;  };
  return count;
}

void centerText(int centerPoint, byte yCoord, char *array) {
  byte len;
  len = cal(array);
  if (len == len / 2 * 2) {
    len = centerPoint - 12 * len / 2;   //Even
  }
  else {
    len = centerPoint - 6 - 12 * len / 2;   //Odd
  }
  printText(len, yCoord, array);
}

void makeTitle(char *title) {
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);

  tft.drawFastVLine(70, 0, 25, ILI9341_WHITE);
  tft.drawFastHLine(0, 25, 320, ILI9341_WHITE);

  printText(4, 5, "Back");
  centerText(195, 5, title);
}

int sensorBytes() {
  byte i;
  int sensor = 0;
  for(i = 0; i < MAXSENSORS; i++) {
    if(EEPROM.read(SENSORS + i * SENSORBLOCK) != 0){  bitSet(sensor,i);  }
  }
  return sensor;
}

//Change Active Profile Screen
void changeActive() {
  Serial.println("Active");
}

//Profile Settings Screen
void profileSettings() {
  char nam[21] = "Active Profile123456";
  do {
    byte tmp = 0;
    makeTitle("Profile Settings");
    tft.drawFastHLine(0, 65, 320, ILI9341_WHITE);
    printText(62, 28, "Currently Active");
    centerText(160, 46, nam);
    //Profile List
    while (tmp < 9) { //while list not empty
      centerText(160, 70 + tmp * 18, nam);
      tmp++;
    }
    do {
      do {
        p = ts.getPoint();
        //Check for Android Connection
      } while (p.z < MINPRESSURE || p.z > MAXPRESSURE);

      if (p.x > 798 && p.y < 318) {  return;   }//Back
      else if (p.x > 683 && p.x < 798) {  changeActive();  } //Rules
      else if (p.x < 684) {
        tmp = (668 - p.x) / 52; //Finds which profile was pressed
        Serial.print("Profile #");
        Serial.println(tmp);
      }
    } while (p.x > 798);
  } while (1);
}

void unitPos(int xCoord, byte yCoord, int temp) {
  tft.setCursor(xCoord, yCoord);
  tft.println(temp);
  if (temp < 100) {
    if (temp > 9) {  tft.setCursor(xCoord + 24, yCoord);  }
    else if (temp < 0) {
      if (temp > -10) {  tft.setCursor(xCoord + 24, yCoord);  }
      else {  tft.setCursor(xCoord + 36, yCoord);  }
    }
    else {  tft.setCursor(xCoord + 12, yCoord);  }
  }
  else {  tft.setCursor(xCoord + 36, yCoord);  }
  tft.println("F"); //Replace with unit
}

void sensorEdit(byte sensorNumber) {
//Required bytes: 27
//13 name
//11 sensor id
//1 sensor number
//1 latest temperature
//1 status & active
  byte temp;
  makeTitle("Sensor Settings");

  readEEPROMBytes(nam, SENSORS + sensorNumber * SENSORBLOCK, 13); //Read Sensor name from EEPROM
  printText(4, 30, "Name:");
  printText(62, 30, nam); //Grab sensor name
  temp = EEPROM.read(SENSORS + sensorNumber * SENSORBLOCK + 25); //Read Sensor temp from EEPROM
  printText(4, 60, "Latest Temperature:");
  unitPos(244 , 60, temp); //Grab latest Temperature
  
  readEEPROMBytes(nam, SENSORS + sensorNumber * SENSORBLOCK + 13, 11); //Read Sensor id from EEPROM
  printText(4, 120, "Sensor ID:");
  printText(130, 120, nam); //Grab sensor ID

  temp = EEPROM.read(SENSORS + sensorNumber * SENSORBLOCK + 26); //Read Sensor status and active from EEPROM
  printText(4, 90, "Active:");
  printText(100, 90, "Yes");
  printText(160, 90, "No");
  if(bitRead(temp, 7)) {  tft.drawRect(95, 85, 46, 26, ILI9341_WHITE);  } //Box around Yes
  else {  tft.drawRect(155, 85, 34, 26, ILI9341_WHITE);  } //Box around No

  printText(4, 150, "Battery Status:");
  printText(184, 150, "Replace"); //Grab sensor battery status

  do {
    do {
      p = ts.getPoint();
      //Check for Android Connection
    } while (p.z < MINPRESSURE || p.z > MAXPRESSURE);
    if (p.x > 717) {
      if (p.y < 318) {
        if(bitRead(temp, 7) != bitRead(EEPROM.read(SENSORS + sensorNumber * SENSORBLOCK + 26),7)) {  EEPROM.write(SENSORS + sensorNumber * SENSORBLOCK + 26,temp);  }
        return;   //Back
      }
    }
    else { //go to sensor list and change active value
      if (p.x > 545 && p.x < 631) {
          tft.drawRect(155, 85, 34, 26, ILI9341_BLACK);
          tft.drawRect(95, 85, 46, 26, ILI9341_BLACK);
        if (p.y > 361 && p.y < 506) {
          tft.drawRect(95, 85, 46, 26, ILI9341_WHITE);
          bitSet(temp, 7);
        }
        else if (p.y < 593) {
          tft.drawRect(155, 85, 34, 26, ILI9341_WHITE);
          temp = EEPROM.read(SENSORS + sensorNumber * SENSORBLOCK + 26); //Read Sensor status and active from EEPROM
          bitClear(temp, 7);
        }
      }
    }
  } while (1);
}

//Sensor Settings Screen
void sensorSettings() {
  TSPoint p;
  byte i, count, tmp;
  int temp = 0;
  do {
    i = tmp = 0;
    makeTitle("Sensor Settings");
    tft.drawFastVLine(160, 25, 215, ILI9341_WHITE);

    printText(50, 30, "Active");
    while (i < MAXSENSORS) { //while list not empty
      if(bitRead(EEPROM.read(SENSORS + i * SENSORBLOCK + 26), 7) == 1 && EEPROM.read(SENSORS + i * SENSORBLOCK) != '\0') {
        readEEPROMBytes(nam, SENSORS + i * SENSORBLOCK, 12);
        centerText(80, 60 + tmp * 18, nam);
        tmp++;
      }
      i++;
    }

    i = tmp = 0;
    printText(198, 30, "Sensors");
    while(i < MAXSENSORS) { //while list not empty
      if(EEPROM.read(SENSORS + i * SENSORBLOCK) != 0) {
        readEEPROMBytes(nam, SENSORS + i * SENSORBLOCK, 12);
        centerText(240, 60 + tmp * 18, nam);
        tmp++;
      }
      i++;
    }
    do {
      do {
        p = ts.getPoint();
        //Check for Android Connection
      } while (p.z < MINPRESSURE || p.z > MAXPRESSURE);

      if (p.x > 798 && p.y < 318) {  return;  } //Back
      else if (p.y > 535) {
        i = (697 - p.x) / 52; //Finds which sensor was pressed
        if(i < tmp) {
          count = tmp = 0;
          temp = sensorBytes();
          while(tmp < MAXSENSORS) {
            if(bitRead(temp,tmp) != 0){
              count++;
              if(i == count - 1) {  
                Serial.print("Sensor #");
                Serial.println(i);
                sensorEdit(tmp);
                tmp = MAXSENSORS;
              }
            }
            tmp++;
          }
        }
        else {  p.x = 800;  }
      }
    } while (p.x > 798);
  } while (1);
}

void accessPointSetup() {
  Serial.println("Access Point Setup");
}

//WiFi Settings
void wifiSettings() {
  makeTitle("WiFi Settings");
  tft.drawFastHLine(0, 200, 320, ILI9341_WHITE);

  readEEPROMBytes(ssid,0,16);
  printText(4, 80, "SSID:");
  printText(76, 80, ssid);
  printText(4, 160, "Connected:");
  printText(136, 160, "Yes");
  printText(52, 215, "Access Point Setup");

  do {
    do {
      p = ts.getPoint();
      //Check for Android Connection
    } while (p.z < MINPRESSURE || p.z > MAXPRESSURE);

    if (p.x > 798 && p.y < 318) {
      return;   //Back
    }
    else if (p.y < 295) {
      accessPointSetup();
    }
  } while (1);
}

// Default actions
void defaults(byte action) {
  if (action == 0) { //Default
    Serial.println("Restore Defaults");
  }
  else if (action == 1) { //Write
    Serial.println("Write Defaults");
  }
  else { //Read
    Serial.println("View Defaults");
  }
}

//Settings Screen
void settings() {
  do {
    makeTitle("Device Settings");
    tft.drawFastHLine(0, 78, 320, ILI9341_WHITE);
    tft.drawFastHLine(0, 131, 320, ILI9341_WHITE);
    tft.drawFastHLine(0, 184, 320, ILI9341_WHITE);

    centerText(160, 43, "Profile Settings");
    centerText(160, 96, "Sensor Settings");
    centerText(160, 149, "WiFi Settings");
    centerText(160, 202, "Defaults");

    do {
      do {
        p = ts.getPoint();
        //Check for Android Connection
      } while (p.z < MINPRESSURE || p.z > MAXPRESSURE);

      if (p.x > 798) {
        if (p.y < 318) {
          return;   //Back
        }
      }
      else {
        if (p.x > 645) {
          profileSettings();   //Profile Settings
        }
        else if (p.x > 493) {
          sensorSettings();   //Sensor Settings
        }
        else if (p.x > 252) {
          wifiSettings();   //WiFi Settings
        }
        else {  defaults(2);  }//Default Settings
      }
    } while (p.x > 798 && p.y > 318);
  } while (1);
}

//Date/Time Setting Screen
void dateSettings() {
  Serial.print("Date/Time\n");
}

//Obtain newest Weather info
void updateWeather() {
  //grab latest weather info
  Serial.print("Weather\n");
}

//Change Desired Temperature Screen
void changeTemp() {
  byte temp = EEPROM.read(HVAC + 1);
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

  do {
    tft.setCursor(105, 80);
    tft.println(temp);
    do {
      p = ts.getPoint();
      //Check for Android Connection
    } while (p.z < MINPRESSURE || p.z > MAXPRESSURE);

    //Cycle temp with arrow presses
    if (p.x > 798 && p.y < 318) {  return;  }
    if (p.x < 798 && p.x > 266) {
      if (p.x > 697 || p.x < 381) {
        if(p.x > 697) {
          if (temp < MAXTEMP) {  temp++;  }
        }
        else {
          if (temp > MINTEMP) {  temp--;  }
        }
      tft.fillRect(70, 80, 170, 70, ILI9341_BLACK);
      }
    }
    else if (p.x < 266) {
      EEPROM.write(HVAC + 1, temp);
      return;
    }
  }while(1);
}

//HVAC Settings Screen
byte hvacSettingChange(byte hvac) {
  makeTitle("HVAC Settings");
  tft.drawFastHLine(0, 107, 320, ILI9341_WHITE);

  //Fan Area
  printText(146, 35, "Fan");
  printText(61, 70, "On");
  printText(146, 70, "Off");
  printText(216, 70, "Auto");

  if (bitRead(hvac, 0)) {  tft.drawRect(56, 65, 34, 26, ILI9341_WHITE);  } //On
  else if (bitRead(hvac, 1)) {  tft.drawRect(141, 65, 46, 26, ILI9341_WHITE);  } //Off
  else {  tft.drawRect(211, 65, 58, 26, ILI9341_WHITE);  } //Auto

  //System Area
  printText(124, 117, "System");
  printText(51, 152, "Heat");
  printText(136, 152, "Cool");
  printText(204, 152, "Blower");

  if (bitRead(hvac, 2)) {  tft.drawRect(46, 147, 58, 26, ILI9341_WHITE);  } //Heat
  else if (bitRead(hvac, 3)) {  tft.drawRect(131, 147, 58, 26, ILI9341_WHITE);  } //Cool
  else {  tft.drawRect(199, 147, 82, 26, ILI9341_WHITE);  } //Blower

  do {
    do {
      p = ts.getPoint();
      //Check for Android Connection
    } while (p.z < MINPRESSURE || p.z > MAXPRESSURE);

    if (p.x > 562) {
      if (p.x > 798 && p.y < 318) {
        if(hvac != EEPROM.read(HVAC)) {  EEPROM.write(HVAC,hvac);  }
        return hvac;
      } //Back
      else if (p.x < 723) {
        tft.drawRect(56, 65, 34, 26, ILI9341_BLACK);
        tft.drawRect(141, 65, 46, 26, ILI9341_BLACK);
        tft.drawRect(211, 65, 58, 26, ILI9341_BLACK);
        if (p.y < 405) {
          tft.drawRect(56, 65, 34, 26, ILI9341_WHITE);
          bitSet(hvac, 0);
          bitClear(hvac, 1);
        }
        else if (p.y < 660) {
          tft.drawRect(141, 65, 46, 26, ILI9341_WHITE);
          bitSet(hvac, 1);
          bitClear(hvac, 0);
        }
        else {  tft.drawRect(211, 65, 58, 26, ILI9341_WHITE);
        bitClear(hvac, 0);
        bitClear(hvac, 1);
        }
      }
    }
    else if (p.x < 487) {
      tft.drawRect(46, 147, 58, 26, ILI9341_BLACK);
      tft.drawRect(131, 147, 58, 26, ILI9341_BLACK);
      tft.drawRect(199, 147, 82, 26, ILI9341_BLACK);
      if (p.y < 405) {
        tft.drawRect(46, 147, 58, 26, ILI9341_WHITE);
        bitSet(hvac, 2);
        bitClear(hvac, 3);
      }
      else if (p.y < 660) {
        tft.drawRect(131, 147, 58, 26, ILI9341_WHITE);
        bitSet(hvac, 3);
        bitClear(hvac, 2);
      }
      else {
        tft.drawRect(199, 147, 82, 26, ILI9341_WHITE);
        bitClear(hvac, 2);
        bitClear(hvac, 3);
      }
    }
  } while (1);
}

//Rotates Sensor List
void cycleSensorList(byte sn) {
  byte i;
  tft.fillRect(16, 215, 253, 20, ILI9341_BLACK);
  readEEPROMBytes(nam, SENSORS + SENSORBLOCK * sn, 13); //Read Sensor name from EEPROM
  i = cal(nam);
  printText(30, 219, nam);
  printText(30 + i * 12, 219, ":");
  unitPos(42 + i * 12, 219, EEPROM.read(SENSORS + SENSORBLOCK * sn + 25));
  delay(100);
}

void doubleLine(int xCoord, byte yCoord, int width, uint16_t color) {
  tft.drawFastHLine(xCoord, yCoord, width, color);
  tft.drawFastHLine(xCoord, yCoord + 1, width, color);
}

void onScreenPrompt() {

}
/* Possible Future Use
void displayList(int xCenter, byte yCoord, char *array){
  byte tmp = 0;
  while(tmp < 8) {  //while list not empty
    centerText(xCenter, yCoord + tmp * 18, array);
    tmp++;
  }
}*/

void addSensor(char *name, char *id, byte temp, byte sa) {
  byte i = 0;
  do{
    if(EEPROM.read(SENSORS + i * SENSORBLOCK) == '\0') {
      writeEEPROMBytes(name, SENSORS + i * SENSORBLOCK, 13);
      writeEEPROMBytes(id, SENSORS + i * SENSORBLOCK + 13, 11);
      EEPROM.write(SENSORS + i * SENSORBLOCK + 24, i);
      EEPROM.write(SENSORS + i * SENSORBLOCK + 25, temp);
      EEPROM.write(SENSORS + i * SENSORBLOCK + 26, sa);
      Serial.print("Adding Sensor #");
      Serial.println(i);
      return;
    }
    i++;
  }while(i < MAXSENSORS);
  Serial.println("Maximum Sensors Reached");
}

void removeSensor(byte sensorNumber) {
  if(sensorNumber == 0){
    Serial.println("ERROR: Cannot remove thermostat's sensor");
    return;  
  }
  EEPROM.write(SENSORS + sensorNumber * SENSORBLOCK, 0);
  Serial.print("Removing Sensor #");
  Serial.println(sensorNumber);
}
