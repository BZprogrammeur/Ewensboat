#include "GPS.h"

GPS* gpsInstance = nullptr;

GPS::GPS(HardwareSerial& serial, unsigned long baud = 9600)
    : gpsSerial(serial), latitude(0.0), longitude(0.0), validFix(false) {
        Serial.println("Initialising GPS...");
        gpsSerial.begin(baud);
    }

void GPS::update() {

  while (gpsSerial.available()) {
    char c = gpsSerial.read();

    if (c == '\n') {
      if (nmeaBuffer.startsWith("$GPRMC")) {
        validFix = parseGPRMC(nmeaBuffer);
      }
      else if (nmeaBuffer.startsWith("$GPGGA")) {
        parseGPGGA(nmeaBuffer);  // Met à jour les satellites
      }
      nmeaBuffer = ""; // reset
    } 
    else if (c != '\r') {
      nmeaBuffer += c;
    }
  }
  // logfile.close();
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

    if (parts[2] != "A") return false; // 'A' = valid data

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

int GPS::getSatelliteCount() const {
    return satellites;
}

GPScoord GPS::getPoint() const {
    return GPScoord{latitude, longitude};
}

bool GPS::parseGPGGA(const String& nmea) {
    // Ex: $GPGGA,123519,4807.038,N,01131.000,E,1,08,...
    if (!nmea.startsWith("$GPGGA")) return false;

    int fieldIndex = 0;
    int start = 0;
    int end = 0;
    while (fieldIndex <= 7 && end != -1) {
        end = nmea.indexOf(',', start);
        if (fieldIndex == 7) {
            String satStr = nmea.substring(start, end);
            satellites = satStr.toInt();
            return true;
        }
        start = end + 1;
        fieldIndex++;
    }
    return false;
}

Cartcoord GPS::conversion(GPScoord point) {
  Cartcoord result;

  // Conversion des degrés en radians
  double lat1 = M.lat * M_PI / 180.0;
  double lat2 = point.lat * M_PI / 180.0;
  double dLat = lat2 - lat1;
  double dLng = (point.lng - M.lng) * M_PI / 180.0;

  // Approximation de projection plane (petites distances)
  result.x = R_EARTH * dLng * cos(lat1); // Est
  result.y = R_EARTH * dLat;                            // Nord

  return result;
}

