#include "LED.h"

LED led(13);

void setup() {
  led.on();
}

void loop() {
  led.off();
  delay(500);
  led.on();
  delay(500);
}
