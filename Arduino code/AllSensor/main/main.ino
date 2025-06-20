#include <Arduino.h>
#include "IMU.h"
#include "GPS2.h"
#include "windSensor.h"
#include "controlMotor.h"
#include "ewensboatlib.h"
#include "controler.h"
#include "SDcard.h"

GPS2 gps_boat(Serial2); 
IMU imu;
controlMotor powerboard;
WindSensor wind;
Controler manette;
Navigation nav(imu, powerboard, wind, gps_boat);
SDcard sd;

int display_count = 0;


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
  maj();
  display();
//  if (manette.unmanned)
//  {
    
//    float cap_cible = 90.0;  // cap cible en degrés
//    nav.follow_cap(cap_cible);
    GPScoord point_cible = {52.4844041, -1.8898449};
    nav.reach_point(point_cible);
   
//  }
//  else
//  {
//    manette.controling();
//  }
}

void display()
{
  if (display_count == 5)
  {
    Serial.print(imu.get_cap());
    Serial.print("        ");
    Serial.print(nav.getTacking());
    GPScoord point = gps_boat.getPoint();
    Serial.print("Latitude: ");
    Serial.print(point.lat);
    Serial.print("        ");
    Serial.print("Longitude: ");
    Serial.println(point.lng);
    Serial.print("        ");
    Serial.print("Sat: ");
    Serial.print(gps_boat.getSatellites());
    Serial.print("        ");
    Serial.print("vent ");
    Serial.print(wind.get_wind_speed());
    Serial.print("        ");
    Serial.println(wind.get_wind_direction());
    display_count = 0;
  }
  else {
    display_count++;
  }
}

void maj()
{
  wind.update();
  gps_boat.update();
  imu.update();
}

void save_data(){
  GPScoord point = gps_boat.getPoint();
  float v = wind.get_wind_speed();
  float d = wind.get_wind_direction();
  float cap = imu.get_cap();
  float angle_sail = powerboard.getSailPos();
  float angle_rudder = nav.getRudderPos();
  sd.saveDatas(millis(), point.lat, point.lng, d, v, cap, angle_rudder, angle_sail, manette.unmanned);
  
}
