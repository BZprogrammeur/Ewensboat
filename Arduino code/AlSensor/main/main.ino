#include <Arduino.h>
#include "IMU.h"
#include "GPS.h"
#include "windSensor.h"
#include "controlMotor.h"
#include "ewensboatlib.h"

GPS gps_boat(Serial2); 
IMU imu;
controlMotor powerboard;
WindSensor wind;
nav nav;


void setup() {
  Serial.begin(9600);
  imu.init();
  gps_boat.init();
  powerboard.init();
  wind.init();
}

void loop() {
  float cap_cible = 90.0;  // cap cible en degrés
  nav.follow_cap(cap_cible);
  delay(100);  // 100 ms = DELTA_T
}
