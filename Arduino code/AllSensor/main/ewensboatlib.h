#ifndef EWENSBOATLIB_H
#define EWENSBOATLIB_H

#include <Arduino.h>
#include <Wire.h>
#include "controlMotor.h"
#include "IMU.h"
#include "config.h"
#include "GPS.h"
#include "windSensor.h"

class Navigation {
public:
    Navigation(IMU& imu, controlMotor& motor, WindSensor& wind, GPS& gps);
    
    void follow_cap(float cap_a_suivre);
    void reach_point(GPScoord point);
    void stopSailing();
    void tacking(float cap, float difference = 30.0);
    void CheckTacking(float cap);
private:
    const float Kp = 2.0;     // Gain proportionnel (à ajuster)
    const float Kd = 1.0;     // Gain dérivé (à ajuster)
    const float DELTA_T = 0.1; // Temps entre deux appels (en secondes)
    float erreur_precedente = 0;
    bool sens;
    bool isTacking;
    
    IMU& imu;
    controlMotor& powerboard;
    WindSensor& wind;
    GPS& gps;
};


#endif
