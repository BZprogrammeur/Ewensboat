#include <TimerOne.h>
#include "windSensor.h"

WindSensor* windSensorInstance = nullptr; // Pointeur vers l'objet

static void countPulse_ISR(void) {
    //Serial.println("Spotted an interrupt...");
    if (windSensorInstance) {
        noInterrupts(); // Désactive les interruptions pour éviter les erreurs
        windSensorInstance->countPulse();
        interrupts(); // Réactive les interruptions
    }
}

static void update_speedISR(void){
    //Serial.println("Updating wind data...");
    if (windSensorInstance) {
        noInterrupts(); // Désactive les interruptions pour éviter les erreurs
        windSensorInstance->update_speed();
        interrupts(); // Réactive les interruptions
    }
}

WindSensor::WindSensor() {
    Serial.println("Initializing wind sensor...");
    pulseCount = 0;
    heading_bytes = 0;
    heading = 0.0;
    windSpeed = 0.0;
    windSensorInstance = this;  // lie l'objet courant à l'ISR
    pinMode(19, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(19), countPulse_ISR, RISING);
    Timer1.initialize(2250000);
    Timer1.attachInterrupt(update_speedISR);  
}
void WindSensor::countPulse() {
    //Serial.println("Treating the interruption...");
    pulseCount++;  // Incrémente à chaque impulsion
    //Serial.print("Pule count:");
    //Serial.println(pulseCount);

}

void WindSensor::update_speed() {
        windSpeed = pulseCount * 0.44704;  // Facteur de conversion (à ajuster selon le capteur)
        // Serial.print("Wind speed: ");
        // Serial.print(windSpeed);
        // Serial.println(" m/s");
        pulseCount = 0;  // Réinitialisation du compteur
    }

void WindSensor::update_heading(){
  heading_bytes = analogRead(A15);
  heading = -2 * atan(tan(((float)heading_bytes / 1023 * 2 * M_PI + M_PI)/2)) * 180/M_PI;
  // if(heading>=0){
  //   heading  = 180 - heading;
  // }
  // else{
  //   heading = -180 - heading;
  // }
  // Serial.print("Apparent wind heading:");
  // Serial.print(heading);
  // Serial.println("°");
}

float WindSensor::get_wind_direction() {
  return heading;
}

float WindSensor::get_wind_speed() {
  return windSpeed;
}
