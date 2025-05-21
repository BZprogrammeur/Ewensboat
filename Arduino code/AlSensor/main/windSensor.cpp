#include <Arduino.h>
#include <Wire.h>
#include "windSensor.h"

int heading_bytes;
float heading;
volatile int pulseCount = 0;
unsigned long lastTime = 0;

void countPulse() {
    pulseCount++;  // Incrémente à chaque impulsion
}

void wind_sensor_init() {
    Serial.begin(9600);
    pinMode(19, INPUT_PULLUP);  // Utilisation de la pin 18 avec pull-up
    attachInterrupt(digitalPinToInterrupt(19), countPulse, RISING);  // Déclenche sur front montant
}

void get_wind_data() {
    unsigned long currentTime = millis();
    if (currentTime - lastTime >= 2250) {  // Calcul toutes les secondes
        float windSpeed = pulseCount * 0.44704;  // Facteur de conversion (à ajuster selon le capteur)
        Serial.print("Vitesse du vent: ");
        Serial.print(windSpeed);
        Serial.println(" m/s");
        heading_bytes = analogRead(A15);
        heading = (float)heading_bytes / 1023 * 360;
        Serial.print("Direction du vent:");
        Serial.print(heading);
        Serial.println("°");
        pulseCount = 0;  // Réinitialisation du compteur
        lastTime = currentTime;
    }
}

float get_wind_direction(){
  heading_bytes = analogRead(A15);
  heading = (float)heading_bytes / 1023 * 360;
  return heading;
}
