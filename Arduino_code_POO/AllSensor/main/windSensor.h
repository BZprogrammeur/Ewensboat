#ifndef WINDSENSOR_H
#define WINDSENSOR_H

#include <Arduino.h>
#include <Wire.h>

class WindSensor {
public:
    WindSensor();
    void update_speed();
    void update_heading();
    float get_wind_speed() ;
    float get_wind_direction() ;
    void countPulse();
private:   
    float windSpeed;
    float heading;
    int heading_bytes;
    volatile int pulseCount;
};
#endif 
