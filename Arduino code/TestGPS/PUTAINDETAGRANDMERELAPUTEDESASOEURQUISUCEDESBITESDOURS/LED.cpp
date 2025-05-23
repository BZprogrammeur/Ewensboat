#include "LED.h"
#include <Arduino.h>

LED::LED(int pin) {
  _pin = pin;
  pinMode(_pin, OUTPUT);
}

void LED::on() {
  digitalWrite(_pin, HIGH);
}

void LED::off() {
  digitalWrite(_pin, LOW);
}
