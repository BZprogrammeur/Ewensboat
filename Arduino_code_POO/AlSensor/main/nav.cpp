#include "nav.h"

// TODO : Log management function

float sawtooth(float x){
  return 2*atan(tan(x/2));
}

nav::nav() : Kp(2.0), Kd(1.0), DELTA_T(0.1) {
  Serial.println("Initialising sailboat...");
  powerboard = new controlMotor();
  Serial.println("Motors ready.");
  init_sequence_rud();
  imu = new IMU();
  Serial.println("IMU ready.");
  init_sequence_rud();
  wind = new WindSensor();
  Serial.println("Wind sensor ready.");
  gps = new GPS(Serial2);
  Serial.println("GPS ready.");
  controler = new Controler();
  Serial.println("Controler ready.");
  if (!SD.begin(PIN_SPI_CS)) {
    Serial.println("SD CARD FAILED, OR NOT PRESENT!");
    while (1); // don't do anything more:
  }
  Serial.println("SD CARD INITIALIZED.");
  sprintf(filename, "NAVLOG%d.TXT", getMaxLogIndex() + 1);
  Serial.print("Find log in file:");
  Serial.println(filename);
}

void nav::init_sequence_rud(){
    for(int i = SERVOMIN_RUDDER; i <= SERVOMAX_RUDDER; i++){
    powerboard->send_com_rudder(i);
    delay(1);
  }
  for(int i = SERVOMAX_RUDDER; i >= SERVOMIN_RUDDER; i--){
    powerboard->send_com_rudder(i);
    delay(1);
  }
  for(int i = SERVOMIN_RUDDER; i <= SERVOMAX_RUDDER; i++){
    powerboard->send_com_rudder(i);
    delay(1);
  }
    for(int i = SERVOMAX_RUDDER; i >= SERVOMIN_RUDDER; i--){
    powerboard->send_com_rudder(i);
    delay(1);
  }
  return;
}

void nav::update_logs(){
  File logfile = SD.open(filename, FILE_WRITE);
  logfile.print(millis());
  logfile.print(' ');
  GPScoord pos_gps = gps->getPoint();
  logfile.print(pos_gps.lat);
  logfile.print(' ');
  logfile.print(pos_gps.lng);
  logfile.print(' ');
  logfile.print(imu->get_heading());
  logfile.print(' ');
  logfile.print(wind->get_wind_direction());
  logfile.print(' ');
  logfile.print(wind->get_wind_speed());
  logfile.print(' ');
  logfile.print(powerboard->get_com_rud());
  logfile.print(' ');
  logfile.print(powerboard->get_com_sail());
  logfile.print(' ');
  logfile.println(controler->isUnmanned());
  logfile.close();
  //TODO : adapt controler class to finish the log function
  return;
}

nav::~nav() {
    delete imu;
    delete powerboard;
    delete wind;
}

void nav::update(){
  imu->update();
  wind->update_heading();
  gps->update();
  update_logs();
  return;
}

void nav::follow_cap(float cap_a_suivre) {
  update();
  if(controler->isUnmanned()){
  float cap_actuel = imu->get_heading(); // Renvoie un cap entre -180 et 180 degrees
  // Calcul de l’erreur dans [-180, +180] degrés
  float erreur = cap_a_suivre - cap_actuel;
  if (erreur > 50.) erreur = 50.;
  if (erreur < -50.) erreur = -50.;
  //Serial.print("Erreur calculée:  ");
  //Serial.println(erreur);
  powerboard->set_angle_rudder((int)erreur);
  erreur_precedente = erreur;
  }
  else{
    powerboard->send_com_rudder(controler->get_com_rudder());
    powerboard->send_com_sail(controler->get_com_sail());
  }
  return;
}

void nav::set_sail_pos(){
  float wind_angle = wind->get_wind_direction();
  //Serial.print("Angle du vent mesuré:");
  //Serial.println(wind_angle);
  float sail_angle;
  if(abs(wind_angle) <= 50){
    sail_angle = 0.;
  }
  else{
    sail_angle = map(abs(wind_angle), 50, 180, 0, 90);
  }
  //Serial.print("Angle calculé de la voile:");
  //Serial.println(sail_angle);
  powerboard->set_angle_sail(sail_angle);  
  return;
}

void nav::linefollowing(float lata, float longa, float latb, float longb){
  update();
  if(controler->isUnmanned()){
    update();
    // from the Matlab simulation coded by Pr. Jian Wan
    // % a --- the starting point;
    // % b --- the ending point;
    float heading = imu->get_heading() * PI / 180;
    // % r --- the cutoff distance;
    float r = 3.;
    // % q --- the tacking variable;
    float q = 1;
    // % gamma --- the incidence angle;
    float gamma = PI / 4;
    // % phi --- the close hauled angle;
    float phi = PI / 3;
    // % angle_ruddermax --- the maximum rudder angle;
    float angle_ruddermax = 50;
    // % angle_truewind --- the true wind direction;
    float angle_truewind = sawtooth(wind->get_wind_direction() * PI / 180 - heading);

    GPScoord pos_gps = gps->getPoint();
    Cartcoord m = gps->conversion(pos_gps);
    GPScoord agps;
    agps.lat = lata;
    agps.lng = longa;
    GPScoord bgps;
    bgps.lat = latb;
    bgps.lng = longb;
    Cartcoord a = gps->conversion(agps);
    Cartcoord b = gps->conversion(bgps);
    Cartcoord ab;
    ab.x = b.x - a.x;
    ab.y = b.y - a.y;
    Cartcoord c;
    c.x = ab.x / sqrt(pow(ab.x, 2) + pow(ab.y, 2));
    c.y = ab.y / sqrt(pow(ab.x, 2) + pow(ab.y, 2));
    Cartcoord d;
    d.x = m.x - a.x;
    d.y = m.y - a.y;
    float e = c.x * d.y - d.x * c.y;
    if(abs(e) > r/2&& e < 0){
        q = -1;
    }
    // TODO : Clarify the part above about q value.
    float angle_target=atan2(ab.y,ab.x);
    float angle_nominal=angle_target-2*gamma*atan(e/r)/PI;

    float angle_actual;
    if(cos(angle_truewind-angle_nominal)+cos(phi) < 0 || (abs(e) < r && cos(angle_truewind-angle_target)+cos(phi) < 0)){
      angle_actual=PI + angle_truewind - q * phi;
    }
    else{
      angle_actual=angle_nominal;
    }
    float angle_rudder;
    if (cos(heading-angle_actual) >= 0){
      angle_rudder=angle_ruddermax*sin(heading-angle_actual);
    }
    else{
      angle_rudder = angle_ruddermax * sin(heading-angle_actual) / abs(sin(heading - angle_actual));
    }
    powerboard->set_angle_rudder(angle_rudder);
    set_sail_pos();
  }
  else{
    powerboard->send_com_rudder(controler->get_com_rudder());
    powerboard->send_com_sail(controler->get_com_sail());
  }
  return;
  }

int getMaxLogIndex() {
  // Function created by Copilot AI
  int maxIndex = -1;
  File root = SD.open("/");

  while (true) {
    File entry = root.openNextFile();
    if (!entry) {
      // Plus de fichiers
      break;
    }

    String filename = entry.name();
    entry.close();

    if (filename.startsWith("NAVLOG") && filename.endsWith(".TXT")) {
      // Extraire la partie numérique
      String numberPart = filename.substring(6, filename.length() - 4);
      int index = numberPart.toInt();
      if (index > maxIndex) {
        maxIndex = index;
      }
    }
  }
  return maxIndex;
}