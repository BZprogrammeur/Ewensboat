#include "GPS.h"

GPS::GPS(HardwareSerial& serial) : gpsSerial(serial) {
    gps = new Adafruit_GPS(&gpsSerial);  // Initialisation correcte
    validFix = false;
    gpsSerial.begin(9600);
    gps->begin(9600);
    gps->sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    gps->sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
    delay(100);
}

void GPS::update() {
  while (gps->available()) {
    char c = gps->read();
    if (gps->newNMEAreceived()) {
      if (!gps->parse(gps->lastNMEA())) return;
      validFix = gps->fix;
    }
  }
  // Serial.print("Longitude : ");
  // Serial.println(longitude);
  // Serial.print("Latitude : ");
  // Serial.println(latitude);
}

double GPS::getLatitude() const {
    return gps->latitudeDegrees;
}

double GPS::getLongitude() const {
    return gps->longitudeDegrees;
}

GPScoord GPS::getPoint() const {
    return GPScoord{getLatitude(), getLongitude()};
}

bool GPS::isValid() const {
    return validFix;
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