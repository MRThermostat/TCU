#define ESP_INIT_BAUD 9600

ESP8266 wifi(Serial1);

void setupWifi(){
  pinMode(ESP_GPIO0,OUTPUT);
  pinMode(ESP_GPIO2,OUTPUT);
  pinMode(ESP_RESET,OUTPUT);
  pinMode(ESP_CH_PD,OUTPUT);
  
  digitalWrite(ESP_GPIO0,HIGH);
  digitalWrite(ESP_GPIO2,HIGH);
  digitalWrite(ESP_RESET,LOW);
  digitalWrite(ESP_CH_PD,HIGH);
  
  wifi.restart();   
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
  digitalWrite(ESP_CH_PD,HIGH);
  digitalWrite(ESP_RESET,LOW);
  delay(10);
  
  //dump data out of arduino RX buffer
  flushRXBuffer();
  
  char resp[100] = "";
  
  //set reset pin high
  digitalWrite(ESP_RESET,HIGH);
  
  int timeout = 0;
  /*while((Serial1.available() == 0) && (timeout < 20)){
    timeout++;//do nothing wait for resp
    delay(5);
  }*/
  Serial1.readBytes(resp,100);
  
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

void setupNetwork()
{
}

void sendCommand(String command){
  flushRXBuffer();
  //Serial1.end();
  //Serial1.begin(9600);
  //digitalWrite(ESP_CH_PD,HIGH); //must be written high again after setup exits. setup writes this pin low on exit for some reason.
  char resp[100] = "";
  digitalWrite(ESP_GPIO0,HIGH);
  digitalWrite(ESP_GPIO2,HIGH);
  digitalWrite(ESP_RESET,HIGH);
  digitalWrite(ESP_CH_PD,HIGH);
  Serial1.println(command);
  
  /*while(Serial1.available() == 0){
    //do nothing wait for resp
  }*/
  digitalWrite(ESP_GPIO0,HIGH);
  digitalWrite(ESP_GPIO2,HIGH);
  digitalWrite(ESP_RESET,HIGH);
  digitalWrite(ESP_CH_PD,HIGH);
  Serial1.readBytes(resp,100);
  Serial.print("response:");
  Serial.println(resp);
}
