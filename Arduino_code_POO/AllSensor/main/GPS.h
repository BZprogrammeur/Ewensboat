#ifndef GPS_H
#define GPS_H

#include <Arduino.h>
#include "config.h"
#include <TinyGPSPlus.h>
// https://github.com/mikalhart/TinyGPSPlus/

struct GPScoord{
  double lat;
  double lng;
};

struct Cartcoord{
  double x;
  double y;
};

const GPScoord M = {52.429369, -1.946515};

class GPS {
public:
    GPS();

    void update(); // Lit les trames NMEA, extrait les coordonnées si valides

    double getLatitude() const;
    double getLongitude() const;
    GPScoord getPoint() const;
    bool parseGPGGA(const String& nmea);
    bool isValid() const;
    float getSOG();
    Cartcoord conversion(GPScoord point);

private:
    TinyGPSPlus* gps = nullptr;
    String nmeaBuffer;
    double latitude;
    double longitude;
    float SOG = 0;
    int satellites;
    bool validdata;
    bool parseGPRMC(const String& nmea); // Analyse la trame $GPRMC
    double convertToDecimal(const String& raw, const String& direction); // Convertit DMM -> degrés décimaux
};

#endif
