#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN  150 // Minimum pulse length
#define SERVOMAX  600 // Maximum pulse length

void setup() {
  Serial.begin(9600);
  Serial.println("Initialisation PCA9685...");

  pwm.begin();
  pwm.setPWMFreq(50);  // 50 Hz pour servos

  delay(10);
}

void loop() {
  // Balayage d’un servo connecté sur le canal 0
  for (int pulselen = SERVOMIN; pulselen <= SERVOMAX; pulselen++) {
    pwm.setPWM(0, 0, pulselen);
    delay(5);
  }

  for (int pulselen = SERVOMAX; pulselen >= SERVOMIN; pulselen--) {
    pwm.setPWM(0, 0, pulselen);
    delay(5);
  }
}
