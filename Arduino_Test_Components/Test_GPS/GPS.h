#ifndef GPS_H
#define GPS_H

#include <Arduino.h>
#include "config.h"
#include <Adafruit_GPS.h>

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
    GPS(HardwareSerial& serial);

    void update(); // Lit les trames NMEA, extrait les coordonnées si valides

    double getLatitude() const;
    double getLongitude() const;
    GPScoord getPoint() const;
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
    Adafruit_GPS* gps;
    bool parseGPRMC(const String& nmea); // Analyse la trame $GPRMC
    double convertToDecimal(const String& raw, const String& direction); // Convertit DMM -> degrés décimaux
};

#endif