//21 byte name
//Some sort of ID
//1 byte # of rules
//6 bytes per rule
//  1 rule number
//  1 type of rule
//  4 rule parameters

void addProfile(char *name, char *id, byte type, unsigned int params) {
  byte i = 0;
  byte bits;
  byte temp = params & 0xFF;
  do{
    if(EEPROM.read(PROFILES + PROFILEBLOCK * i) == '\0') {
      writeEEPROMBytes(name, PROFILES + PROFILEBLOCK * i, 16);
      writeEEPROMBytes(id, PROFILES + PROFILEBLOCK * i + 16, 11);
      EEPROM.write(PROFILES + PROFILEBLOCK * i + 27, 1);
      EEPROM.write(PROFILES + PROFILEBLOCK * i + 28, 1);
      EEPROM.write(PROFILES + PROFILEBLOCK * i + 29, type);
      EEPROM.write(PROFILES + PROFILEBLOCK * i + 31, temp); //Second part of params, meaning 30 from 12:30
      for(bits = 0; bits < 8; bits++) {  bitWrite(temp, bits, bitRead(params, 8 + bits));  }
      EEPROM.write(PROFILES + PROFILEBLOCK * i + 30, temp); //First part of params, meaning 12 from 12:30
      Serial.print("Adding Profile #");
      Serial.println(i);
      return;
    }
    i++;
  }while(i < MAXPROFILES);
  Serial.println("Maximum Profiles Reached");
}

void removeProfile(byte pn) {
  EEPROM.write(PROFILES + PROFILEBLOCK * pn, 0);
  Serial.print("Removing Profile #");
  Serial.println(pn);
}

void editProfile(byte pn, char *name, bool active) {
  byte temp = EEPROM.read(PROFILES + PROFILEBLOCK * pn + 26);
  if(name[0]) {
    writeEEPROMBytes(name, PROFILES + PROFILEBLOCK * pn, 13);
    Serial.println("Profile Name changed");
  }
  if(active != bitRead(temp , 7)) {
    temp = temp ^ 0x80;
    EEPROM.write(PROFILES + PROFILEBLOCK * pn + 26, temp);
    Serial.println("Profile Active state changed");
  }
}

void editRule(byte pn, char *name, byte type, int params) {
  byte bits;
  byte temp = params & 0xFF;
  if(type) {  EEPROM.write(PROFILES + PROFILEBLOCK * i + 29, type);  }
  if(params) {
    EEPROM.write(PROFILES + PROFILEBLOCK * i + 31, temp); //Second part of params, meaning 30 from 12:30
    for(bits = 0; bits < 8; bits++) {  bitWrite(temp, bits, bitRead(params, 8 + bits));  }
    EEPROM.write(PROFILES + PROFILEBLOCK * i + 30, temp); //First part of params, meaning 12 from 12:30
  }
}

int profileBytes() { //Returns an int that, if displayed in binary will have a 1 for active and 0 for not active
  byte i;
  int profile = 0;
  for(i = 0; i < MAXPROFILES; i++) {
    if(EEPROM.read(PROFILES + PROFILEBLOCK * i) != 0){  bitSet(profile,i);  }
  }
  return profile;
}
