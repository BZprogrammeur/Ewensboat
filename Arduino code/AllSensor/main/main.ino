#include <Arduino.h>
#include "IMU.h"
#include "GPS.h"
#include "windSensor.h"
#include "controlMotor.h"
#include "ewensboatlib.h"
#include "controler.h"

GPS gps_boat(Serial2); 
IMU imu;
controlMotor powerboard;
WindSensor wind;
Controler manette;
Navigation nav(imu, powerboard, wind, gps_boat);
bool check;
float wspeed, wdir;


void setup() {
  Serial.begin(9600);
  imu.init();
  gps_boat.init();
  powerboard.init();
  wind.init();
  manette.init();
}

void loop() {
  manette.setUnmanned();
  wind.update();
  if (manette.unmanned)
  {
    
    float cap_cible = 90.0;  // cap cible en degrés
    nav.follow_cap(cap_cible);
    delay(100);  // 100 ms = DELTA_T
    check = nav.getTacking();
    Serial.println(check);
    wspeed = wind.get_wind_speed();
    wdir = wind.get_wind_direction();
    Serial.print("vitesse ");;
    Serial.println(wspeed);
    Serial.print("orientation ");
    Serial.println(wdir);
    delay(1000);
  }
  else
  {
    manette.controling();
  }
}
