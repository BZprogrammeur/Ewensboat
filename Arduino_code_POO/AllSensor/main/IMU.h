#ifndef IMU_H
#define IMU_H

#include <Arduino.h>
#include "config.h"

class IMU {
public:
    IMU();
    void update();
    float get_heading();
    bool calibrate();

private:
    float cap;
};

float sawtooth(float x);
#endif 
