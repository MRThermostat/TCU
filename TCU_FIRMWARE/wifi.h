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
  delay(100);
  /**char resp[100];
  Serial1.setTimeout(2000);*/
  digitalWrite(ESP_RESET,HIGH);
  //delay(3000);
  /*
  Serial1.readBytes(resp,100);
  
  Serial.println(resp);
  while(1){
  Serial1.println("AT+RST");
  resp[Serial1.readBytes(resp,100)]='\0';
  Serial.println(resp);
  delay(1000);
  }  */
  wifi.restart();   
}

bool checkNetwork(){
  
}

void setupNetwork(){
   if(wifi.setSoftAPParam("MR THERMOSTAT", "password",7,0)){
      Serial.print("Soft Access Point parameters set\r\n"); 
    } 
    else {
      Serial.print("Soft Access Point parameters not set\r\n"); 
    }

    if (wifi.setOprToSoftAP()) {
      Serial.print("Soft Access Point started\r\n");   
    }
    else{
      Serial.print("Soft Access Point failed\r\n");
    } 
 
   while(1){
    Serial.println(wifi.getAPList());
   Serial.println(wifi.getJoinedDeviceIP());
  delay(5000); 
   }
}

