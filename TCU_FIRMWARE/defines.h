#define CODE_VERSION "0.01"
#define DEBUG 1
#define AREF 3.3
#define HAS_LCD 0
#define RUN_WIFI 0

#define TS_MINX 180
#define TS_MINY 150
#define TS_MAXX 870
#define TS_MAXY 920

#define MINPRESSURE 10
#define MAXPRESSURE 1000

#define WEATHER 97 //Weather Forecast High
                   //Weather Forecast Low WEATHER + 1
                   //Weather Current Temperature WEATHER + 1

#define MAXTEMP 99 //Maximum temperature thermostat can set
#define MINTEMP 50 //Minimum temperature thermostat can set
#define HVAC 100 //HVAC Settings Address Location
                 //HVAC + 1 will be Desired Temperature
                 
#define SENSORS 700 //Sensor List Address Location
#define MAXSENSORS 9 //Maximum number of sensors allowed
#define SENSORBLOCK 27 //Bytes required to save 1 sensors info

#define PROFILES 102 //Profile List Address Location
#define MAXPROFILES 5 //Maximum number of profiles allowed
#define PROFILEBLOCK 57 //Unknown at the moment
#define MAXRULES 4 //Maximum number of rules allowed per profile
#define RULEBLOCK 6 //Bytes required for each profile rule

#define FOREGROUND_COLOR 0x0000      /*   0,   0,   0 */
#define BACKGROUND_COLOR 0xFFFF      /*   255,   255,   255 */

TSPoint p;
char ssid[16];
char password[64];
char nam[21];
int hvac, sn;
bool is_connected = 0;
//eeprom address space
//0x00-0x1F 32 byte ssid
//0x20-0x5F 64 byte password
//PROFILE STRUCTURE
//0x60 Active Profile


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
  for(i;i<(start+length);i++) {  EEPROM.write(i, 0);  }
}

bool restoreWifiData(){
  readEEPROMBytes(ssid,0,16);
  readEEPROMBytes(password,16,64);
  return 0;
}

bool updateWifiData(){
  writeEEPROMBytes(ssid,0,16);
  writeEEPROMBytes(password,16,64);
  return 0;
}

bool restoreProfileData(){
  //number_profiles = EEPROM.read();
  
}

bool restoreUserData(){
  readEEPROMBytes(ssid,0,16);
  readEEPROMBytes(password,16,64);

  return 0;
}



void androidConnect() {
  
}
