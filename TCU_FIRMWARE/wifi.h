#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define ESP_INIT_BAUD 9600

void setupWifi(){
  pinMode(ESP_GPIO0,OUTPUT);
  pinMode(ESP_GPIO2,OUTPUT);
  pinMode(ESP_RESET,OUTPUT);
  pinMode(ESP_CH_PD,OUTPUT);
  
  digitalWrite(ESP_GPIO0,HIGH);
  digitalWrite(ESP_GPIO2,HIGH);
  digitalWrite(ESP_RESET,HIGH);
  digitalWrite(ESP_CH_PD,HIGH);
    
  Serial1.begin(ESP_INIT_BAUD);
  Serial1.setTimeout(1000);
}

int findText(String needle, String haystack) {
  int foundpos = -1;
  for (int i = 0; (i < haystack.length() - needle.length()); i++) {
    if (haystack.substring(i,needle.length()+i) == needle) {
      foundpos = i;
    }
  }
  return foundpos;
}

void flushRXBuffer(){
  //dump data out of arduino RX buffer
  while(Serial1.available() > 0){
    Serial1.read();
  }  
}

int resetWifi(){
  digitalWrite(ESP_RESET,LOW);
  delay(100);
  
  //dump data out of arduino RX buffer
  flushRXBuffer();
  
  char resp[200] = "";
  
  //set reset pin high
  digitalWrite(ESP_RESET,HIGH);
  
  while(Serial1.available() == 0){
    //do nothing wait for resp
  }
  Serial1.readBytes(resp,200);
  
  #ifdef DEBUG
    Serial.print("ESP8266 initialized:");
    Serial.println(resp);
  #endif
  
  if(findText("ready",resp) != -1){
    Serial.println("WIFI MODULE READY");
    return 1;  
  }
  else{
    Serial.println("WIFI MODULE NOT READY");
    return 0;
  }

}

void sendCommand(String command){
  flushRXBuffer();
  
  digitalWrite(ESP_CH_PD,HIGH); //must be written high again after setup exits. setup writes this pin low on exit for some reason.
  char resp[200] = "";
  Serial1.println(command);
  
  while(Serial1.available() == 0){
    //do nothing wait for resp
  }
  Serial1.readBytes(resp,200);
  Serial.print("response:");
  Serial.println(resp);
}
