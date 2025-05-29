#include <Arduino.h>
#include <Wire.h>
#include "windSensor.h"

WindSensor::WindSensor() {
    pulseCount = 0;
    lastTime = 0;
    heading_bytes = 0;
    heading = 0.0;
    windSpeed = 0.0;  
}
void WindSensor::countPulse() {
    pulseCount++;  // Incrémente à chaque impulsion
    Serial.println(pulseCount);
}

WindSensor* windSensorInstance = nullptr; // Pointeur vers l'objet

void countPulse_ISR() {
    if (windSensorInstance) {
      Serial.println("!!!");
        windSensorInstance->countPulse();
    }
}

void WindSensor::init() {
    windSensorInstance = this;  // lie l'objet courant à l'ISR
    pinMode(19, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(19), countPulse_ISR, RISING);
    Serial.println("Initialisation du capteur de vent...");
}

void WindSensor::update() {
    unsigned long currentTime = millis();
    if (currentTime - lastTime >= 2250) {  // Calcul toutes les secondes
        windSpeed = pulseCount * 0.44704;  // Facteur de conversion (à ajuster selon le capteur);
        heading_bytes = analogRead(A15);
        heading = (float)heading_bytes / 1023 * 360;
        pulseCount = 0;  // Réinitialisation du compteur
        lastTime = currentTime;
    }
}

float WindSensor::get_wind_direction() const{
  return heading;
}

float WindSensor::get_wind_speed() const{
  return windSpeed;
}
