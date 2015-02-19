#include "defines.h"
#include "pin_definitions.h"
#include <SPI.h>
#include "temperature.h"
//#include <Adafruit_ILI9341.h>
//#include <Adafruit_GFX.h>
//#include <TouchScreen.h>

void setup(void)
{
  //open up serial comms
  //be careful with this, as this channel also connects to ESP8266
  Serial.begin(115200);
  #ifdef DEBUG
  delay(1000);
  while(!Serial){
    ; 
  }
  #endif
  
  
  Serial.print("Mr Thermostat code version ");
  Serial.println(CODE_VERSION);
  
  Serial.println("beginning setup function");

  //pinMode(TEMP_PIN,INPUT);
  //analogReference(DEFAULT);
}

void loop(void)
{  
  Serial.print("Temperature in voltage: ");
  Serial.println(getTemp(VOLTAGE));
  delay(100);
}



