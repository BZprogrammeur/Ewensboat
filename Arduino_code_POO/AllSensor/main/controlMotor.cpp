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

int controlMotor::get_com_rud(){
  return com_rud;
}

int controlMotor::get_com_sail(){
  return com_sail;
}

void controlMotor::set_angle_sail(int angle)
{
  com_sail = map(angle, 0, 90, SERVOMIN_SAIL, SERVOMAX_SAIL);
  //Serial.print("Sail command:");
  //Serial.println(com_sail);
  pwm.setPWM(SERVO_SAIL, 0, com_sail);
}

void controlMotor::set_angle_rudder(int angle)
{
  com_rud = map(angle, -50, 50, SERVOMAX_RUDDER, SERVOMIN_RUDDER);
  pwm.setPWM(SERVO_RUDDER, 0, com_rud);
}

void controlMotor::send_com_rudder(int com){
  pwm.setPWM(SERVO_RUDDER, 0, com);
}

void controlMotor::send_com_sail(int com){
  pwm.setPWM(SERVO_SAIL, 0, com);
}