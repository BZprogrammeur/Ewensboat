#include "GPS.h"

GPS::GPS(HardwareSerial& serial)
    : gpsSerial(serial), latitude(0.0), longitude(0.0), validFix(false) {}

void GPS::init(unsigned long baud) {
    gpsSerial.begin(baud);
    Serial.println("Initialisation GPS sans TinyGPS...");
}

void GPS::update() {
    while (gpsSerial.available()) {
        char c = gpsSerial.read();

        if (c == '\n') {
            if (nmeaBuffer.startsWith("$GPRMC")) {
                validFix = parseGPRMC(nmeaBuffer);
            }
            nmeaBuffer = ""; // reset
        } else if (c != '\r') {
            nmeaBuffer += c;
        }
    }
}

bool GPS::parseGPRMC(const String& nmea) {
    // Exemple : $GPRMC,123519,A,4807.038,N,01131.000,E,...
    int fields[12];
    String parts[12];
    int index = 0, lastComma = 0;

    for (int i = 0; i < nmea.length(); i++) {
        if (nmea.charAt(i) == ',') {
            parts[index++] = nmea.substring(lastComma, i);
            lastComma = i + 1;
            if (index >= 12) break;
        }
    }

    if (parts[2] != "A") return false; // 'A' = Active fix

    String rawLat = parts[3];
    String latDir = parts[4];
    String rawLon = parts[5];
    String lonDir = parts[6];

    latitude = convertToDecimal(rawLat, latDir);
    longitude = convertToDecimal(rawLon, lonDir);
    return true;
}

double GPS::convertToDecimal(const String& raw, const String& direction) {
    if (raw.length() < 6) return 0.0;

    double deg = raw.substring(0, raw.length() - 7).toDouble();
    double min = raw.substring(raw.length() - 7).toDouble();

    double decimal = deg + (min / 60.0);
    if (direction == "S" || direction == "W") decimal *= -1;

    return decimal;
}

double GPS::getLatitude() const {
    return latitude;
}

double GPS::getLongitude() const {
    return longitude;
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

  // Approximation de projection plane (petites distances)
  result.x = R_EARTH * dLng * cos((lat1 + lat2) / 2.0); // Est
  result.y = R_EARTH * dLat;                            // Nord

  return result;
}
