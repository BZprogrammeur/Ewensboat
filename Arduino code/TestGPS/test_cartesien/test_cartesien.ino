#include "GPS2.h"
#include "config.h"

GPS2 gps_boat(Serial2); 

void setup() {
  Serial.begin(9600);
  Serial.print("démarrage du boat");
  gps_boat.init();

  GPScoord point_cible = {52.4844041, -1.8898449}; //0,0 3,0.5
  GPScoord start_point = {52.4863908, -1.8889920};
  GPScoord rondpoint = {52.4859933, -1.8899753}; //-9,177 -15,186
  GPScoord cloturefootball = {52.4876462, -1.8887633}; //73,360 74.5,359.5
  GPScoord croisementgymnase = {52.4862247, -1.8873002}; //173,202 //163, 210
}

void loop() {
  gps_boat.update();
  GPScoord point = gps_boat.getPoint();
  Cartcoord pointconv = gps_boat.conversion(point);
  Serial.print("Latitude: ");
  Serial.print(point.lat);
  Serial.print("        ");
  Serial.print("Longitude: ");
  Serial.print(point.lng);
  Serial.print("y :");
  Serial.print(pointconv.y);
  Serial.print("        ");
  Serial.print("x: ");
  Serial.println(pointconv.x);
}
