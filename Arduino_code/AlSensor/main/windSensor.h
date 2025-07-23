#ifndef WINDSENSOR_H
#define WINDSENSOR_H

#include <Arduino.h>
#include <Wire.h>

void wind_sensor_init();
void get_wind_data();
void countPulse();
float get_wind_direction();

#endif 
