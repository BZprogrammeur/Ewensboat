#ifndef GPS_H
#define GPS_H

#include <Arduino.h>
#include "config.h"

class GPS {
public:
    GPS(HardwareSerial& serial);

    void init(unsigned long baud = 9600);
    void update(); // Lit les trames NMEA, extrait les coordonnées si valides

    double getLatitude() const;
    double getLongitude() const;
    bool isValid() const;

    Cartcoord conversion(GPScoord point);

private:
    HardwareSerial& gpsSerial;
    String nmeaBuffer;
    double latitude;
    double longitude;
    bool validFix;

    bool parseGPRMC(const String& nmea); // Analyse la trame $GPRMC
    double convertToDecimal(const String& raw, const String& direction); // Convertit DMM -> degrés décimaux
};

#endif
