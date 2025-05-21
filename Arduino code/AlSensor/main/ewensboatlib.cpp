#include <Arduino.h>
#include <Wire.h>
#include "ewensboatlib.h"

#define SERVOMIN_RUDDER  250 // Minimum pulse length
#define SERVOMAX_RUDDER  430 //somme des deux doit faire 580

// Constantes PD en DEGRÉS
const float Kp = 2.0;     // Gain proportionnel (à ajuster)
const float Kd = 1.0;     // Gain dérivé (à ajuster)
const float DELTA_T = 0.1; // Temps entre deux appels (en secondes)

float erreur_precedente = 0;

void follow_cap(float cap_a_suivre) {
  float cap_actuel = get_cap(); // Renvoie un cap entre 0 et 360

  // Calcul de l’erreur dans [-180, +180] degrés
  float erreur = cap_a_suivre - cap_actuel;
  if (erreur > 180) erreur -= 360;
  if (erreur < -180) erreur += 360;

  // Dérivée
  float derivee = (erreur - erreur_precedente) / DELTA_T;

  // Commande PD (inversée : angle positif = virage à gauche)
  float commande = -Kp * erreur - Kd * derivee;

  // Saturation à l’amplitude max du gouvernail
  if (commande > SERVOMAX_RUDDER) commande = SERVOMAX_RUDDER;
  if (commande < -SERVOMAX_RUDDER) commande = -SERVOMAX_RUDDER;

  set_angle_rudder((int)commande);

  erreur_precedente = erreur;
}
