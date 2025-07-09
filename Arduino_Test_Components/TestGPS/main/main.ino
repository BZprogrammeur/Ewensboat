#include "GPS.h"

GPS gps(Serial2);

void setup() {
    Serial.begin(9600);
    gps.init();
}

void loop() {
    gps.update();

    if (gps.isValid()) {
        Serial.print("Latitude : "); Serial.println(gps.getLatitude(), 6);
        Serial.print("Longitude : "); Serial.println(gps.getLongitude(), 6);
    } else {
        Serial.println("Fix non valide");
    }

    delay(1000);
}
