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

uint8_t servonum = 1;
int command = 400;

void setup() {
  Serial.begin(9600);

  pwm.begin();

  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
}


void loop() {
  // Drive each servo one at a time
  if(Serial.available()>0){
    command = Serial.parseInt();
    Serial.print("Received command : ");
    Serial.println(command);
  }
  pwm.setPWM(servonum, 0, command);
  delay(500);
  
}