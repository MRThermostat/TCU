#define DISPLAY_POWER_MODE 1
#define MADCTL_MODE 1
#define PIXEL_FORMAT 1
#define IMAGE_FORMAT 1
#define SELF_DIAGNOSTIC 1

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, MOSI, SCK, TFT_RESET, MISO);

void setupLCD(){
  //setup LCD pins
  pinMode(TFT_CS, OUTPUT);
  pinMode(TFT_DC, OUTPUT);
  pinMode(TFT_RESET, OUTPUT);
  pinMode(TFT_BACKLIGHT, OUTPUT);
  analogWrite(TFT_BACKLIGHT,255);
}

bool readDiagnostics(){
  // read diagnostics (optional but can help debug problems)
  uint8_t x = tft.readcommand8(ILI9341_RDMODE);
  //Serial.print("Display Power Mode: 0x"); Serial.println(x, HEX);
  if(x = DISPLAY_POWER_MODE)
  {
    x = tft.readcommand8(ILI9341_RDMADCTL);
    //Serial.print("MADCTL Mode: 0x"); Serial.println(x, HEX);
    if(x = MADCTL_MODE)
    {
      x = tft.readcommand8(ILI9341_RDPIXFMT);
      //Serial.print("Pixel Format: 0x"); Serial.println(x, HEX);
      if(x = PIXEL_FORMAT)
      {
        x = tft.readcommand8(ILI9341_RDIMGFMT);
        //Serial.print("Image Format: 0x"); Serial.println(x, HEX);
        if(x = IMAGE_FORMAT)
        {
          x = tft.readcommand8(ILI9341_RDSELFDIAG);
          //Serial.print("Self Diagnostic: 0x"); Serial.println(x, HEX); 
          if(x = SELF_DIAGNOSTIC)
          {
            return(1);
          }
        }
      }
    }
  }
  
  return(0);
}
