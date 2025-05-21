#include <Adafruit_PWMServoDriver.h>

#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

uint8_t servosail = 0;
uint8_t servorud = 1;

int elevationpin = 3;
int aileronpin = 2;
int elevation, aileron, com_sail, com_rud;


void setup() {
    Serial.begin(9600);
    pinMode(elevationpin, INPUT);
    pinMode(aileronpin, INPUT);
    pwm.begin();
    pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
}

void loop() {
  // put your main code here, to run repeatedly:
  elevation = pulseIn(elevationpin, HIGH);
  aileron = pulseIn(aileronpin, HIGH);
  Serial.print("Elevation:");
  Serial.println(elevation);
  Serial.print("Aileron:");
  Serial.println(aileron);
  com_sail = map(elevation, 1800, 910, SERVOMIN, SERVOMAX);
  com_rud = map(aileron, 985, 2155, SERVOMIN, SERVOMAX);
  pwm.setPWM(servosail, 0, com_sail);
  pwm.setPWM(servorud, 0, com_rud);
}
