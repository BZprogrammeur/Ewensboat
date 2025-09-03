#include <Arduino.h>
#include <Wire.h>
#include "ewensboatlib.h"

Navigation::Navigation(IMU& imu_, controlMotor& motor_, WindSensor& wind_, GPS2& gps_, SDcard& sd_)
    : imu(imu_), powerboard(motor_), wind(wind_), gps(gps_), sd(sd_){
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
//  Serial.print("erreur :");
//  Serial.println(erreur);

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
//  Serial.print("cible : ");
//  Serial.print(x_target);
//  Serial.print(",");
//  Serial.print(y_target);
//  Serial.print("   ");
  

  // Position actuelle
  GPScoord pos = {gps.getLatitude(), gps.getLongitude()};
  Cartcoord current_cart = gps.conversion(pos);
  double x_pos = current_cart.x;
  double y_pos = current_cart.y;

//  Serial.print("nous : ");
//  Serial.print(x_pos);
//  Serial.print(",");
//  Serial.print(y_pos);
//  Serial.print("   ");

  // Différence de position
  double dx = x_target - x_pos;
  double dy = y_target - y_pos;

  // Cap vers le point (angle en radians)
  float cap = atan2(dx, dy)*180/M_PI;
  Serial.print("  cap cible :");
  Serial.print(cap);
  sd.save_capcible(cap);

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
    if (norm == 0) return; 

    float nx = dx / norm;
    float ny = dy / norm;
    float cap = atan2(dx, dy)*180/M_PI;
    cap += 180;
    if (cap > 180) cap -= 360;
    if (cap < -180) cap += 360;

    Serial.print("   ligne : ");
    Serial.print(nx);
    Serial.print(",");
    Serial.print(ny);
    Serial.print("    orientation : ");
    Serial.print(cap);
    Serial.print("    ");

    // Erreur de distance perpendiculaire à la ligne
    float error = nx * (pos.y - a.y) - ny * (pos.x - a.x);
    Serial.print("distance à la ligne :");
    Serial.print(error);
    
    // Commande proportionnelle
    float commande = 90*(error/(20+abs(error)));

    // Saturation de la commande
    if (commande > 90) commande = 90;
    if (commande < -90) commande = -90;
//    Serial.print("     com :");
//    Serial.println(commande);

    float cap_final = cap - commande;
    Serial.print("     cap suivi :");
    Serial.print(cap_final);
    follow_cap(cap_final);
}

void Navigation::follow_path(GPScoord chemin[], int taille){
  GPScoord depart;
  GPScoord arrivee;
  
  Serial.print("flag : ");
  Serial.print(i);
  depart = chemin[i];
  arrivee = chemin[i+1];
  GPScoord pos_actuelle = {gps.getLatitude(), gps.getLongitude()};
  line_following(depart, arrivee);
  if (check_radius(10.0, arrivee, pos_actuelle)){
    i++;
    i = i%(taille-1);
  }
}

void Navigation::follow_path2(GPScoord chemin[], int taille){
  GPScoord depart;
  GPScoord arrivee;

  Serial.print("flag : ");
  Serial.print(i);
  sd.save_flag(i);
  GPScoord next_point = chemin[i];
  GPScoord pos_actuelle = {gps.getLatitude(), gps.getLongitude()};
  reach_point(next_point);
  if (check_radius(10.0, next_point, pos_actuelle)){
    i++;
    i = i%taille;
  }
}

bool Navigation::check_radius(float r, GPScoord centre, GPScoord pos) {
  Cartcoord centre_conv = gps.conversion(centre);
  Cartcoord pos_conv = gps.conversion(pos);

  float dx = pos_conv.x - centre_conv.x;
  float dy = pos_conv.y - centre_conv.y;
  float distance = sqrt(sq(dx) + sq(dy));

  Serial.print("  distance: ");
  Serial.println(distance);

  return (distance < r);
}

void Navigation::set_sail_pos(){
  float wind_angle = wind.get_wind_direction();
  if (wind_angle>180)
  {
    wind_angle = 360-wind_angle;
  }
  float sail_angle = map(abs(wind_angle), 0, 180, 0, 90);
  powerboard.set_angle_sail(sail_angle);  
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
