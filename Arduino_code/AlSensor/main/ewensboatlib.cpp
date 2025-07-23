#include <Arduino.h>
#include <Wire.h>
#include "ewensboatlib.h"
#include "windSensor.h"

#define SERVOMIN_RUDDER  230 
#define SERVOMAX_RUDDER  490 

// Constantes PD en DEGRÉS
const float Kp = 2.0;     // Gain proportionnel (à ajuster)
const float Kd = 1.0;     // Gain dérivé (à ajuster)
const float DELTA_T = 0.1; // Temps entre deux appels (en secondes)

float erreur_precedente = 0;

void follow_cap(float cap_a_suivre) {
  float cap_actuel = get_cap(); // Renvoie un cap entre 0 et 360
  // Calcul de l’erreur dans [-180, +180] degrés
  float erreur =cap_actuel -  cap_a_suivre;
  // Saturation à l’amplitude max du gouvernail
  if (erreur > 50) erreur = 50;
  if (erreur < -50) erreur = -50;
  //Serial.print("Erreur calculée:  ");
  //Serial.println(erreur);
  set_angle_rudder(erreur);

  erreur_precedente = erreur;
}

void set_sail_pos(){
  float wind_angle = get_wind_direction();
  float sail_angle = map(abs(wind_angle), 0, 180, 0, 90);
  //Serial.print("Angle calculé de la voile:");
  //Serial.println(sail_angle);
  set_angle_sail(sail_angle);  
}