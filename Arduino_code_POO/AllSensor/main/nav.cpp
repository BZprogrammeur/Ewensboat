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
  logfile.print(controler->unmanned_status());
  logfile.print(' ');
  logfile.println(scenario);
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
  controler->update();
  update_logs();
  return;
}

void nav::linefollowing(float lata, float longa, float latb, float longb, bool integral = false){
  update();
  if(controler->checkUnmanned()){
    // from the Matlab simulation coded by Pr. Jian Wan
    // % a --- the starting point;
    // % b --- the ending point;
    float heading = sawtooth(imu->get_heading() * PI / 180);
    // % r --- the cutoff distance;
    float r = 10.; //Short distance such as 3 meters will hopefully allow the boat to navigate in a narrow canal.
    // % gamma --- the incidence angle;
    float gamma = PI / 4;
    // % phi --- the close hauled angle;
    float phi = PI / 3;
    // % angle_ruddermax --- the maximum rudder angle;
    float angle_ruddermax = 50;
    // % angle_truewind --- the true wind direction;
    float angle_truewind = get_true_wind_dir();
    // Serial.print("True wind heading : ");
    // Serial.println(angle_truewind * 180 / PI);

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
    if(e >= 50){
      z = 0;
    }
    if(abs(e) > r/2){
        q = e / abs(e);
    }
    // TODO : Clarify the part above about q value.
    float angle_target = sawtooth(atan2(ab.y,ab.x) - (PI/2));
    if(integral){
      z += alpha * dt * e;
    }
    float angle_nominal = sawtooth(angle_target-2*gamma*atan((e + z)/r)/PI);

    float aimed_angle;
    if((cos(angle_truewind-angle_nominal)+cos(phi) < 0) || ((abs(e) < r) && ((cos(angle_truewind-angle_target)+cos(phi)) < 0))){
      aimed_angle= sawtooth(PI + angle_truewind - q * phi);
      z = 0;
    }
    else{
      aimed_angle=angle_nominal;
    }
    // Serial.print("Target angle:");
    // Serial.println(aimed_angle);
    float angle_rudder;
    angle_rudder = angle_ruddermax*sin(sawtooth(heading-aimed_angle));
    if(cos(heading - aimed_angle) < 0){ // Meaning "if the aimed heading is behind the boat"
      if(sin(heading - aimed_angle) >= 0){
        angle_rudder = angle_ruddermax;
      }
      else{
        angle_rudder = - angle_ruddermax;
      }
    }    // Serial.print("Angle rudder:");
    // Serial.println(angle_rudder);
    powerboard->set_angle_rudder(angle_rudder);
    float angle_sail = 45 * (cos(angle_truewind - aimed_angle) + 1);
    powerboard->set_angle_sail(angle_sail);
  }
  else{
    powerboard->send_com_rudder(controler->get_com_rudder());
    powerboard->send_com_sail(controler->get_com_sail());
  }
  delay(dt * 1000);
  return;
  }

float nav::get_true_wind_dir(){
  // According to previous internship reports and : https://www.bwsailing.com/cc/2017/05/calculating-the-true-wind-and-why-it-matters/
  float SOG = gps->getSOG();
  // Serial.print("SOG: ");
  // Serial.println(SOG);
  float COG = imu->get_heading() * PI / 180; // technically false but good enough approximation as the GPS is very unprecise and the boat doesn't drift much.
  float AWS = wind->get_wind_speed() ;
  // Serial.print("AWS: ");
  // Serial.println(AWS);
  float AWD = sawtooth((wind->get_wind_direction() + imu->get_heading()) * PI / 180);
  if(AWS == 0 && SOG == 0){
    return AWD;
  }
  float u = SOG * sin(COG) - AWS * sin(AWD);
  float v = SOG * cos(COG) - AWS * cos(AWD);
  return sawtooth(atan2(u, v) - PI);
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

void nav::path_following(GPScoord list_points[], int nb_points, bool integral = false){
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
      linefollowing(starting_point.lat, starting_point.lng, ending_point.lat, ending_point.lng, integral);
      gps_pos = gps->getPoint(); // Updates the 'while' loop's condition
      pos_cart = gps->conversion(gps_pos);
      end2pos = diff(pos_cart, end_cart);
    }
    //Note : this architecture should allow for the boat to be brought from one point to another using the controler, and then resuming it's mission
    // in autonoous mode properly.
    // init_sequence_rud();
    z = 0;
  }
  // for(int i = 0; i<3; i++){
  // init_sequence_rud();
  // delay(1000);
  // }
  while(true){
    if(!controler->checkUnmanned()){
      z = 0;
      powerboard->send_com_rudder(controler->get_com_rudder());
      powerboard->send_com_sail(controler->get_com_sail());
    }
  }; //Make sure the program won't start over. Might be removed later if we need the program to do something else once it has completed
  //this part of the mission...
  return;
}

void nav::non_blocking_path_following(GPScoord list_points[], int nb_points, bool integral = false){
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
      linefollowing(starting_point.lat, starting_point.lng, ending_point.lat, ending_point.lng, integral);
      gps_pos = gps->getPoint(); // Updates the 'while' loop's condition
      pos_cart = gps->conversion(gps_pos);
      end2pos = diff(pos_cart, end_cart);
      if(!controler->checkUnmanned()){
        Serial.println("Switching back to manual control.");
        return;
      }
    }
    //Note : this architecture should allow for the boat to be brought from one point to another using the controler, and then resuming it's mission
    // in autonoous mode properly.
    // init_sequence_rud();
    z = 0;
  }
  Serial.println("Path following done.");
}

void nav::basic_place_holder(int time_millis){
  int t0 = millis();
  while(millis - t0 < time_millis){
    powerboard->set_angle_rudder(50); // Set rudder to full left
    powerboard->set_angle_sail(SERVOMAX_SAIL); // Set the sail free/loose
  }
  return;
}

void nav::run_mission(){
  update();
  scenario = (int)controler->get_scenario_number();
  // Serial.print("Scenario :");
  // Serial.println(scenario);
  if(scenario == 0){
    // Water test line 1
    Serial.println("Scenario 0");
    // GPScoord Point10 = {52.429415, -1.946715};
    // GPScoord Point20 = {52.429463, -1.945933};
    GPScoord Point10 = {52.429390, -1.946684};
    GPScoord Point20 = {52.429450, -1.946031};
    GPScoord listpoints0[] = {Point10, Point20, Point10};
    non_blocking_path_following(listpoints0, 3);
  }
  else if(scenario == 1){
    // Water test loop 1
    Serial.println("Scenario 1");
    // GPScoord Point11 = {52.429209, -1.946479};
    // GPScoord Point21 = {52.429459, -1.946778};
    // GPScoord Point31 = {52.429506, -1.946167};
    GPScoord Point11 = {52.429246, -1.946501};
    GPScoord Point21 = {52.429420, -1.946751};
    GPScoord Point31 = {52.429440, -1.946201};
    GPScoord listpoints1[] = {Point11, Point21, Point31, Point11};
    non_blocking_path_following(listpoints1, 4);
  }
  else if(scenario == 2){
    // Water test line 2
    // Note : Short vertical line.
    Serial.println("Scenario 2");
    GPScoord Point12 = {52.429254, -1.946549};
    GPScoord Point22 = {52.429447, -1.946549};
    GPScoord listpoints2[] = {Point12, Point22, Point12};
    non_blocking_path_following(listpoints2, 3);
  }
  else if(scenario == 3){
    // Water test loop 2
    // Note : goes around the whole lake.
    Serial.println("Scenario 3");
    // GPScoord Point13 = {52.429172, -1.946466};
    // GPScoord Point23 = {52.429439, -1.946722};
    // GPScoord Point33 = {52.429542, -1.945092};
    // GPScoord Point43 = {52.429359, -1.946209};
    GPScoord Point13 = {52.429250, -1.946510};
    GPScoord Point23 = {52.429438, -1.946770};
    GPScoord Point33 = {52.429502, -1.945649};
    GPScoord Point43 = {52.429433, -1.946102};
    GPScoord listpoints3[] = {Point13, Point23, Point33, Point43, Point13};
    non_blocking_path_following(listpoints3, 5);
  }
  else if(scenario == 4){
    //Water test line 1
    Serial.println("Scenario 4");
    // GPScoord Point10 = {52.429415, -1.946715};
    // GPScoord Point20 = {52.429463, -1.945933};
    GPScoord Point10 = {52.429390, -1.946684};
    GPScoord Point20 = {52.429450, -1.946031};
    GPScoord listpoints0[] = {Point10, Point20, Point10};
    non_blocking_path_following(listpoints0, 3, true);
  }
  else if(scenario == 5){
    // Water test loop 1
    Serial.println("Scenario 5");
    // GPScoord Point11 = {52.429209, -1.946479};
    // GPScoord Point21 = {52.429459, -1.946778};
    // GPScoord Point31 = {52.429506, -1.946167};
    GPScoord Point11 = {52.429246, -1.946501};
    GPScoord Point21 = {52.429420, -1.946751};
    GPScoord Point31 = {52.429440, -1.946201};
    GPScoord listpoints1[] = {Point11, Point21, Point31, Point11};
    non_blocking_path_following(listpoints1, 4, true);
  }
  else if(scenario == 6){
    // Water test loop 2
    // Note : goes around the whole lake.
    Serial.println("Scenario 6");
    // GPScoord Point13 = {52.429172, -1.946466};
    // GPScoord Point23 = {52.429439, -1.946722};
    // GPScoord Point33 = {52.429542, -1.945092};
    // GPScoord Point43 = {52.429359, -1.946209};
    GPScoord Point13 = {52.429250, -1.946510};
    GPScoord Point23 = {52.429438, -1.946770};
    GPScoord Point33 = {52.429502, -1.945649};
    GPScoord Point43 = {52.429433, -1.946102};
    GPScoord listpoints3[] = {Point13, Point23, Point33, Point43, Point13};
    non_blocking_path_following(listpoints3, 5, true);
  }
  else if(scenario == 7){
    // Dry test loop 
    Serial.println("Scenario 7");
    GPScoord Point1 = {52.4844663, -1.8895039};
    GPScoord Point2 = {52.4843069, -1.8905943};
    GPScoord Point3 = {52.4845141, -1.8905922};
    GPScoord Point4 = {52.4847932, -1.8899488};
    GPScoord Point5 = {52.4846881, -1.8896900};
    GPScoord listpoints[] = {Point1, Point2, Point3, Point4, Point5, Point1};
    non_blocking_path_following(listpoints, 6);
  }
  else if(scenario == 9){
      Serial.println("Manual control");
      powerboard->send_com_rudder(controler->get_com_rudder());
      powerboard->send_com_sail(controler->get_com_sail());
  }    
  delay(100);
}