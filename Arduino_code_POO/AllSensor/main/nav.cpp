#include "nav.h"

double scalprod(Cartcoord A, Cartcoord B){
  return A.x * B.x + A.y * B.y;
}

Cartcoord diff(Cartcoord A, Cartcoord B){
  Cartcoord C;
  C.x = A.x - B.x;
  C.y = A.y- B.y;
  return C;
}

nav::nav() : Kp(2.0), Kd(1.0), DELTA_T(0.1) {
  Serial.println("Initialising sailboat...");
  powerboard = new controlMotor();
  Serial.println("Motors ready.");
  imu = new IMU();
  Serial.println("IMU ready.");
  init_sequence_rud();
  wind = new WindSensor();
  Serial.println("Wind sensor ready.");
  gps = new GPS();
  while(!gps->isValid()){
    Serial.println("Searching for GPS...");
    powerboard->set_angle_rudder(50 * cos(2 * PI *millis() / (5000)));
    gps->update();
  }
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
  logfile.print(gps->getLatitude(), 12);
  logfile.print(' ');
  logfile.print(gps->getLongitude(), 12);
  logfile.print(' ');
  logfile.print(imu->get_heading());
  logfile.print(' ');
  logfile.print(gps->getSOG());
  logfile.print(' ');
  logfile.print(wind->get_wind_direction());
  logfile.print(' ');
  logfile.print(wind->get_wind_speed());
  logfile.print(' ');
  logfile.print(powerboard->get_com_rud());
  logfile.print(' ');
  logfile.print(powerboard->get_com_sail());
  logfile.print(' ');
  logfile.println(controler->unmanned_status());
  logfile.close();
  //TODO : adapt controler class to finish the log function
  return;
}

nav::~nav() {
    delete imu;
    delete powerboard;
    delete wind;
    delete gps;
    delete controler;
}

void nav::update(){
  imu->update();
  wind->update_heading();
  // Serial.println(wind->get_wind_direction());
  // wind speed automatically updates every 2.25 seconds.
  gps->update();
  update_logs();
  return;
}

void nav::follow_cap(float cap_a_suivre) {
  update();
  if(controler->checkUnmanned()){
  float cap_actuel = imu->get_heading(); // Renvoie un cap entre -180 et 180 degrees
  // Calcul de l’erreur dans [-180, +180] degrés
  float erreur = cap_a_suivre - cap_actuel;
  if (erreur > 50.) erreur = 50.;
  if (erreur < -50.) erreur = -50.;
  //Serial.print("Erreur calculée:  ");
  //Serial.println(erreur);
  powerboard->set_angle_rudder(-(int)erreur);
  erreur_precedente = erreur;
  set_sail_pos();
  }
  else{
    powerboard->send_com_rudder(controler->get_com_rudder());
    powerboard->send_com_sail(controler->get_com_sail());
  }
  delay(100);
  return;
}

void nav::set_sail_pos(){
  float wind_angle = wind->get_wind_direction();
  //Serial.print("Angle du vent mesuré:");
  //Serial.println(wind_angle);
  float sail_angle;
  if(abs(wind_angle) <= 50){
    sail_angle = 90.;
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
  if(controler->checkUnmanned()){
    // from the Matlab simulation coded by Pr. Jian Wan
    // % a --- the starting point;
    // % b --- the ending point;
    float heading = sawtooth(imu->get_heading() * PI / 180);
    // % r --- the cutoff distance;
    float r = 6.; //Short distance such as 3 meters will hopefully allow the boat to navigate in a narrow canal.
    // % q --- the tacking variable;
    float q = 1;
    // % gamma --- the incidence angle;
    float gamma = PI / 4;
    // % phi --- the close hauled angle;
    float phi = PI / 3;
    // % angle_ruddermax --- the maximum rudder angle;
    float angle_ruddermax = 50;
    // % angle_truewind --- the true wind direction;
    float angle_truewind = get_true_wind_dir();

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
    if(abs(e) > r/2){
        q = e / abs(e);
    }
    // TODO : Clarify the part above about q value.
    float angle_target = sawtooth(atan2(ab.y,ab.x) - (PI/2));
    float angle_nominal = sawtooth(angle_target-2*gamma*atan(e/r)/PI);

    float aimed_angle;
    if((cos(angle_truewind-angle_nominal)+cos(phi) < 0) || ((abs(e) < r) && ((cos(angle_truewind-angle_target)+cos(phi)) < 0))){
      aimed_angle= sawtooth(PI + angle_truewind - q * phi);
    }
    else{
      aimed_angle=angle_nominal;
    }
    // Serial.print("Target angle:");
    // Serial.println(aimed_angle);
    float angle_rudder;
    angle_rudder = min(max(-angle_ruddermax, angle_ruddermax*sin(sawtooth(heading-aimed_angle)/2)), angle_ruddermax);
    // Serial.print("Angle rudder:");
    // Serial.println(angle_rudder);
    powerboard->set_angle_rudder(angle_rudder);
    set_sail_pos();
  }
  else{
    powerboard->send_com_rudder(controler->get_com_rudder());
    powerboard->send_com_sail(controler->get_com_sail());
  }
  delay(100);
  return;
  }

float nav::get_true_wind_dir(){
  // According to previous internship reports and : https://www.bwsailing.com/cc/2017/05/calculating-the-true-wind-and-why-it-matters/
  float SOG = gps->getSOG();
  // Serial.print("SOG: ");
  // Serial.println(SOG);
  float COG = imu->get_heading() * PI / 180; // technically false but good enough approximation as the GPS is very unprecise and the boat doesn't drift much.
  float AWS = wind->get_wind_speed() ;
  float AWD = sawtooth((wind->get_wind_direction() + imu->get_heading()) * PI / 180);

  float u = SOG * sin(COG) - AWS * sin(AWD);
  float v = SOG * cos(COG) - AWS * cos(AWD);
  return sawtooth(atan2(v, u) - (PI / 2));
}

int getMaxLogIndex() {
  // Function created by Copilot AI
  int maxIndex = -1;
  File root = SD.open("/");

  while (true) {
    File entry = root.openNextFile();
    if (!entry) {
      break;
    }

    String filename = entry.name();
    entry.close();

    if (filename.startsWith("NAVLOG") && filename.endsWith(".TXT")) {
      String numberPart = filename.substring(6, filename.length() - 4);
      int index = numberPart.toInt();
      if (index > maxIndex) {
        maxIndex = index;
      }
    }
  }
  return maxIndex;
}

void nav::path_following(GPScoord list_points[], int nb_points){
  for(int i = 0; i < nb_points - 1; i++){
    GPScoord starting_point = list_points[i];
    GPScoord ending_point = list_points[i+1];
    GPScoord gps_pos = gps->getPoint();
    Cartcoord start_cart = gps->conversion(starting_point);
    Cartcoord end_cart = gps->conversion(ending_point);
    Cartcoord pos_cart = gps->conversion(gps_pos);
    Cartcoord end2start = diff(start_cart, end_cart);
    Cartcoord end2pos =  diff(pos_cart, end_cart);
    while(scalprod(end2start, end2pos) > 0){ // While the boat has not overpassed the end of the line...
      linefollowing(starting_point.lat, starting_point.lng, ending_point.lat, ending_point.lng);
      gps_pos = gps->getPoint(); // Updates the 'while' loop's condition
      pos_cart = gps->conversion(gps_pos);
      end2pos = diff(pos_cart, end_cart);
    }
    //Note : this architecture should allow for the boat to be brought from one point to another using the controler, and then resuming it's mission
    // in autonoous mode properly.
    init_sequence_rud();
  }
  // for(int i = 0; i<3; i++){
  // init_sequence_rud();
  // delay(1000);
  // }
  // while(true){}; //Make sure the program won't start over. Might be removed later if we need the program to do something else once it has completed
  //this part of the mission...
  return;
}

void nav::basic_place_holder(int time_millis){
  int t0 = millis();
  while(millis - t0 < time_millis){
    powerboard->set_angle_rudder(50); // Set rudder to full left
    powerboard->set_angle_sail(SERVOMAX_SAIL); // Set the sail free/loose
  }
  return;
}