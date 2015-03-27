//Required bytes: 27
//13 name
//11 sensor id
//1 sensor number
//1 latest temperature
//1 status & active

void addSensor(char *name, char *id, byte temp, byte sa) {
  byte i = 0;
  do{
    if(EEPROM.read(SENSORS + i * SENSORBLOCK) == '\0') {
      writeEEPROMBytes(name, SENSORS + SENSORBLOCK * i, 13);
      writeEEPROMBytes(id, SENSORS + SENSORBLOCK * i + 13, 11);
      EEPROM.write(SENSORS + SENSORBLOCK * i + 24, i);
      EEPROM.write(SENSORS + SENSORBLOCK * i + 25, temp);
      EEPROM.write(SENSORS + SENSORBLOCK * i + 26, sa);
      Serial.print("Adding Sensor #");
      Serial.println(i);
      return;
    }
    i++;
  }while(i < MAXSENSORS);
  Serial.println("Maximum Sensors Reached");
}

void removeSensor(byte sn) {
  if(sn == 0){
    Serial.println("ERROR: Cannot remove thermostat's sensor");
    return;  
  }
  EEPROM.write(SENSORS + SENSORBLOCK * sn, 0);
  Serial.print("Removing Sensor #");
  Serial.println(sn);
}

int sensorBytes() { //Returns an int that, if displayed in binary will have a 1 for active and 0 for not active
  byte i;
  int sensor = 0;
  for(i = 0; i < MAXSENSORS; i++) {
    if(EEPROM.read(SENSORS + SENSORBLOCK * i) != 0){  bitSet(sensor,i);  }
  }
  return sensor;
}

//Rotates Sensor List
void cycleSensorList(byte sn) {
  byte i;
  tft.fillRect(16, 215, 253, 20, BACKGROUND_COLOR);
  readEEPROMBytes(nam, SENSORS + SENSORBLOCK * sn, 13); //Read Sensor name from EEPROM
  i = cal(nam);
  printText(30, 219, nam);
  printText(30 + i * 12, 219, ":");
  unitPos(42 + i * 12, 219, EEPROM.read(SENSORS + SENSORBLOCK * sn + 25));
  delay(100);
}

void editSensor(byte sn, char *name, bool active) {
  byte temp = EEPROM.read(SENSORS + SENSORBLOCK * sn + 26);
  if(name[0] != 0) {
    writeEEPROMBytes(name, SENSORS + SENSORBLOCK * sn, 13);
    Serial.println("Sensor Name changed");
  }
  if(active != bitRead(temp , 7)) {
    temp = temp ^ 10000000;
    EEPROM.write(SENSORS + SENSORBLOCK * sn + 26, temp);
    Serial.println("Sensor Active state changed");
  }
}
