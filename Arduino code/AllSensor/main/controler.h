#ifndef CONTROLER_H
#define CONTROLER_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "config.h"

class Controler {
public:
    Controler();
    void init();
    void controling();
    void setUnmanned();
    
    bool unmanned = false;
private:
    Adafruit_PWMServoDriver pwm;

    const int controlPin = 23;
    const int elevationPin = 3;
    const int aileronPin = 2;

    int elevation, aileron, comSail, comRud, controlValue;
    int compteur = 0;
};

#endif // CONTROLER_H
