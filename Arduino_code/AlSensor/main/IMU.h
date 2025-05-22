#ifndef IMU_H
#define IMU_H

#include <Arduino.h>
#include <Wire.h>

void imu_init();
void get_all_IMU_data();
float get_cap();

#endif 
