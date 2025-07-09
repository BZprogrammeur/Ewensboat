#include "controlMotor.h"

controlMotor::controlMotor() : pwm(Adafruit_PWMServoDriver()) {
  angle_rudder = 0;
  angle_sail = 0;
  Serial.println("Initialising Motors...");
  pwm.begin();
  Serial.println("pwm ready...");
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

int controlMotor::get_com_rud(){
  return com_rud;
}

int controlMotor::get_com_sail(){
  return com_sail;
}

void controlMotor::set_angle_sail(int angle)
{
  float pulse = 0;
  pulse = map(angle, 0, 90, SERVOMIN_SAIL, SERVOMAX_SAIL);
  com_sail = pulse;
  //Serial.print("Sail command:");
  //Serial.println(pulse);
  pwm.setPWM(SERVO_SAIL, 0, pulse);
}

void controlMotor::set_angle_rudder(int angle)
{
  float pulse = 0;
  pulse = map(angle, -50, 50, SERVOMIN_RUDDER, SERVOMAX_RUDDER);
  com_rud = pulse;
  pwm.setPWM(SERVO_RUDDER, 0, pulse);
}

void controlMotor::send_com_rudder(int com){
  pwm.setPWM(SERVO_RUDDER, 0, com);
}

void controlMotor::send_com_sail(int com){
  pwm.setPWM(SERVO_SAIL, 0, com);
}