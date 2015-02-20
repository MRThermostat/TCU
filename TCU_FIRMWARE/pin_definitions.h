//touchscreen pins
#define YP A1
#define XM A2
#define YM 4
#define XP 5

//lcd control pins
#define TFT_CS 2
#define TFT_DC 3
#define TFT_RESET 9
#define TFT_BACKLIGHT 11

//nrf24l01 control pins
#define NRF_CE A4
#define NRF_CSN A5

//esp8266 control pins
#define ESP_GPIO0 12
#define ESP_GPIO2 13
#define ESP_RESET A3
#define ESP_CH_PD 17 //should be 14 according to picmicro schematic, 17 according to arduinio/variants/micro/pins_arduino.h

//temperature reading
#define TEMP_PIN A0

//relay control pins
#define RELAY1 6
#define RELAY2 7
#define RELAY3 8
