#include "pin_definitions.h"
#include <SPI.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>
#include <TouchScreen.h>

void setup(void)
{
  //open up serial comms
  //be careful with this, as this channel also connects to ESP8266
  Serial.begin(9600);
  Serial.println("Mr Thermostat");


}

void loop(void)
{

}

