#include <Arduino.h>
#include <Wire.h>
#include "IMU.h"

#define CMPS12_ADDRESS 0x60  // Address of CMPS12 shifted right one bit for arduino wire library
#define ANGLE_8  1           // Register to read 8bit angle from

IMU::IMU()
{
    angle8 = 0;
    high_byte = 0;
    low_byte = 0;
    pitch = 0;
    roll = 0;
    angle16 = 0;
    cap = 0.0;
}
void imu_init()
{
    Serial.begin(9600); 
    Wire.begin();
    Serial.println("Initialisation IMU...");
}

void update()
{
    Wire.beginTransmission(CMPS12_ADDRESS);  //starts communication with CMPS12
    Wire.write(ANGLE_8);                     //Sends the register we wish to start reading from
    Wire.endTransmission();

    // Request 5 bytes from the CMPS12
    // this will give us the 8 bit bearing, 
    // both bytes of the 16 bit bearing, pitch and roll
    Wire.requestFrom(CMPS12_ADDRESS, 5);       

    unsigned long timeout = millis();
    while (Wire.available() < 5) {
      if (millis() - timeout > 500) {
        Serial.println("Timeout I2C : données IMU non disponibles.");
        return;  // abandonne la fonction
      }
    }        // Wait for all bytes to come back

    angle8 = Wire.read();               // Read back the 5 bytes
    high_byte = Wire.read();
    low_byte = Wire.read();
    pitch = Wire.read();
    roll = Wire.read();

    angle16 = high_byte;                 // Calculate 16 bit angle
    angle16 <<= 8;
    angle16 += low_byte;

    cap = angle16 / 10;               
}

float get_cap(){
  return cap;
}
