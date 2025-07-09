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

void Controler::update_commands() {
  while(!unmanned){
  elevation = pulseIn(elevationPin, HIGH);
  aileron = pulseIn(aileronPin, HIGH);
  comSail = map(elevation, ELEVATION_MIN, ELEVATION_MAX, SERVOMIN_SAIL, SERVOMAX_SAIL);
  comRud = map(aileron, AILERON_MIN, AILERON_MAX, SERVOMAX_RUDDER, SERVOMIN_RUDDER);
  }
}

int Controler::get_com_rudder(){
  return comRud;
}

int Controler::get_com_sail(){
  return comSail;
}

bool Controler::isUnmanned() {
  controlValue = pulseIn(controlPin, HIGH);
  if (controlValue < CONTROL_THRESHOLD) // correspond to the state where the last REV/NOR switch from the controller is up.
  {
    unmanned = true;
  }
  else {
    unmanned = false;
    Serial.println("Remote control active.");
    update_commands();
  }
  return unmanned;
}
