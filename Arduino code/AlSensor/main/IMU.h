#ifndef IMU_H
#define IMU_H

#include <Arduino.h>
#include <Wire.h>

class IMU {
public:
    IMU();
    void init();
    void update();
    float get_cap();

private:
    unsigned char angle8;
    unsigned char high_byte;
    unsigned char low_byte;
    char pitch;
    char roll;
    unsigned int angle16;
    double cap;


    static const int CMPS12_ADDRESS = 0x60; // Adresse I2C du CMPS12
    static const int ANGLE_8 = 0x00;        // Registre pour l'angle 8 bits
};

#endif 
