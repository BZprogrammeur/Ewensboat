#ifndef IMU_H
#define IMU_H

#include <Arduino.h>
#include "config.h"

class IMU {
public:
    IMU();
    void update();
    void init();
    float get_cap();
    bool calibrate();

private:
    float cap;
};

#endif 
