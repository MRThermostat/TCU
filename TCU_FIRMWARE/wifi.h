#define ESP_INIT_BAUD 9600
#define CWLAP_LENGTH 72
#define SSIDLIST_LENGTH 256

ESP8266 wifi(Serial1);

void setupWifi() {
  pinMode(ESP_GPIO0, OUTPUT);
  pinMode(ESP_GPIO2, OUTPUT);
  pinMode(ESP_RESET, OUTPUT);
  pinMode(ESP_CH_PD, OUTPUT);

  digitalWrite(ESP_GPIO0, HIGH);
  digitalWrite(ESP_GPIO2, HIGH);
  digitalWrite(ESP_RESET, LOW);
  digitalWrite(ESP_CH_PD, HIGH);
  delay(100);
  digitalWrite(ESP_RESET, HIGH);

  delay(1000);
  wifi.restart();
  delay(1000);

  wifi.setOprToStationSoftAP();
  wifi.setSoftAPParam("MR THERMOSTAT", "", 7, 0);
  wifi.enableMUX();
  wifi.startTCPServer(80);
  wifi.setTCPServerTimeout(10);
}

bool checkWeather() {
  //todo send some request to the internets and get expected response
}

bool checkNetwork() {
  return 0;
}

void parseSSIDs(char* ssidlist) {
  char buffer[CWLAP_LENGTH];

  Serial1.setTimeout(5000);
  int charpos = 0;
  //Serial.print("wait for 0");
  Serial1.println("AT+CWLAP");
  while ((Serial1.readBytesUntil(10, buffer, CWLAP_LENGTH) > 0) && (charpos < SSIDLIST_LENGTH)) {  //line feed
    //Serial.println(buffer);
    int quote = 0;

    for (int i = 0; i < CWLAP_LENGTH; i++) {

      if ((quote > 0) && (quote < 2)) {
        ssidlist[charpos] = buffer[i];
        charpos++;
      }
      if (buffer[i] == 34) { //dec 34 is doublequote char
        if (quote == 1) {
          ssidlist[charpos - 1] = '\n';
          //ssidlist[charpos - 1] = '*';
          //charpos++;
        }
        quote++;
        /*if(quote == 2){
          i = CWLAP_LENGTH;
        }*/
      }
      //if (buffer[i] == '\r') {
      //  i = 74;
      //}
    }
  }
  //Serial.println("FOUND IT");
}

int generateSetupResponse(uint8_t* buffer, char* ssidlist) {
  const uint8_t firstHalf[78] = "<html><h1>Mr Thermostat Setup</h1><form>Choose Network:<br><select name=\"nw\">";
  const uint8_t secondHalf[114] = "</select><br>Password:<br><input type=\"text\" name=\"pw\"><br><br><input type=\"submit\" value=\"Submit\"></form></html>";
  const uint8_t one[] = "<option value=\"";
  const uint8_t two[] = "</option>";
  int pos = 0;
  int ssidpos = 0;

  for (int i = 0; i < 77; i++) {
    buffer[pos + i] = firstHalf[i];
  }
  pos += 77;
/*
  while (ssidlist[ssidpos] != '\r') {

    for (int i = 0; i < 15; i++) {
      buffer[pos + i] = one[i];
    }
    pos += 15;

    for (int i = 0; ssidlist[ssidpos + i] != '\n'; pos++) {
      buffer[pos] = ssidlist[ssidpos + i];
      i++;
    }

    buffer[pos] = '\"';
    pos++;
    buffer[pos] = '>';
    pos++;

    for (ssidpos; ssidlist[ssidpos] != '\n'; pos++) {
      buffer[pos] = ssidlist[ssidpos];
      ssidpos++;
    }

    for (int i = 0; i < 9; i++) {
      buffer[pos + i] = two[i];
    }
    pos += 9;

    ssidpos++;
  }
*/
  for (int i = 0; i < 113; i++) {
    buffer[pos + i] = secondHalf[i];
  }
  pos += 113;
  buffer[pos]='\0';

  return pos-10;
}

int generateSensorData() {

}

int generateRuleData() {

}

void setupNetwork() {
  wifi.setOprToStationSoftAP();
  wifi.setSoftAPParam("MR THERMOSTAT", "", 7, 0);
  wifi.enableMUX();
  wifi.startTCPServer(80);
  wifi.setTCPServerTimeout(10);
  Serial.print("IP: ");
  Serial.println(wifi.getLocalIP().c_str());

  char ssidlist[SSIDLIST_LENGTH] = {0};
  parseSSIDs(ssidlist);
  Serial.println(ssidlist);

  Serial.println("GO AHEAD");
  while (1) {

    pinMode(ESP_GPIO0, OUTPUT);
    pinMode(ESP_GPIO2, OUTPUT);
    pinMode(ESP_RESET, OUTPUT);
    pinMode(ESP_CH_PD, OUTPUT);

    digitalWrite(ESP_GPIO0, HIGH);
    digitalWrite(ESP_GPIO2, HIGH);
    digitalWrite(ESP_RESET, HIGH);
    digitalWrite(ESP_CH_PD, HIGH);


    uint8_t buffer[200] = {0};
    char buffer2[50] = {0};
    uint8_t mux_id;
    uint32_t len = wifi.recv(&mux_id, buffer, sizeof(buffer), 200);

    sprintf (buffer2, "buffer points to: %p", buffer);
    Serial.println(buffer2);

    if (len > 0) {
      Serial.print("Status:[");
      Serial.print(wifi.getIPStatus().c_str());
      Serial.println("]");
      
      sprintf (buffer2, "buffer points to: %p", buffer);
      Serial.println(buffer2);
      
      Serial.print("Received from :");
      Serial.print(mux_id);
      
      sprintf (buffer2, "buffer points to: %p", buffer);
      Serial.println(buffer2);
      
      Serial.print("[");
      for (uint32_t i = 0; i < len; i++) {
        Serial.print((char)buffer[i]);
      }
      Serial.print("]\r\n");
      
      sprintf (buffer2, "buffer points to: %p", buffer);
      Serial.println(buffer2);
      
      //len = generateSetupResponse(buffer,ssidlist);
      if (wifi.send(mux_id, buffer, len)) {
        Serial.print("send back ok\r\n");
      }
      else {
        Serial.print("send back err\r\n");
      }
      
      sprintf (buffer2, "buffer points to: %p", buffer);
      Serial.println(buffer2);

      if (wifi.releaseTCP(mux_id)) {
        Serial.print("release tcp ");
        Serial.print(mux_id);
        Serial.println(" ok");
      }
      else {
        Serial.print("release tcp");
        Serial.print(mux_id);
        Serial.println(" err");
      }

      Serial.print("Status:[");
      Serial.print(wifi.getIPStatus().c_str());
      Serial.println("]");
    }
  }
}





