#include "controler.h"

Controler::Controler() : pwm(Adafruit_PWMServoDriver()) {
}

void Controler::init() {
    pinMode(elevationPin, INPUT);
    pinMode(aileronPin, INPUT);
    pinMode(controlPin, INPUT);
    pwm.begin();
    pwm.setPWMFreq(50); 
}

void Controler::controling() {
    elevation = pulseIn(elevationPin, HIGH);
    aileron = pulseIn(aileronPin, HIGH);

    comSail = map(elevation, 1800, 910, SERVOMIN_SAIL, SERVOMAX_SAIL);
    comRud = map(aileron, 985, 2155, SERVOMIN_RUDDER, SERVOMAX_RUDDER);

    pwm.setPWM(SERVO_SAIL, 0, comSail);
    pwm.setPWM(SERVO_RUDDER, 0, comRud);
}

void Controler::setUnmanned() {
    controlValue = pulseIn(controlPin, HIGH);

    if (controlValue > 1700) {
        compteur++;
        Serial.println(compteur);
    } else {
        compteur = 0;
    }

    if (compteur > 60) {
        compteur = 0;
        unmanned = !unmanned;

        if (unmanned) {
            Serial.println("Mode autonome activé");
        } else {
            Serial.println("Prise de contrôle");
        }
    }
}
