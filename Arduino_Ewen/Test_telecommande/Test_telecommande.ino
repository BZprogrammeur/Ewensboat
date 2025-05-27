#include <Adafruit_PWMServoDriver.h>

#define SERVOMIN_SAIL  230 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX_SAIL  420 // this is the 'maximum' pulse length count (out of 4096)
#define SERVOMIN_RUDDER 150
#define SERVOMAX_RUDDER 600

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

uint8_t servosail = 0;
uint8_t servorud = 1;

int controlpin = 23;
int elevationpin = 3;
int aileronpin = 2;
int elevation, aileron, com_sail, com_rud, control;

bool unmaned = false;
int compteur = 0;

void setup() {
    Serial.begin(9600);
    pinMode(elevationpin, INPUT);
    pinMode(aileronpin, INPUT);
    pinMode(controlpin, INPUT);
    pwm.begin();
    pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
}

void loop() {
  control = pulseIn(controlpin, HIGH);
  if (control > 1700){
    compteur++;
    Serial.println(compteur);
  }
  else {
    compteur = 0;
  }
  if (compteur > 60){
    compteur = 0;
    if (unmaned){Serial.println("prise de controle");}
    else {Serial.println("Mode autonome activé");}
    unmaned = !unmaned;
    }
  if (!unmaned){
    elevation = pulseIn(elevationpin, HIGH);
    aileron = pulseIn(aileronpin, HIGH);
//    Serial.print("Elevation:");
//    Serial.println(elevation);
//    Serial.print("Aileron:");
//    Serial.println(aileron);
//    Serial.print("Control:");
//    Serial.println(control);
    com_sail = map(elevation, 1800, 910, SERVOMIN_SAIL, SERVOMAX_SAIL);
    com_rud = map(aileron, 985, 2155, SERVOMIN_RUDDER, SERVOMAX_RUDDER);
    pwm.setPWM(servosail, 0, com_sail);
    pwm.setPWM(servorud, 0, com_rud);
  }
}
