#ifndef GPS_H
#define GPS_H

#include <Arduino.h>
#include "config.h"

struct GPScoord{
  double lat;
  double lng;
};

struct Cartcoord{
  double x;
  double y;
};

const GPScoord M = {52.4844041, -1.8898449};

class GPS {
public:
    GPS(HardwareSerial& serial, unsigned long baud = 9600);

    void update(); // Lit les trames NMEA, extrait les coordonnées si valides

    double getLatitude() const;
    double getLongitude() const;
    GPScoord getPoint() const;
    int getSatelliteCount() const;
    bool parseGPGGA(const String& nmea);
    bool isValid() const;

    Cartcoord conversion(GPScoord point);

private:
    HardwareSerial& gpsSerial;
    String nmeaBuffer;
    double latitude;
    double longitude;
    int satellites;
    bool validFix;

    bool parseGPRMC(const String& nmea); // Analyse la trame $GPRMC
    double convertToDecimal(const String& raw, const String& direction); // Convertit DMM -> degrés décimaux
};

#endif
