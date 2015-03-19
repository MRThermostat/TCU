Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RESET);

void doubleHLine(int xCoord, byte yCoord, int width, uint16_t color){
  tft.drawFastHLine(xCoord, yCoord, width, color);
  tft.drawFastHLine(xCoord, yCoord + 1, width, color);
}

void doubleVLine(int xCoord, byte yCoord, int width, uint16_t color){
  tft.drawFastVLine(xCoord, yCoord, width, color);
  tft.drawFastVLine(xCoord + 1, yCoord, width, color);
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

void makeTitle(char *title){
  tft.fillScreen(BACKGROUND_COLOR);
  tft.setTextColor(FOREGROUND_COLOR);
  tft.setTextSize(2);

  doubleVLine(70, 0, 25, FOREGROUND_COLOR);
  doubleHLine(0, 25, 320, FOREGROUND_COLOR);

  printText(4, 5, "Back");
  centerText(195, 5, title);
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
