#define CODE_VERSION "0.01"
#define DEBUG
#define AREF 3.3
#define HAS_LCD 0

#define TS_MINX 180
#define TS_MINY 150
#define TS_MAXX 870
#define TS_MAXY 920

#define MINPRESSURE 10
#define MAXPRESSURE 1000

char ssid[16];
char password[64];
//eeprom address space
//0x00-0x1F 32 byte ssid
//0x20-0x5F 64 byte password
//0x60 number of profiles
//0x61 active profile
//PROFILE STRUCTURE
//16 byte name
//other goodies
//1 byte pointer to next rule

int readEEPROMBytes(char *buffer, int start, int length){
  for(int i=start;i<(start+length);i++){
    *buffer=EEPROM.read(i);  
  }
}

bool restoreUserData(){
  readEEPROMBytes(ssid,0,16);
  readEEPROMBytes(password,16,64);
  
  return 0;
}

