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
  digitalWrite(ESP_RESET,HIGH);

  wifi.restart();   
}

bool checkNetwork(){
  //todo send some request to the internets and get expected response
}



