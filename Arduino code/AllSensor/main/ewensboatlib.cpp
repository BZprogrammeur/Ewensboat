#include <Arduino.h>
#include <Wire.h>
#include "ewensboatlib.h"

Navigation::Navigation(IMU& imu_, controlMotor& motor_, WindSensor& wind_, GPS2& gps_)
    : imu(imu_), powerboard(motor_), wind(wind_), gps(gps_){
  sens = false;
  isTacking = false;
  }

void Navigation::follow_cap(float cap_a_suivre) {
  CheckTacking();
  float cap_actuel = imu.get_cap(); // Renvoie un cap entre 0 et 360
  marge = 0.0;

  if (isTacking || tackingMode){
    count4tacking();
    if (sens){
      marge = 30.0;      // gestion du tacking
    }
    else {
      marge = -30.0;
    }
  }
  cap_a_suivre += marge;
  // Calcul de l’erreur dans [-180, +180] degrés
  float erreur = cap_a_suivre - cap_actuel;
  if (erreur > 180) erreur -= 360;
  if (erreur < -180) erreur += 360;

  // Dérivée
  float derivee = (erreur - erreur_precedente) / DELTA_T;

  // Commande PD (inversée : angle positif = virage à gauche)
  float commande = -Kp * erreur; //- Kd * derivee;

  // Saturation à l’amplitude max du gouvernail
  if (commande > 30) commande = 30;
  if (commande < -30) commande = -30;

  powerboard.set_angle_rudder((int)commande);
  angle_rudder = commande;

  erreur_precedente = erreur;
}

void Navigation::reach_point(GPScoord point) {
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
  angle_rudder = 0;
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
void Navigation::line_following(GPScoord arrival, GPScoord startline)
{
    // Convertir les points GPS en coordonnées cartésiennes
    Cartcoord a = gps.conversion(startline);
    Cartcoord b = gps.conversion(arrival);
    GPScoord posGPS = gps.getPoint(); // Corrigé ici
    Cartcoord pos = gps.conversion(posGPS);

    // Calcul du vecteur directeur de la ligne normalisé
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    float norm = sqrt(dx * dx + dy * dy);
    if (norm == 0) return; // éviter une division par zéro

    float nx = dx / norm;
    float ny = dy / norm;

    // Erreur de distance perpendiculaire à la ligne
    float error = nx * (pos.y - a.y) - ny * (pos.x - a.x);

    CheckTacking();
    marge = 0.0;
  
    if (isTacking || tackingMode){
      count4tacking();
      if (sens){
        marge = 30.0;      // gestion du tacking
      }
      else {
        marge = -30.0;
      }
    }
    error += marge;
    
    // Commande proportionnelle
    float commande = Kp * error;

    // Saturation de la commande
    if (commande > 30) commande = 30;
    if (commande < -30) commande = -30;

    // Application au gouvernail
    powerboard.set_angle_rudder((int)commande);
    angle_rudder = commande; //pour les logs
}


void Navigation::CheckTacking() {
  float windDirection = wind.get_wind_direction();
  isTacking = (windDirection > 345 || windDirection < 15);
}

bool Navigation::getTacking(){
  return isTacking;
}

float Navigation::getRudderPos(){
  return angle_rudder;
}
