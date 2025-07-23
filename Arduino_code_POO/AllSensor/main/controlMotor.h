#ifndef CONTROLMOTOR_H
#define CONTROLMOTOR_H

#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>
#include "config.h"

class controlMotor {
public:
  controlMotor();
  int sail_control(int wind_direction);
  void set_angle_sail(int angle);
  void set_angle_rudder(int angle);
  int get_com_rud();
  int get_com_sail();
  void send_com_rudder(int com);
  void send_com_sail(int com);
private:
  Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
  float angle_rudder;
  float angle_sail;
  int com_sail = (int)((SERVOMIN_SAIL + SERVOMAX_SAIL) / 2);
  int com_rud = (int)((SERVOMIN_RUDDER + SERVOMAX_RUDDER) / 2);
};

#endif 
