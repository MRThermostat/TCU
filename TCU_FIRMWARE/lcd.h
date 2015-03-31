#define DISPLAY_POWER_MODE 1
#define MADCTL_MODE 1
#define PIXEL_FORMAT 1
#define IMAGE_FORMAT 1
#define SELF_DIAGNOSTIC 1

void setupLCD(){
  //setup backlight
  pinMode(TFT_BACKLIGHT, OUTPUT);
  analogWrite(TFT_BACKLIGHT,255);
  
  //initialize lcd
  tft.begin();
  tft.setRotation(3);
}

bool readDiagnostics(){
  // read diagnostics (optional but can help debug problems)
  uint8_t x = tft.readcommand8(ILI9341_RDMODE);
  //Serial.print("Display Power Mode: 0x"); Serial.println(x, HEX);
  if(x == DISPLAY_POWER_MODE) 
  {
    x = tft.readcommand8(ILI9341_RDMADCTL);
    //Serial.print("MADCTL Mode: 0x"); Serial.println(x, HEX);
    if(x == MADCTL_MODE)
    {
      x = tft.readcommand8(ILI9341_RDPIXFMT);
      //Serial.print("Pixel Format: 0x"); Serial.println(x, HEX);
      if(x == PIXEL_FORMAT)
      {
        x = tft.readcommand8(ILI9341_RDIMGFMT);
        //Serial.print("Image Format: 0x"); Serial.println(x, HEX);
        if(x == IMAGE_FORMAT)
        {
          x = tft.readcommand8(ILI9341_RDSELFDIAG);
          //Serial.print("Self Diagnostic: 0x"); Serial.println(x, HEX); 
          if(x == SELF_DIAGNOSTIC)
          {
            return(1);
          }
        }
      }
    }
  }
  
  return(0);
}

void profileEdit(byte pn) {
  byte temp, i, j;
  makeTitle("Profile Settings");
  
  doubleHLine(0, 100, 320, FOREGROUND_COLOR);

  readEEPROMBytes(nam, PROFILES + pn * PROFILEBLOCK, 20); //Read Profile name from EEPROM
  printText(4, 30, "Name:");
  printText(62, 30, nam); //Grab sensor name
  
  readEEPROMBytes(nam, PROFILES + pn * PROFILEBLOCK + 21, 11); //Read Profile id from EEPROM
  printText(4, 50, "Profile ID:");
  printText(142, 50, nam); //Grab sensor ID

  temp = findActiveProfile();
  printText(4, 75, "Active:");
  printText(100, 75, "Yes");
  printText(160, 75, "No");
  if(temp == pn) {  tft.drawRect(95, 70, 46, 26, FOREGROUND_COLOR);  } //Box around Yes
  else {  tft.drawRect(155, 70, 34, 26, FOREGROUND_COLOR);  } //Box around No

  temp = 0;
  centerText(160, 103, "Rules");
  for(i = 0; i < MAXRULES; i++) {
    Serial.print("Rule #");
    Serial.print(i);
    if(EEPROM.read(PROFILES + pn * PROFILEBLOCK + 34 + i * RULEBLOCK + 1) == 1) {
      printText(4, 125 + 18 * temp, "Time|");
      unitPos(65, 125 + 18 * temp, EEPROM.read(PROFILES + pn * PROFILEBLOCK + 34 + i * RULEBLOCK));
      tft.setCursor(124, 125 + 18 * temp);
      tft.print(EEPROM.read(PROFILES + pn * PROFILEBLOCK + 34 + i * RULEBLOCK + 2));
      tft.print(":");
      tft.print(EEPROM.read(PROFILES + pn * PROFILEBLOCK + 34 + i * RULEBLOCK + 3));
      tft.print(" To ");
      tft.print(EEPROM.read(PROFILES + pn * PROFILEBLOCK + 34 + i * RULEBLOCK + 4));
      tft.print(":");
      tft.print(EEPROM.read(PROFILES + pn * PROFILEBLOCK + 34 + i * RULEBLOCK + 5));
      temp++;
      Serial.println(" Time");
    }
    else if(EEPROM.read(PROFILES + pn * PROFILEBLOCK + 34 + i * RULEBLOCK + 1) == 2) {
      printText(4, 125 + 18 * temp, "Temp|");
      unitPos(65, 125 + 18 * temp, EEPROM.read(PROFILES + pn * PROFILEBLOCK + 34 + i * RULEBLOCK));
      tft.setCursor(124, 125 + 18 * temp);
      tft.print("If ");
      if(EEPROM.read(PROFILES + pn * PROFILEBLOCK + 34 + i * RULEBLOCK + 3) == 0) {  tft.print("<");  }
      else {  tft.print(">");  }
      unitPos(184, 125 + 18 * temp, EEPROM.read(PROFILES + pn * PROFILEBLOCK + 34 + i * RULEBLOCK + 5));
      temp++;
      Serial.println(" Temp");
    }
    else if(EEPROM.read(PROFILES + pn * PROFILEBLOCK + 34 + i * RULEBLOCK + 1) == 3) {
      printText(4, 125 + 18 * temp, "Prox|");
      unitPos(65, 125 + 18 * temp, EEPROM.read(PROFILES + pn * PROFILEBLOCK + 34 + i * RULEBLOCK));
      tft.setCursor(124, 125 + 18 * temp);
      for(j = 0; j < 4; j++) {
        tft.print(EEPROM.read(PROFILES + pn * PROFILEBLOCK + 34 + i * RULEBLOCK + 2 + j)); //IP to check for
        if(j < 3) {  tft.print(".");  }
      }
      temp++;
      Serial.println(" Prox");
    }
    else {  Serial.println(" Does not exist");  }
  }
  temp = EEPROM.read(PROFILES + pn * PROFILEBLOCK + 32); //Read Profile status and active from EEPROM
  do {
    do {
      p = ts.getPoint();
      //Check for Android Connection
    } while (p.z < MINPRESSURE || p.z > MAXPRESSURE);
    if (p.x > 717) {
      if (p.y < 318) {
        if(bitRead(temp, 7) != bitRead(EEPROM.read(PROFILES + pn * PROFILEBLOCK + 33),7)) {
          for(i = 0; i < MAXPROFILES; i++) {  EEPROM.write(PROFILES + i * PROFILEBLOCK + 32, EEPROM.read(PROFILES + i * PROFILEBLOCK + 32) & 01111111);  }
          EEPROM.write(PROFILES + pn * PROFILEBLOCK + 32, temp);
        }
        return;   //Back
      }
    }
    else {
      if (p.x > 545 && p.x < 631) {
          tft.drawRect(155, 70, 34, 26, BACKGROUND_COLOR);
          tft.drawRect(95, 70, 46, 26, BACKGROUND_COLOR);
        if (p.y > 361 && p.y < 506) {
          tft.drawRect(95, 70, 46, 26, FOREGROUND_COLOR);
          bitSet(temp, 7);
        }
        else if (p.y < 593) {
          tft.drawRect(155, 70, 34, 26, FOREGROUND_COLOR);
          bitClear(temp, 7);
        }
      }
    }
  } while (1);
}

//Profile Settings Screen
void profileSettings(){
  TSPoint p;
  byte tmp, i, count;
  int temp = 0;
  do{
    tmp = 0;
    makeTitle("Profile Settings");
    doubleHLine(0, 65, 320, FOREGROUND_COLOR);
    
    centerText(160, 28, "Currently Active");
    i = findActiveProfile();
    if(i != MAXPROFILES) {
      readEEPROMBytes(nam, PROFILES + i * PROFILEBLOCK, 20);
      centerText(160, 46, nam);
    }
    else {  centerText(160, 46, "No Profiles Active");  }
    i = 0;
    //Profile List
    while (i < MAXPROFILES) { //while list not empty
      if(EEPROM.read(PROFILES + i * PROFILEBLOCK) != 0) {
        readEEPROMBytes(nam, PROFILES + i * PROFILEBLOCK, 20);
        centerText(160, 70 + tmp * 18, nam);
        tmp++;
      }
      i++;
    }
    do{
      do{
        p = ts.getPoint();
        //Check for Android Connection
      }while(p.z < MINPRESSURE || p.z > MAXPRESSURE);
    
      if(p.x > 798 && p.y < 318) {  return;  } //Back
      else if (p.x < 684) {
        i = (668 - p.x) / 52; //Finds which profile was pressed
          if(i < tmp) {
          count = tmp = 0;
          temp = profileBytes();
          while(tmp < MAXPROFILES) {
            if(bitRead(temp,tmp) != 0){
              count++;
              if(i == count - 1) {  
                Serial.print("Profile #");
                Serial.println(i);
                profileEdit(tmp);
                tmp = MAXPROFILES;
              }
            }
            tmp++;
          }
        }
        else {  p.x = 800;  }
      }
      else if(p.x > 684) {  p.x = 800;  }
    }while(p.x > 798);
  }while(true);
}

void sensorEdit(byte sn) {
  byte temp;
  makeTitle("Sensor Settings");

  readEEPROMBytes(nam, SENSORS + sn * SENSORBLOCK, 13); //Read Sensor name from EEPROM
  printText(4, 30, "Name:");
  printText(62, 30, nam); //Grab sensor name
  temp = EEPROM.read(SENSORS + sn * SENSORBLOCK + 25); //Read Sensor temp from EEPROM
  printText(4, 60, "Latest Temperature:");
  unitPos(244 , 60, temp); //Grab latest Temperature
  
  readEEPROMBytes(nam, SENSORS + sn * SENSORBLOCK + 13, 11); //Read Sensor id from EEPROM
  printText(4, 120, "Sensor ID:");
  printText(130, 120, nam); //Grab sensor ID

  temp = EEPROM.read(SENSORS + sn * SENSORBLOCK + 26); //Read Sensor status and active from EEPROM
  printText(4, 90, "Active:");
  printText(100, 90, "Yes");
  printText(160, 90, "No");
  if(bitRead(temp, 7)) {  tft.drawRect(95, 85, 46, 26, FOREGROUND_COLOR);  } //Box around Yes
  else {  tft.drawRect(155, 85, 34, 26, FOREGROUND_COLOR);  } //Box around No

  printText(4, 150, "Battery Status:");
  printText(184, 150, "Replace"); //Grab sensor battery status

  do {
    do {
      p = ts.getPoint();
      //Check for Android Connection
    } while (p.z < MINPRESSURE || p.z > MAXPRESSURE);
    if (p.x > 717) {
      if (p.y < 318) {
        if(bitRead(temp, 7) != bitRead(EEPROM.read(SENSORS + sn * SENSORBLOCK + 26),7)) {  EEPROM.write(SENSORS + sn * SENSORBLOCK + 26,temp);  }
        return;   //Back
      }
    }
    else { //go to sensor list and change active value
      if (p.x > 545 && p.x < 631) {
          tft.drawRect(155, 85, 34, 26, BACKGROUND_COLOR);
          tft.drawRect(95, 85, 46, 26, BACKGROUND_COLOR);
        if (p.y > 361 && p.y < 506) {
          tft.drawRect(95, 85, 46, 26, FOREGROUND_COLOR);
          bitSet(temp, 7);
        }
        else if (p.y < 593) {
          tft.drawRect(155, 85, 34, 26, FOREGROUND_COLOR);
          temp = EEPROM.read(SENSORS + sn * SENSORBLOCK + 26); //Read Sensor status and active from EEPROM
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
    tft.drawFastVLine(160, 25, 215, FOREGROUND_COLOR);

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
      else if (p.y < 535) {  p.x = 800;  }
    } while (p.x > 798);
  } while (1);
}

void accessPointSetup() {
  Serial.println("Access Point Setup");
}

//WiFi Setup
void setupNetwork(){
  TSPoint p;  
  makeTitle("WiFi Setup");
  
                  //"12345678901234567890123456"
  printText(4, 29,  "1.Using your mobile device");
  printText(4, 54,  "  connect to MR THERMOSTAT");
  printText(4, 79,  "2.Open your browser then");
  printText(4, 104, "  navigate to:");
  char ipaddress[15];
  String stringobject = wifi.getLocalIP();
  stringobject.toCharArray(ipaddress,15);
  printText(50, 129, ipaddress);
  printText(4, 154, "3.Enter wifi credentials");
  printText(4, 179, "  then click submit");
 

    do{
      do{
        p = ts.getPoint();
        //Check for Android Connection
      }while(p.z < MINPRESSURE || p.z > MAXPRESSURE);

      if(p.x > 798 && p.y < 318) {  return;  } //Back
    }while(true);
}

//WiFi Settings
void wifiSettings(){
  do{
    TSPoint p;  
    makeTitle("WiFi Settings");
    doubleHLine(0, 200, 320, FOREGROUND_COLOR);
  
    printText(4, 29, "SSID:");
    printText(136, 29, ssid);
    printText(4, 54, "Password:");
    printText(136, 54, password);
    printText(4, 79, "Connected:");
    if(is_connected){
      printText(136, 79, "Yes");
      printText(4, 104, "IP Address:");
      char ipaddress[15];
      String stringobject = wifi.getLocalIP();
      stringobject.toCharArray(ipaddress,15);
      printText(136, 104, ipaddress);
      printText(4, 129, "Mac Address:");
      printText(136, 129, "");
    }else{
      printText(136, 79, "No");
    }
    printText(52, 215, "Access Point Setup");
  
      
    do{
      p = ts.getPoint();
      //Check for Android Connection
    }while(p.z < MINPRESSURE || p.z > MAXPRESSURE);

    if(p.x > 798 && p.y < 318) {  return;  } //Back
    else if(p.x < 295) {  setupNetwork();  }
  }while(true);
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
    tft.drawFastHLine(0, 78, 320, FOREGROUND_COLOR);
    tft.drawFastHLine(0, 131, 320, FOREGROUND_COLOR);
    tft.drawFastHLine(0, 184, 320, FOREGROUND_COLOR);

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
  tft.drawFastHLine(0, 210, 320, FOREGROUND_COLOR);
  tft.drawTriangle(                 // Up Arrow
    160, 30,         // peak
    100, 60,         // bottom left
    220, 60,         // bottom right
    FOREGROUND_COLOR);
  tft.drawTriangle(                 // DownArrow
    160, 200,        // peak
    100, 170,        // bottom left
    220, 170,        // bottom right
    FOREGROUND_COLOR);

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
      tft.fillRect(70, 80, 170, 70, BACKGROUND_COLOR);
      }
    }
    else if (p.x < 266) {
      EEPROM.write(HVAC + 1, temp);
      return;
    }
  }while(1);
}

//HVAC Settings Screen
void hvacSettingChange(byte hvac) {
  hvac = EEPROM.read(HVAC);
  makeTitle("HVAC Settings");
  tft.drawFastHLine(0, 107, 320, FOREGROUND_COLOR);

  //Fan Area
  printText(146, 35, "Fan");
  printText(61, 70, "On");
  printText(146, 70, "Off");
  printText(216, 70, "Auto");

  if (bitRead(hvac, 0)) {  tft.drawRect(56, 65, 34, 26, FOREGROUND_COLOR);  } //On
  else if (bitRead(hvac, 1)) {  tft.drawRect(141, 65, 46, 26, FOREGROUND_COLOR);  } //Off
  else {  tft.drawRect(211, 65, 58, 26, FOREGROUND_COLOR);  } //Auto

  //System Area
  printText(124, 117, "System");
  printText(51, 152, "Heat");
  printText(136, 152, "Cool");
  printText(204, 152, "Blower");

  if (bitRead(hvac, 2)) {  tft.drawRect(46, 147, 58, 26, FOREGROUND_COLOR);  } //Heat
  else if (bitRead(hvac, 3)) {  tft.drawRect(131, 147, 58, 26, FOREGROUND_COLOR);  } //Cool
  else {  tft.drawRect(199, 147, 82, 26, FOREGROUND_COLOR);  } //Blower

  do {
    do {
      p = ts.getPoint();
      //Check for Android Connection
    } while (p.z < MINPRESSURE || p.z > MAXPRESSURE);

    if (p.x > 562) {
      if (p.x > 798 && p.y < 318) {
        if(hvac != EEPROM.read(HVAC)) {  EEPROM.write(HVAC,hvac);  }
        return ;
      } //Back
      else if (p.x < 723) {
        tft.drawRect(56, 65, 34, 26, BACKGROUND_COLOR);
        tft.drawRect(141, 65, 46, 26, BACKGROUND_COLOR);
        tft.drawRect(211, 65, 58, 26, BACKGROUND_COLOR);
        if (p.y < 405) {
          tft.drawRect(56, 65, 34, 26, FOREGROUND_COLOR);
          bitSet(hvac, 0);
          bitClear(hvac, 1);
        }
        else if (p.y < 660) {
          tft.drawRect(141, 65, 46, 26, FOREGROUND_COLOR);
          bitSet(hvac, 1);
          bitClear(hvac, 0);
        }
        else {  tft.drawRect(211, 65, 58, 26, FOREGROUND_COLOR);
        bitClear(hvac, 0);
        bitClear(hvac, 1);
        }
      }
    }
    else if (p.x < 487) {
      tft.drawRect(46, 147, 58, 26, BACKGROUND_COLOR);
      tft.drawRect(131, 147, 58, 26, BACKGROUND_COLOR);
      tft.drawRect(199, 147, 82, 26, BACKGROUND_COLOR);
      if (p.y < 405) {
        tft.drawRect(46, 147, 58, 26, FOREGROUND_COLOR);
        bitSet(hvac, 2);
        bitClear(hvac, 3);
      }
      else if (p.y < 660) {
        tft.drawRect(131, 147, 58, 26, FOREGROUND_COLOR);
        bitSet(hvac, 3);
        bitClear(hvac, 2);
      }
      else {
        tft.drawRect(199, 147, 82, 26, FOREGROUND_COLOR);
        bitClear(hvac, 2);
        bitClear(hvac, 3);
      }
    }
  } while (1);
}

//Profile Edit Screen
void editProfile(){
  Serial.println("Edit");
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


