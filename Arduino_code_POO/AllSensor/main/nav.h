#ifndef EWENSBOATLIB_H
#define EWENSBOATLIB_H

#include <Arduino.h>
#include <Wire.h>
#include "controlMotor.h"
#include "IMU.h"
#include "config.h"
#include "windSensor.h"
#include "GPS.h"
#include <SD.h>
#include <SPI.h>
#include "controler.h"

#define PIN_SPI_CS 53

class nav {
public:
  nav();  // Déclaration correcte du constructeur
  ~nav();
  void follow_cap(float cap_a_suivre);
  void set_sail_pos();
  void linefollowing(float lata, float longa, float latb, float longb);
  void update_logs();
  void update();
private:
    const float Kp = 2.0;     // Gain proportionnel (à ajuster)
    const float Kd = 1.0;     // Gain dérivé (à ajuster)
    const float DELTA_T = 0.1; // Temps entre deux appels (en secondes)
    float erreur_precedente = 0;
    IMU* imu;
    controlMotor* powerboard;
    WindSensor* wind;
    GPS* gps;
    Controler* controler;
    char filename[32];
    //Tacking variable :
    bool unmanned = false;
    bool sens;
    bool isTacking;
    unsigned long tackingStart = 0;
    bool tackingMode = false;
    float marge;
    void init_sequence_rud();
};

int getMaxLogIndex();

#endif
