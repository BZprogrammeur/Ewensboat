#include "GPS2.h"

GPS2::GPS2(HardwareSerial& serial) : gpsSerial(serial) {
    gps = new Adafruit_GPS(&gpsSerial);  // Initialisation correcte
    validFix = false;
}

void GPS2::init(unsigned long baud) {
    gpsSerial.begin(baud);
    gps->begin(baud);
    gps->sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    gps->sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
    delay(1000);
}
void GPS2::update() {
  while (gps->available()) {
    char c = gps->read();
    if (gps->newNMEAreceived()) {
      if (!gps->parse(gps->lastNMEA())) return;
      validFix = gps->fix;
    }
  }
}

double GPS2::getLatitude() const {
    return gps->latitudeDegrees;
}

double GPS2::getLongitude() const {
    return gps->longitudeDegrees;
}

GPScoord GPS2::getPoint() const {
    return GPScoord{getLatitude(), getLongitude()};
}

bool GPS2::isValid() const {
    return validFix;
}

int GPS2::getSatellites() const {
    return gps->satellites;
}


Cartcoord GPS2::conversion(GPScoord point) {
  Cartcoord result;

  // Conversion des degrés en radians
  double lat1 = M.lat * M_PI / 180.0;
  double lat2 = point.lat * M_PI / 180.0;
  double dLat = lat2 - lat1;
  double dLng = (point.lng - M.lng) * M_PI / 180.0;

  // Approximation de projection plane (petites distances)
  result.x = R_EARTH * dLng * cos((lat1 + lat2) / 2.0); // Est
  result.y = R_EARTH * dLat;                            // Nord

  return result;
}
