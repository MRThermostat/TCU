#define CODE_VERSION "0.01"
#define DEBUG 1
#define AREF 3.3
#define HAS_LCD 0

#define TS_MINX 180
#define TS_MINY 150
#define TS_MAXX 870
#define TS_MAXY 920

#define MINPRESSURE 10
#define MAXPRESSURE 1000

char ssid[32];
char password[64];
bool is_connected = 0;
//eeprom address space
//0x00-0x1F 32 byte ssid
//0x20-0x5F 64 byte password
//0x60 number of profiles
//0x61 active profile
//PROFILE STRUCTURE
//16 byte name
//number of rules
//time proximity weather
//1 byte pointer to next rule

int readEEPROMBytes(char* buffer, int start, int length){
  //int l = 0;
  for(int i=0;i<length;i++){
    *(buffer+i)=EEPROM.read(i+start); 
    //l = i;
    //todo check if character is null, set i to end of string address if(*(buffer+i) == "\0") i=start+length;
  }
  return 0;
}

int writeEEPROMBytes(char *buffer,int start){
  int i = 0;
  while(*(buffer+i) != '\0'){
    EEPROM.write(start+i,*(buffer+i));
    i++;
  }
  EEPROM.write(start+i,'\0');
  return i;
}

bool restoreWifiData(){
  readEEPROMBytes(ssid,0,32);
  readEEPROMBytes(password,32,64);
  return 0;
}

bool updateWifiData(){
  writeEEPROMBytes(ssid,0);
  writeEEPROMBytes(password,32);
  return 0;
}

bool restoreProfileData(){
  //number_profiles = EEPROM.read();
  
}

bool addProfile(){
  
}

bool restoreUserData(){
  restoreWifiData();
  restoreProfileData(); 
 
  return 0; 
}
