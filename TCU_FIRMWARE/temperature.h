#define KELVIN 0
#define CELSIUS 1
#define FAHRENHEIT 2
#define VOLTAGE 3
#define RAW 4
#define LM335_COEF 0.01
#define GAIN 10.0
#define MIN_VOLTAGE 2.77

void setupTemp(){
  //setup temperature hardware
  pinMode(TEMP_PIN,INPUT);
  digitalWrite(TEMP_PIN,LOW);
  //analogReference(DEFAULT);
}

float getTemp(int unit = 2){
  int number_samples = 5;
  int adc_count = 0;
  float adc_voltage, voltage;
  
  for(int i = 0;i<number_samples;i++){
    adc_count += analogRead(TEMP_PIN);
  }
  adc_count = adc_count/number_samples;
  
  adc_voltage = adc_count*(AREF/1024.0);
  //adc_voltage=GAIN(pregain_voltage-TEMPERATURE_VOLTAGE_REFERENCE)
  voltage = (adc_voltage/GAIN)+MIN_VOLTAGE; 
  //voltage = MIN_VOLTAGE+(adc_count*(AREF/1024));
  
  switch (unit){
    case KELVIN:
      return (voltage/LM335_COEF); //kelvin temperature
      break;
    case CELSIUS:
      return (voltage/LM335_COEF)-273.15; //celsius temperature
      break;
    case FAHRENHEIT:
      return (((voltage/LM335_COEF)-273.15)*(9.0/5.0))+32.0; //fahrenheit temperature
      break;
    case VOLTAGE:
      return voltage;
      break;
    case RAW:
      return adc_count;
      break;
  }
}

