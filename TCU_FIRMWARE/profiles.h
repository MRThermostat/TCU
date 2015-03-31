//21 byte name
//11 byte id
//1 byte # of rules
//6 bytes per rule
//  1 rule number
//  1 type of rule
//  4 rule parameters

void ruleWrite(int pn, byte rn, byte temperature, byte type, unsigned int params1, unsigned int params2) {
  byte temp = params1 & 0xFF;
  byte bits;
  EEPROM.write(pn + 34 + rn, temperature); //Temperature to set
  EEPROM.write(pn + 34 + rn + 1, type); //Rule type
  EEPROM.write(pn + 34 + rn + 3, temp); //Second part of params, meaning 30 from 12:30
  for(bits = 0; bits < 8; bits++) {  bitWrite(temp, bits, bitRead(params1, 8 + bits));  }
  EEPROM.write(pn + 34 + rn + 2, temp); //First part of params, meaning 12 from 12:30
  temp = params2 & 0xFF;
  EEPROM.write(pn + 34 + rn + 5, temp); //Second part of params, meaning 30 from 12:30
  for(bits = 0; bits < 8; bits++) {  bitWrite(temp, bits, bitRead(params2, 8 + bits));  }
  EEPROM.write(pn + 34 + rn + 4, temp); //First part of params, meaning 12 from 12:30
}

int profileBytes() { //Returns an int that, if displayed in binary will have a 1 for existing and 0 for non-existent
  byte i;
  int profile = 0;
  for(i = 0; i < MAXPROFILES; i++) {
    if(EEPROM.read(PROFILES + PROFILEBLOCK * i) != 0){  bitSet(profile,i);  }
  }
  return profile;
}

boolean profileCheck(byte pn) {
  int profile = profileBytes();
  if(bitRead(profile, pn)) {  return 1;  }
  Serial.print("Profile #");
  Serial.print(pn);
  Serial.println(" does not exist");
  return 0;
}
boolean ruleCheck(byte pn, byte rn) {
  if(!profileCheck(pn)) {  return 1;  }
  if(EEPROM.read(PROFILES + PROFILEBLOCK * pn + 34 + rn * RULEBLOCK + 1) == 0) {  return 1;  }
  Serial.print("Rule #");
  Serial.print(rn);
  Serial.print(" does not exist for Profile #");
  Serial.println(pn);
  return 0;
}
void addProfile(char *name, char *id) {
  byte i = 0;
  int pn;
  do{
    pn = PROFILES + PROFILEBLOCK * i;
    if(EEPROM.read(pn) == 0) {
      writeEEPROMBytes(name, PROFILES + PROFILEBLOCK * i, 21); //Profile name
      writeEEPROMBytes(id, PROFILES + PROFILEBLOCK * i + 21, 11); //Profile id
      EEPROM.write(PROFILES + PROFILEBLOCK * i + 32, 0); //# of rules
      Serial.print("Adding Profile #");
      Serial.println(i);
      return;
    }
    i++;
  }while(i < MAXPROFILES);
  Serial.println("Maximum Profiles Reached");
}

void removeProfile(byte pn) {
  byte i;
  
  if(!profileCheck(pn)) {  return;  }
  
  for(i = 0; i < PROFILEBLOCK; i++) {  EEPROM.write(PROFILES + PROFILEBLOCK * pn + i, 0);  }
  Serial.print("Removing Profile #");
  Serial.println(pn);
}

void removeRule(byte pn, byte rn) {
  if(!ruleCheck(pn, rn)) {  return;  }
  for(byte i = 0; i < RULEBLOCK; i++) {  EEPROM.write(PROFILES + PROFILEBLOCK * pn + RULEBLOCK * rn, 0);  }
  Serial.print("Removing Rule #");
  Serial.print(rn);
  Serial.print(" from Profile #");
  Serial.println(pn);  
}

void editProfile(byte pn, char *name, bool active) {
  int p = PROFILES + PROFILEBLOCK * pn;
  byte temp = EEPROM.read(p + 32);
  
  if(!profileCheck(pn)) {  return;  }
  
  if(name[0]) {
    writeEEPROMBytes(name, p, 21);
    Serial.println("Profile Name changed");
  }
  if(active != bitRead(temp , 7)) {
    temp = temp ^ 0x80;
    EEPROM.write(p + 32, temp);
    Serial.println("Profile Active state changed");
  }
}

void editRule(byte pn, byte rn, byte temperature, byte type, unsigned int params1, unsigned int params2) {
  if(!ruleCheck(pn, rn)) {  return;  }
  ruleWrite(PROFILES + PROFILEBLOCK * pn, RULEBLOCK * rn, temperature, type, params1, params2);
}

void addRule(byte pn, byte temperature, byte type, unsigned int params1, unsigned int params2) {
  int p;
  byte i = 0;
  
  if(!profileCheck(pn)) {  return;  }
  
  p = PROFILES + PROFILEBLOCK * pn;
  while(i < MAXRULES) {
    if(EEPROM.read(p + 34 + i * RULEBLOCK + 1) == 0) {  break;  }
    i++;
  }
  if(i == MAXRULES) {
    Serial.print("Cannot add rule. Maximum rules reached for profile #");
    Serial.println(pn);
    return;
  }
  EEPROM.write(p + 32, EEPROM.read(p + 32) + 1); //# of rules
  ruleWrite(p, i * RULEBLOCK, temperature, type, params1, params2);
}

byte findActiveProfile() {
  byte i = 0;
  while(i < MAXPROFILES) {
    if(bitRead(EEPROM.read(PROFILES + PROFILEBLOCK * i + 32), 7)) {  return i;  }
    i++;
  }
  return MAXPROFILES;
}
