#ifndef GPS2_H
#define GPS2_H

#include <Arduino.h>
#include <Adafruit_GPS.h>
#include "config.h"

class GPS2 {
public:
    GPS2(HardwareSerial& serial);  // Constructeur
    void init(unsigned long baud = 9600);
    void update();
    
    double getLatitude() const;
    double getLongitude() const;
    GPScoord getPoint() const;
    bool isValid() const;
    int getSatellites() const;

    Cartcoord conversion(GPScoord point);

private:
    HardwareSerial& gpsSerial;
    Adafruit_GPS* gps;  // Pointeur vers l'objet GPS
    bool validFix;
};

#endif
