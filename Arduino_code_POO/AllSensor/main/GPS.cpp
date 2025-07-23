#include "GPS.h"

GPS::GPS() {
  gps = new TinyGPSPlus();
  validdata = false;
  delay(100);
  Serial2.begin((9600));
}

void GPS::update() {
  while (Serial2.available() > 0)
    if (gps->encode(Serial2.read())){
      validdata = gps->location.isValid();
      if(validdata){
        latitude = gps->location.lat();
        longitude = gps->location.lng();
      }
      if(gps->speed.isValid()){
        SOG = gps->speed.kmph();
      }
    }
}

double GPS::getLatitude() const {
  return latitude;
}

double GPS::getLongitude() const {
  return longitude;
}

float GPS::getSOG(){
  return SOG;
}

GPScoord GPS::getPoint() const {
  return GPScoord{getLatitude(), getLongitude()};
}

bool GPS::isValid() const {
  return validdata;
}


Cartcoord GPS::conversion(GPScoord point) {
  Cartcoord result;

  // Conversion des degrés en radians
  double lat1 = M.lat * M_PI / 180.0;
  double lat2 = point.lat * M_PI / 180.0;
  double dLat = lat2 - lat1;
  double dLng = (point.lng - M.lng) * M_PI / 180.0;

  result.x = R_EARTH * dLng * cos(lat1);                // East
  result.y = R_EARTH * dLat;                            // North

  return result;
}