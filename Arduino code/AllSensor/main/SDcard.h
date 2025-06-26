#ifndef SDCARD_H
#define SDCARD_H

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

class SDcard {
public:
    SDcard() = default;
    void init(); 
    void saveDatas(float time, float latitude, float longitude,
                   float wind_direction, float wind_speed, float heading,
                   int angle_rud, int angle_sail, bool control);

private:
    const int chipSelect = 53;  // Pin CS pour la carte SD (Mega 2560)
    File logFile;
};

#endif
