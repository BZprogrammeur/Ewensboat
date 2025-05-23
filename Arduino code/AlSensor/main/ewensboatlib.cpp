#include <Arduino.h>
#include <Wire.h>
#include "ewensboatlib.h"

nav::nav(){}

void nav::follow_cap(float cap_a_suivre) {
  float cap_actuel = imu.get_cap(); // Renvoie un cap entre 0 et 360

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

  powerboard.set_angle_rudder((int)commande);

  erreur_precedente = erreur;
}
