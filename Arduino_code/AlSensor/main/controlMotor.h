#ifndef CONTROLMOTOR_H
#define CONTROLMOTOR_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

struct coord {
    float x;
    float y;
};


void motor_init();
void test_motor_sail();
void test_motor_rudder();
int sail_control(int wind_direction);
void set_angle_sail(int angle);
float rudder_control(coord a, coord b, coord pos);
void set_angle_rudder(int angle);
float sawtooth(float x);

#endif 
