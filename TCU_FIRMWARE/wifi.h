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
  /*if(wifi.setSoftAPParam("MR THERMOSTAT", "password",7,0)){
    Serial.print("Soft Access Point parameters set\r\n"); 
    if (wifi.setOprToSoftAP()) {
      Serial.print("Soft Access Point started\r\n");   
    }
    else{
      Serial.print("Soft Access Point failed\r\n");
    } 
  } 
  else {
    Serial.print("Soft Access Point parameters not set\r\n"); 
  }

  if (wifi.enableMUX()) {
    Serial.print("multiple ok\r\n");
  } 
  else {
    Serial.print("multiple err\r\n");
  }

  if (wifi.startTCPServer(80)) {
    Serial.print("start tcp server ok\r\n");
  } 
  else {
    Serial.print("start tcp server err\r\n");
  }

  if (wifi.setTCPServerTimeout(10)) { 
    Serial.print("set tcp server timout 10 seconds\r\n");
  } 
  else {
    Serial.print("set tcp server timout err\r\n");
  }
 */
}

