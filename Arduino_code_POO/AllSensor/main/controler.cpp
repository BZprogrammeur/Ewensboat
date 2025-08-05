#include "controler.h"


Controler::Controler(){
  Serial.println("Initialising RC Remote controler...");
  pinMode(elevationPin, INPUT);
  pinMode(aileronPin, INPUT);
  pinMode(controlPin, INPUT);
}

void Controler::update(){
  controlValue = pulseIn(controlPin, HIGH);
  elevation = pulseIn(elevationPin, HIGH);
  aileron = pulseIn(aileronPin, HIGH);
  comSail = map(elevation, ELEVATION_MIN, ELEVATION_MAX, SERVOMIN_SAIL, SERVOMAX_SAIL);
  comRud = map(aileron, AILERON_MIN, AILERON_MAX, SERVOMAX_RUDDER, SERVOMIN_RUDDER);
}

int Controler::get_elevation(){
  return elevation;
}

int Controler::get_aileron(){
  return aileron;
}

int Controler::get_control_value(){
  return controlValue;
}

// void Controler::update_commands() {
//   elevation = pulseIn(elevationPin, HIGH);
//   aileron = pulseIn(aileronPin, HIGH);
//   comSail = map(elevation, ELEVATION_MIN, ELEVATION_MAX, SERVOMIN_SAIL, SERVOMAX_SAIL);
//   comRud = map(aileron, AILERON_MIN, AILERON_MAX, SERVOMAX_RUDDER, SERVOMIN_RUDDER);
// }

int Controler::get_com_rudder(){
  return comRud;
}

int Controler::get_com_sail(){
  return comSail;
}

bool Controler::unmanned_status(){
  return unmanned;
}

bool Controler::checkUnmanned() {
  if (controlValue > CONTROL_THRESHOLD) // correspond to the state where the last REV/NOR switch from the controller is up.
  {
    unmanned = true;
  }
  else {
    unmanned = false;
    Serial.println("RC active");
  }
  return unmanned;
}

int Controler::get_scenario_number(){
  if (controlValue < CONTROL_THRESHOLD){
    unmanned = false;
    return 9; // Function returns 9 if manual control is active
  }
  int scenario = 0;
  scenario += (int)(elevation < 1500);
  if(aileron < 1450){
    scenario += 4;
    // Serial.println("Added 4.");
  }
  if(1450 <= aileron && aileron < 1505){
    scenario += 6;
    // Serial.println("Added 6.");
  }
  if(1505 <= aileron && aileron < 1600){
    scenario += 0;
    // Serial.println("Added 0.");
  }
  if(1600 <= aileron){
    scenario += 2;
    // Serial.println("Added 2.");
  }
  // Serial.print("Aileron : ");
  // Serial.print(aileron);
  // Serial.print("; Elevation : ");
  // Serial.print(elevation);
  // Serial.print("; Control : ");
  // Serial.println(controlValue);
  return scenario;
}
