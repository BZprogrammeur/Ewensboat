/*
 * code from https://arduino.stackexchange.com/questions/4095/sample-code-for-using-adafruit-servo-shield
 * INSTALL FOLLOWING LIBRAIRIES TO RUN:
 *      https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library
 *      https://github.com/adafruit/Adafruit_BusIO
 * More information on https://learn.adafruit.com/adafruit-16-channel-pwm-slash-servo-shield/using-the-adafruit-library
 *
 * Once SERVOMIN and SERVOMAX are calibrated, use the following line to transform degrees into a pulselength:
 *  pulselength = map(degrees, 0, 180, SERVOMIN, SERVOMAX);
 */

#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)

uint8_t servonum = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("16 channel Servo test!");

  pwm.begin();

  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
}


void loop() {
  // Drive each servo one at a time
  Serial.println(servonum);
  Serial.println("Going from 0 to 180...");
  for (int pos = 0; pos <= 180; pos++) {
    pwm.setPWM(servonum, 0, map(pos, 0, 180, SERVOMIN, SERVOMAX));
    delay(10);
  }
  delay(500);
  Serial.println("Going from 180 to 0...");
  for (int pos = 180; pos >= 0; pos--) {
    pwm.setPWM(servonum, 0, map(pos, 0, 180, SERVOMIN, SERVOMAX));
    delay(10);
  }
  delay(500);
}