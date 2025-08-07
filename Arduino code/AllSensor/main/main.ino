#include <Arduino.h>
#include "IMU.h"
#include "GPS2.h"
#include "windSensor.h"
#include "controlMotor.h"
#include "ewensboatlib.h" 
#include "controler.h"
#include "SDcard.h"
#include "config.h"

GPS2 gps_boat(Serial2); 
IMU imu;
controlMotor powerboard;
WindSensor wind;
Controler manette;
Navigation nav(imu, powerboard, wind, gps_boat);
SDcard sd;

int display_count = 0;
GPScoord point_cible = {52.4844041, -1.8898449}; 
GPScoord start_point = {52.4863908, -1.8889920};
GPScoord rondpoint = {52.4859933, -1.8899753}; //-9,177
GPScoord cloturefootball = {52.4876462, -1.8887633}; //73,360
GPScoord croisementgymnase = {52.4862247, -1.8873002}; //173,202
GPScoord startCanal = {52.488429, -1.886253};
GPScoord endCanal = {52.487312, -1.885140};
GPScoord SU = {52.4844688, -1.8895073};
GPScoord eastSide = {52.4843099, -1.8906130};
GPScoord cornerBuilding = {52.486006, -1.887655};
GPScoord cornerParking = {52.486308, -1.886213};
GPScoord flecheAnglaise = {52.485827, -1.885899};
GPScoord entryParking = {52.485067, -1.886799};
GPScoord ESBenergy = {52.486141, -1.887731};
GPScoord pointLac1 = {52.429233, -1.946685};
GPScoord pointLac2 = {52.429465, -1.946741};
GPScoord pointLac3 = {52.429524, -1.945309};
GPScoord pointLac4 = {52.429329, -1.945999};

GPScoord cheminLac[4] = {pointLac1, pointLac2, pointLac3};
GPScoord chemin[7]= {rondpoint, cornerBuilding, cornerParking, flecheAnglaise, entryParking, ESBenergy, rondpoint};

void setup() {
  Serial.begin(9600);
  Serial.print("démarrage du boat");
  imu.init();
  gps_boat.init();
  powerboard.init();
  wind.init();
  manette.init();
  sd.init();

}

void loop() {
  maj();
  display();
  save_data();
  if (manette.checkUnmanned())
  {
//    float cap_cible = 135.0;  // cap cible en degrés
//    nav.follow_cap(cap_cible);
    
    nav.follow_path2(cheminLac, 3);
    //nav.reach_point(point_cible);çà
    //nav.line_following(SU, eastSide);
    nav.set_sail_pos();
  }
  else
  {
    powerboard.send_com_rudder(manette.get_com_rudder());
    powerboard.send_com_sail(manette.get_com_sail());
  }
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
    Serial.print(point.lat, 6);
    Serial.print("        ");
    Serial.print("Longitude: ");
    Serial.print(point.lng, 6);
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
  manette.update();
}

void save_data(){
  GPScoord point = gps_boat.getPoint();
  float v = wind.get_wind_speed();
  float d = wind.get_wind_direction();
  float cap = imu.get_cap();
  float angle_sail = powerboard.getSailPos();
  float angle_rudder = nav.getRudderPos();
  sd.saveDatas(millis(), point.lat, point.lng, d, v, cap, angle_rudder, angle_sail, manette.unmanned_status());
  
}
