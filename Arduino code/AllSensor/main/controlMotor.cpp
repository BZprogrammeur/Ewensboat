#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "controlMotor.h"
#include "IMU.h"
#include "windSensor.h"

controlMotor::controlMotor() : pwm(Adafruit_PWMServoDriver()) {
  angle_rudder = 0;
  angle_sail = 0;
}

float controlMotor::sawtooth(float x)
{
    return 2*atan(tan(x/2));
}

void controlMotor::init() {
  Serial.begin(9600);
  Serial.println("Initialisation PCA9685...");

  pwm.begin();
  pwm.setPWMFreq(50);  // 50 Hz pour servos

  delay(10);
}

void controlMotor::test_motor_sail() {
  // Balayage d’un servo connecté sur le canal 0
  for (int pulselen = SERVOMIN_SAIL; pulselen <= SERVOMAX_SAIL; pulselen++) {
    pwm.setPWM(SERVO_SAIL, 0, pulselen);
    delay(5);
  }

  for (int pulselen = SERVOMAX_SAIL; pulselen >= SERVOMIN_SAIL; pulselen--) {
    pwm.setPWM(SERVO_SAIL, 0, pulselen);
    delay(5);
  }
}

void controlMotor::test_motor_rudder(){
  // Balayage d’un servo connecté sur le canal 0
  for (int pulselen = SERVOMIN_RUDDER; pulselen <= SERVOMAX_RUDDER; pulselen++) {
    pwm.setPWM(SERVO_RUDDER, 0, pulselen);
    delay(5);
  }

  for (int pulselen = SERVOMAX_SAIL; pulselen >= SERVOMIN_SAIL; pulselen--) {
    pwm.setPWM(SERVO_RUDDER, 0, pulselen);
    delay(5);
  }
}

int controlMotor::sail_control(int wind_direction) {
    int sail_angle = 0;

    if ((wind_direction >= 345 && wind_direction <= 360) || (wind_direction >= 0 && wind_direction <= 15)) {
        // Vent de face – zone interdite
        sail_angle = 0;
    }
    else if ((wind_direction > 15 && wind_direction <= 35) || (wind_direction >= 325 && wind_direction < 345)) {
        sail_angle = 20;
    }
    else if ((wind_direction > 35 && wind_direction <= 55) || (wind_direction >= 305 && wind_direction <= 325)) {
        sail_angle = 30;
    }
    else if ((wind_direction > 55 && wind_direction <= 75) || (wind_direction >= 285 && wind_direction <= 305)) {
        sail_angle = 40;
    }
    else if ((wind_direction > 75 && wind_direction <= 105) || (wind_direction >= 255 && wind_direction <= 285)) {
        // Vent de travers
        sail_angle = 45;
    }
    else if ((wind_direction > 105 && wind_direction <= 135) || (wind_direction >= 225 && wind_direction <= 255)) {
        sail_angle = 60;
    }
    else if ((wind_direction > 135 && wind_direction <= 165) || (wind_direction >= 195 && wind_direction <= 225)) {
        sail_angle = 70;
    }
    else if ((wind_direction > 165 && wind_direction < 195)) {
        // Vent arrière
        sail_angle = 90;
    }

    return sail_angle;
}

void controlMotor::set_angle_sail(int angle)
{
  float pulse = 0;
  pulse = map(angle, 0, 90, SERVOMIN_SAIL, SERVOMAX_SAIL);
  pwm.setPWM(SERVO_SAIL, 0, pulse);
}

void controlMotor::set_angle_rudder(int angle)
{
  float pulse = 0;
  pulse = map(angle, -30, 30, SERVOMIN_RUDDER, SERVOMAX_RUDDER);
  pwm.setPWM(SERVO_RUDDER, 0, pulse);
}
