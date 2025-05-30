#include <Arduino.h>
#include <Wire.h>
#include "ewensboatlib.h"

Navigation::Navigation(IMU& imu_, controlMotor& motor_, WindSensor& wind_, GPS& gps_)
    : imu(imu_), powerboard(motor_), wind(wind_), gps(gps_){
  sens = false;
  isTacking = false;
  }

void Navigation::follow_cap(float cap_a_suivre) {
  imu.update();
  CheckTacking();
  float cap_actuel = imu.get_cap(); // Renvoie un cap entre 0 et 360
  marge = 0.0;

  if (isTacking){
    count4tacking();
    if (sens){
      marge = 30.0;      // gestion du tacking
    }
    else {
      marge = -30.0;
    }
  }
  cap_a_suivre += marge;
  Serial.println(cap_a_suivre);
  // Calcul de l’erreur dans [-180, +180] degrés
  float erreur = cap_a_suivre - cap_actuel;
  if (erreur > 180) erreur -= 360;
  if (erreur < -180) erreur += 360;

  // Dérivée
  float derivee = (erreur - erreur_precedente) / DELTA_T;

  // Commande PD (inversée : angle positif = virage à gauche)
  float commande = -Kp * erreur - Kd * derivee;

  // Saturation à l’amplitude max du gouvernail
  if (commande > 30) commande = 30;
  if (commande < -30) commande = -30;

  powerboard.set_angle_rudder((int)commande);

  erreur_precedente = erreur;
}

void Navigation::reach_point(GPScoord point) {
  gps.update();  // Met à jour les données GPS

  // Conversion du point cible
  Cartcoord target_cart = gps.conversion(point);
  double x_target = target_cart.x;
  double y_target = target_cart.y;

  // Position actuelle
  GPScoord pos = {gps.getLatitude(), gps.getLongitude()};
  Cartcoord current_cart = gps.conversion(pos);
  double x_pos = current_cart.x;
  double y_pos = current_cart.y;

  // Différence de position
  double dx = x_target - x_pos;
  double dy = y_target - y_pos;

  // Cap vers le point (angle en radians)
  float cap = atan2(dy, dx)*180/M_PI;

  // Fonction qui fait suivre le cap
  follow_cap(cap);
}

void Navigation::stopSailing(){
  powerboard.set_angle_sail(SERVOMAX_SAIL);
  powerboard.set_angle_rudder(0);
}

void Navigation::count4tacking() {
  if (!tackingMode) {
    tackingStart = millis();
    tackingMode = true;
  }

  if (millis() - tackingStart > 10000) {
    tackingMode = false;
    sens = !sens;
  } 
}


void Navigation::CheckTacking() {
  float windDirection = wind.get_wind_direction();
  isTacking = (windDirection > 345 || windDirection < 15);
}

bool Navigation::getTacking(){
  return isTacking;
}
