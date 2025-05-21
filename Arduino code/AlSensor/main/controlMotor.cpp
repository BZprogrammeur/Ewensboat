#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "controlMotor.h"
#include "IMU.h"
#include "windSensor.h"

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN_SAIL  200 // Minimum pulse length
#define SERVOMAX_SAIL  370 // Maximum pulse length
#define SERVOMIN_RUDDER  250 // Minimum pulse length
#define SERVOMAX_RUDDER  430 //somme des deux doit faire 580
#define R 12
#define GAMMA M_PI/4  ///< Incidence angle
#define ZETA M_PI/4

float sawtooth(float x)
{
    return 2*atan(tan(x/2));
}


void motor_init() {
  Serial.begin(9600);
  Serial.println("Initialisation PCA9685...");

  pwm.begin();
  pwm.setPWMFreq(50);  // 50 Hz pour servos

  delay(10);
}

void test_motor_sail() {
  // Balayage d’un servo connecté sur le canal 0
  for (int pulselen = SERVOMIN_SAIL; pulselen <= SERVOMAX_SAIL; pulselen++) {
    pwm.setPWM(0, 0, pulselen);
    delay(5);
  }

  for (int pulselen = SERVOMAX_SAIL; pulselen >= SERVOMIN_SAIL; pulselen--) {
    pwm.setPWM(0, 0, pulselen);
    delay(5);
  }
}

void test_motor_rudder(){
  // Balayage d’un servo connecté sur le canal 0
  for (int pulselen = SERVOMIN_RUDDER; pulselen <= SERVOMAX_RUDDER; pulselen++) {
    pwm.setPWM(1, 0, pulselen);
    delay(5);
  }

  for (int pulselen = SERVOMAX_SAIL; pulselen >= SERVOMIN_SAIL; pulselen--) {
    pwm.setPWM(1, 0, pulselen);
    delay(5);
  }
}

int sail_control(int wind_direction) {
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

void set_angle_sail(int angle)
{
  float pulse = 0;
  pulse = ((SERVOMAX_SAIL-SERVOMIN_SAIL)/90)*angle + SERVOMIN_SAIL;
  pwm.setPWM(0, 0, pulse);
}

float rudder_control(coord a, coord b, coord pos) {
    float a1 = a.x, a2 = a.y;
    float b1 = b.x, b2 = b.y;
    float x1 = pos.x, x2 = pos.y;

    float theta = get_cap(); // Cap actuel
    float wind_direction = get_wind_direction(); 

    float dx = b1 - a1;
    float dy = b2 - a2;
    float N = sqrt(dx * dx + dy * dy);

    float e = (dx*(x2 - a2) - dy*(x1 - a1))/N;

    int m_q = 1;
    if(abs(e) > R/2)
        m_q = e/abs(e);
    
    float angle_target = atan2(dy, dx);
    float angle_nominal = angle_target - 2*GAMMA*atan(e/R)/M_PI;
    float angle_actual = angle_nominal;

    if ((cos(wind_direction - angle_nominal) + cos(ZETA) < 0) ||
        (abs(e) < R && cos(wind_direction - angle_target) + cos(ZETA) < 0)) {
        angle_actual = M_PI + wind_direction - m_q * ZETA;
    }

    float angle_rudder = SERVOMAX_RUDDER/M_PI * sawtooth(theta - angle_actual);
    return angle_rudder;
}

void set_angle_rudder(int angle)
{
  float pulse = 0;
  pulse = ((SERVOMAX_RUDDER-SERVOMIN_RUDDER)/100)*angle + (SERVOMIN_RUDDER+SERVOMAX_RUDDER)/2;
  pwm.setPWM(1, 0, pulse);
}
