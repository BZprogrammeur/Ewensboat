#include "nav.h"
#include "controler.h"

nav* navigator;
Controler* controler;

void setup() {
    Serial.begin(9600);
    Serial.println("Starting main...");
    delay(500);
    navigator = new nav();
}

void loop() {
  navigator->update();
}
