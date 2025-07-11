#include <Arduino.h>
#include <Wire.h>
#include "IMU.h"

#define CMPS12_ADDRESS 0x60  // Address of CMPS12 shifted right one bit for arduino wire library
#define ANGLE_8  1           // Register to read 8bit angle from

unsigned char high_byte, low_byte, angle8;
char pitch, roll;
int16_t angle16;

void imu_init()
{
    Serial.begin(9600);  // Start serial port
    Wire.begin();
}

void get_all_IMU_data()
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

    Serial.print("roll: ");               // Display roll data
    Serial.print(roll, DEC);

    Serial.print("    pitch: ");          // Display pitch data
    Serial.print(pitch, DEC);

    Serial.print("    angle full: ");     // Display 16 bit angle with decimal place
    Serial.print(angle16 / 10 - 180, DEC);
    Serial.print(".");
    Serial.print(angle16 % 10, DEC);

    Serial.print("    angle 8: ");        // Display 8bit angle
    Serial.println(angle8, DEC);
}

float get_cap(){
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
    return angle16/10 - 180;
}
