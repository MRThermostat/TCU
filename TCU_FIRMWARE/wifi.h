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

  wifi.restart();

  if (wifi.setOprToStation()) {
    Serial.print("to station ok\r\n");
  }
  else {
    Serial.print("to station err\r\n");
  }
  if (wifi.enableMUX()) {
    Serial.print("multiple ok\r\n");
  }
  else {
    Serial.print("multiple err\r\n");
  }
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

int generateResponse(uint8_t* buffer, char* ssidlist) {
  const uint8_t firstHalf[79] = "<html><h1>Mr Thermostat Setup</h1><form>Choose Network:<br><select name=\"nw\">";
  const uint8_t secondHalf[120] = "</select><br>Password:<br><input type=\"text\" name=\"pw\"><br><br><input type=\"submit\" value=\"Submit\"></form></html>";
  const uint8_t one[] = "<option value=\"";
  const uint8_t two[] = "</option>";
  int pos = 0;
  for (int i = 0; i < 79; i++) {
    buffer[pos + i] = firstHalf[i];
  }
  pos += 77;
  //<option value="volvo">Volvo</option>
  
  for (int i = 0; i < 15; i++) {
    buffer[pos + i] = one[i];
  }
  pos += 15;
  
  for (int i = 0; ((i < 32)&&(ssidlist[i] != '\n')); pos++) {
    buffer[pos] = ssidlist[i];
    i++;
  }
  
  buffer[pos] = '\"';
  pos++;
  buffer[pos] = '>';
  pos++;
  
  for (int i = 0; ((i < 32)&&(ssidlist[i] != '\n')); pos++) {
    buffer[pos] = ssidlist[i];
    i++;
  }
  
  for (int i = 0; i < 120; i++) {
    buffer[pos + i] = secondHalf[i];
  }
  pos += 113;

  return pos-2;
}

void setupNetwork() {
  if (wifi.setOprToStationSoftAP()) {
    Serial.print("to station + soft ap\r\n");
  }
  else {
    Serial.print("error1\r\n");
  }

  char ssidlist[SSIDLIST_LENGTH];
  parseSSIDs(ssidlist);
  Serial.println(ssidlist);

  wifi.setSoftAPParam("MR THERMOSTAT", "", 7, 0);
  wifi.enableMUX();
  wifi.startTCPServer(80);
  wifi.setTCPServerTimeout(10);
  Serial.print("IP: ");
  Serial.println(wifi.getLocalIP().c_str());
  
  Serial.println("GO AHEAD");
  while (1) {
    Serial.print("freeMemory()=");
    Serial.println(freeMemory());
    uint8_t buffer[200] = {0};
    uint8_t mux_id;
    uint32_t len = wifi.recv(&mux_id, buffer, sizeof(buffer), 200);
    //Serial.println("created vars");
    if (len > 0) {
      Serial.print("Received from :");
      Serial.print(mux_id);
      Serial.print("[");
      for (uint32_t i = 0; i < len; i++) {
        Serial.print((char)buffer[i]);
      }
      Serial.print("]\r\n");
      len = generateResponse(buffer, ssidlist);
      if (wifi.send(mux_id, buffer, len)) {
        Serial.print("send back ok\r\n");
      }
      else {
        Serial.print("send back err\r\n");
      }

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





