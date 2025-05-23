#ifndef EWENSBOATLIB_H
#define EWENSBOATLIB_H

#include <Arduino.h>
#include <Wire.h>
#include "controlMotor.h"
#include "IMU.h"
#include "config.h"

class nav {
public:
    nav();
    void follow_cap(float cap_a_suivre);
private:
    const float Kp = 2.0;     // Gain proportionnel (à ajuster)
    const float Kd = 1.0;     // Gain dérivé (à ajuster)
    const float DELTA_T = 0.1; // Temps entre deux appels (en secondes)
    float erreur_precedente = 0;
    IMU imu;
    controlMotor powerboard;
};


#endif
