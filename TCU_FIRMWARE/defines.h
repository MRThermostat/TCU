#define CODE_VERSION "0.01"
#define DEBUG
#define AREF 3.3
#define HAS_LCD 1

#define TS_MINX 180
#define TS_MINY 150
#define TS_MAXX 870
#define TS_MAXY 920

#define MINPRESSURE 10
#define MAXPRESSURE 1000

#define MAXTEMP 99 //Maximum temperature thermostat can set
#define MINTEMP 50 //Minimum temperature thermostat can set
#define HVAC 200 //HVAC Settings Address Location
                 //HVAC + 1 will be Desired Temperature
#define SENSORS 500 //Sensor List Address Location
#define MAXSENSORS 9 //Maximum number of sensors allowed
#define SENSORBLOCK 27 //Bytes required to save 1 sensors info

TSPoint p;
char ssid[16];
char password[64];
int hvac, sn;
char nam[13];
//eeprom address space
//0x00-0x1F 32 byte ssid
//0x20-0x5F 64 byte password
//0x60 number of profiles
//0x61 active profile
//PROFILE STRUCTURE
//16 byte name
//other goodies
//1 byte pointer to next rule

void readEEPROMBytes(char *buffer, int start, int length){
  int i;
  for(i=0;i<length;i++){
    if(EEPROM.read(i+start) == '\0') {  break;  }
    buffer[i]=EEPROM.read(i+start);
  }
  buffer[i] = '\0';
}

void writeEEPROMBytes(char *buffer, int start, int length){
  int i;
  for(i=start;i<(start+length);i++){
    if(buffer[i-start] == '\0') {  break;  }
    EEPROM.write(i, buffer[i-start]-0);
  }
  EEPROM.write(i, 0);
}

boolean restoreUserData(){
  readEEPROMBytes(ssid,0,16);
  readEEPROMBytes(password,16,64);
  
  return 0;
}
