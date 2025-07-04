#include <Arduino.h>
#include "IMU.h"
#include "GPS.h"
#include "windSensor.h"
#include "controlMotor.h"
#include "ewensboatlib.h"

void setup() {
  Serial.begin(9600);
  imu_init();
  GPS_init();
  motor_init();
  wind_sensor_init();
}

void loop() {
  float cap_cible = 90.0;  // cap cible en degrés
  follow_cap(cap_cible);
  set_sail_pos();
  delay(500);  // 100 ms = DELTA_T
}
