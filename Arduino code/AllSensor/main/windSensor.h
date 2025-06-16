#ifndef WINDSENSOR_H
#define WINDSENSOR_H

#include <Arduino.h>
#include <Wire.h>

class WindSensor {
public:
    WindSensor();
    void init();
    void update();
    float get_wind_speed() const;
    float get_wind_direction() const;
    void countPulse();
private:   
    float windSpeed;
    float heading;
    int heading_bytes;
    volatile int pulseCount;
    volatile unsigned long lastTime;
    bool EMAfilter = true;
    float alpha = 0.8;
    float filteredHeading;
};
#endif 
