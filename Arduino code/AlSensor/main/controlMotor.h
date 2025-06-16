#ifndef CONTROLMOTOR_H
#define CONTROLMOTOR_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "config.h"

class controlMotor {
public:
    controlMotor();
    void init();
    void test_motor_sail();
    void test_motor_rudder();
    int sail_control(int wind_direction);
    void set_angle_sail(int angle);
    float rudder_control(coord a, coord b, coord pos);
    void set_angle_rudder(int angle);
    float sawtooth(float x);
private:
    Adafruit_PWMServoDriver pwm;
    float angle_rudder;
    float angle_sail;
};

#endif 
