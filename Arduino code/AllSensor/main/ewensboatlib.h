#ifndef EWENSBOATLIB_H
#define EWENSBOATLIB_H

#include <Arduino.h>
#include <Wire.h>
#include "controlMotor.h"
#include "IMU.h"
#include "config.h"
#include "GPS2.h"
#include "windSensor.h"

class Navigation {
public:
    Navigation(IMU& imu, controlMotor& motor, WindSensor& wind, GPS2& gps);
    
    void follow_cap(float cap_a_suivre);
    void reach_point(GPScoord point);
    void line_following(GPScoord arrival, GPScoord startline);
    void stopSailing();
    void count4tacking();
    void CheckTacking();
    bool getTacking();
    float getRudderPos();
    void set_sail_pos();

    
private:
    const float Kp = 2.0;     // Gain proportionnel (à ajuster)
    const float Kd = 1.0;     // Gain dérivé (à ajuster)
    const float DELTA_T = 0.1; // Temps entre deux appels (en secondes)
    float erreur_precedente = 0;

    //variable concernant le tacking
    bool sens;
    bool isTacking;
    unsigned long tackingStart = 0;
    bool tackingMode = false;
    float marge;

    //Pour les datas
    float angle_rudder = 0;
    
    IMU& imu;
    controlMotor& powerboard;
    WindSensor& wind;
    GPS2& gps;
};


#endif
