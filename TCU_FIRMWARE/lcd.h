#define DISPLAY_POWER_MODE 1
#define MADCTL_MODE 1
#define PIXEL_FORMAT 1
#define IMAGE_FORMAT 1
#define SELF_DIAGNOSTIC 1
#define FOREGROUND_COLOR       0x0000      /*   0,   0,   0 */
#define BACKGROUND_COLOR       0xFFFF      /*   255,   255,   255 */

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RESET);

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

void printText(int xCoord, byte yCoord, char *array){
  tft.setCursor(xCoord, yCoord);
  tft.println(array);
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

void makeTitle(char *title){
  tft.fillScreen(BACKGROUND_COLOR);
  tft.setTextColor(FOREGROUND_COLOR);
  tft.setTextSize(2);

  tft.drawFastVLine(70, 0, 25, FOREGROUND_COLOR);
  tft.drawFastHLine(0, 25, 320, FOREGROUND_COLOR);

  printText(4, 5, "Back");
  centerText(195, 5, title);
}

//Change Active Profile Screen
void changeActive(){
  Serial.println("Active");
}

//Profile Settings Screen
void profileSettings(){
  TSPoint p;
  char nam[21] = "Active Profile123456";
  do{
    byte tmp = 0;
    makeTitle("Profile Settings");
    tft.drawFastHLine(0, 65, 320, FOREGROUND_COLOR);
    tft.drawFastHLine(0, 215, 320, FOREGROUND_COLOR);
    
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
  /*if(sensorActive) {  tft.drawRect(95, 85, 46, 26, FOREGROUND_COLOR);  } //Box around Yes
  else {  tft.drawRect(155, 85, 34, 26, FOREGROUND_COLOR);  } //Box around No*/
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
          tft.drawRect(95, 85, 46, 26, FOREGROUND_COLOR);
          tft.drawRect(155, 85, 34, 26, BACKGROUND_COLOR);
        }
        else if(p.y < 593){
          tft.drawRect(95, 85, 46, 26, BACKGROUND_COLOR);
          tft.drawRect(155, 85, 34, 26, FOREGROUND_COLOR);
        }
      } 
    }
  }while(true);
}

//Sensor Settings Screen
void sensorSettings(){
  TSPoint p;
  char nam[13] = "Sensor Name1";
  do{
    byte tmp = 0;
    makeTitle("Sensor Settings");
    tft.drawFastVLine(160, 25, 215, FOREGROUND_COLOR);
    
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

void accessPointSetup() {
  Serial.println("Access Point Setup");  
}

//WiFi Settings
void wifiSettings(){
  TSPoint p;  
  makeTitle("WiFi Settings");
  tft.drawFastHLine(0, 200, 320, FOREGROUND_COLOR);

  printText(4, 130, "SSID:");
  printText(136, 130, ssid);
  printText(4, 155, "Password:");
  printText(136, 155, password);
  printText(4, 180, "Connected:");
  if(is_connected){
    printText(136, 180, "Yes");
  }else{
    printText(136, 180, "No");
  }
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

//Settings Screen
void settings(){
  TSPoint p;
  
  do{  
    makeTitle("Device Settings");    
    tft.drawFastHLine(0, 78, 320, FOREGROUND_COLOR);
    tft.drawFastHLine(0, 131, 320, FOREGROUND_COLOR);
    tft.drawFastHLine(0, 184, 320, FOREGROUND_COLOR);
    
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
  tft.drawFastHLine(0, 210, 320, FOREGROUND_COLOR);
  tft.fillTriangle(                 // Up Arrow
                   160, 30,         // peak
                   100, 60,         // bottom left
                   220, 60,         // bottom right
                   FOREGROUND_COLOR);
  tft.fillTriangle(                 // DownArrow
                   160, 200,        // peak
                   100, 170,        // bottom left
                   220, 170,        // bottom right
                   FOREGROUND_COLOR);

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
    tft.fillRect(70, 80, 170, 70, BACKGROUND_COLOR);
    if(temp > 99) {  tft.setCursor(70, 80);  }
    else {  tft.setCursor(105, 80);  }
    tft.println(temp);
  }while(true);
}

//HVAC Settings Screen
void hvacSettingChange(){
  TSPoint p;  
  makeTitle("HVAC Settings");
  tft.drawFastHLine(0, 107, 320, FOREGROUND_COLOR);
  
  //Fan Area
  printText(146, 35, "Fan");
  printText(61, 70, "On");
  printText(146, 70, "Off");
  printText(216, 70, "Auto");
  /*
  if(bitRead(hvac,0)) {  tft.drawRect(56, 65, 34, 26, BACKGROUND_COLOR);  } //On
  else if(bitRead(hvac,1)) {  tft.drawRect(141, 65, 46, 26, BACKGROUND_COLOR);  } //Off
  else {  tft.drawRect(211, 65, 58, 26, BACKGROUND_COLOR);  } //Auto
  */
  //System Area
  printText(124, 117, "System");
  printText(51, 152, "Heat");
  printText(136, 152, "Cool");
  printText(204, 152, "Blower");
  /*
  if(bitRead(hvac,2)) {  tft.drawRect(46, 147, 58, 26, BACKGROUND_COLOR);  } //Heat
  else if(bitRead(hvac,3)) {  tft.drawRect(131, 147, 58, 26, BACKGROUND_COLOR);  } //Cool
  else {  tft.drawRect(199, 147, 82, 26, BACKGROUND_COLOR);  } //Blower
  */
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
        tft.drawRect(56, 65, 34, 26, BACKGROUND_COLOR);
        tft.drawRect(141, 65, 46, 26, BACKGROUND_COLOR);
        tft.drawRect(211, 65, 58, 26, BACKGROUND_COLOR);
        if(p.y < 405) {
          tft.drawRect(56, 65, 34, 26, FOREGROUND_COLOR);
          //bitWrite(hvac, 0, bitRead(hvac, 0) ^ 1);
        }
        else if(p.y < 660) {
          tft.drawRect(141, 65, 46, 26, FOREGROUND_COLOR);
          //bitWrite(hvac, 1, bitRead(hvac, 1) ^ 1);
        }
        else {  tft.drawRect(211, 65, 58, 26, FOREGROUND_COLOR);  }
      }
    }
    else if(p.x < 487){
      tft.drawRect(46, 147, 58, 26, BACKGROUND_COLOR);
      tft.drawRect(131, 147, 58, 26, BACKGROUND_COLOR);
      tft.drawRect(199, 147, 82, 26, BACKGROUND_COLOR);
      if(p.y < 405) {
        tft.drawRect(46, 147, 58, 26, FOREGROUND_COLOR);
        //bitWrite(hvac, 2, bitRead(hvac, 2) ^ 1);
      }
      else if(p.y < 660) {
        tft.drawRect(131, 147, 58, 26, FOREGROUND_COLOR);
        //bitWrite(hvac, 3, bitRead(hvac, 3) ^ 1);
      }
      else {  tft.drawRect(199, 147, 82, 26, FOREGROUND_COLOR);  } 
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

//Profile Edit Screen
void editProfile(){
  Serial.println("Edit");
}

void doubleLine(int xCoord, byte yCoord, int width, uint16_t color){
  tft.drawFastHLine(xCoord, yCoord, width, color);
  tft.drawFastHLine(xCoord, yCoord + 1, width, color);
}

void onScreenPrompt(){
  
}
/* Possible Future Use
void displayList(int xCenter, byte yCoord, char *array){
  byte tmp = 0;
  while(tmp < 8) {  //while list not empty
    centerText(xCenter, yCoord + tmp * 18, array);
    tmp++;
  }
}*/
