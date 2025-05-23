#include <Arduino.h>
#include "IMU.h"
#include "GPS.h"
#include "windSensor.h"
#include "controlMotor.h"
#include "ewensboatlib.h"

GPS gps_boat;  

void setup() {
  Serial.begin(9600);
  imu_init();
  gps_boat.init();
  motor_init();
  wind_sensor_init();
}

void loop() {
  float cap_cible = 90.0;  // cap cible en degrés
  follow_cap(cap_cible);
  delay(100);  // 100 ms = DELTA_T
}
