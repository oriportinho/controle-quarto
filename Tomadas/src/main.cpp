#include <Arduino.h>

#include <EmonLib.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define VOLTAGEM          127
#define VOLTASAMPERIMETRO 4
#define DHTTYPE           DHT11

// Modulo Rele 8 canais
int rele0   =  2;
int rele1   =  3;
int rele2   =  4;
int rele3   =  5;
int rele4   =  6;
int rele5   =  7;
int rele6   =  8;
int rele7   = 12;

// Sensores
int emon1   = A5;
int ledWork = A4;
int DHT     = A1;
int LDR     = A2;
int statusL = 13;

bool statusRele [] = {true,true,true,true,true,true,true,true};

EnergyMonitor emon;
DHT_Unified dht(DHT, DHTTYPE);

double calculatePower() {
  return ((emon.calcIrms(1480) * VOLTAGEM) / VOLTASAMPERIMETRO);
}

double calculateTemperature(sensors_event_t event) {
  dht.temperature().getEvent(&event);
  if(isnan(event.temperature)) {
    return -999;

  } else {
    return event.temperature;
  }
}

double calculateHumidity(sensors_event_t event) {
  dht.humidity().getEvent(&event);
  if(isnan(event.relative_humidity)) {
    return -1;

  } else {
    return event.relative_humidity;
  }
}

int tableRele(int a){
  int resp = rele0;
  if(a == 0)      { resp = rele0; }
  else if(a == 1) { resp = rele1; }
  else if(a == 2) { resp = rele2; }
  else if(a == 3) { resp = rele3; }
  else if(a == 4) { resp = rele4; }
  else if(a == 5) { resp = rele5; }
  else if(a == 6) { resp = rele6; }
  else if(a == 7) { resp = rele7; }
  return resp;
}

void switchRele(int i) {
  if(statusRele[i]) {
    digitalWrite(tableRele(i),  LOW);
  } else {
    digitalWrite(tableRele(i), HIGH);
  }

  statusRele[i] = !statusRele[i];
}

void readData(String data) { // "$3,0,0,0,0,0,0,0,0"
  int i = 3;
  int j = 0;

  if(data.length() > 17){
    while(i < 18) {

      if(data.charAt(i) == '0' && statusRele[j]){
        switchRele(i);
      } else if(data.charAt(i) == '1' && !statusRele[j]) {
        switchRele(i);
      }

      j += 1;
      i += 2;
    }
  }
}


void setup() {
  Serial.begin(9600);
  dht.begin();
  emon.current(emon1, 111.1);
}

void loop() {
  sensors_event_t event;



  delay(15);
}
