#include "IMU.h"

IMU* imuInstance = nullptr;

IMU::IMU()
{
  Serial.println("Initialising IMU...");
  cap = 0.0;
  CMPS12_SERIAL.begin(9600);
  calibrate();
}

void IMU::update()
{
  CMPS12_SERIAL.write(ANGLE_16);  // Request 16 bit angle
  while(CMPS12_SERIAL.available() < 2); // Wait for full angle (16 bits = 2 bytes)
  unsigned char high_byte = CMPS12_SERIAL.read();
  unsigned char low_byte = CMPS12_SERIAL.read();
  unsigned int angle16 = high_byte;           // Calculate 16 bit angle
  angle16 <<= 8;
  angle16 += low_byte;
  cap = angle16 / 10 + (float)(angle16%10)/10 - 180; 
  Serial.print("Angle:");
  Serial.println(cap);
  return;           
}

float IMU::get_heading(){
  //Serial.print("Current heading:");
  //Serial.println(cap);
  return ;
}

bool IMU::calibrate(){
  // Code made by Titouan Leost : https://github.com/TitouanLeost/Aston-Autonomous-Sailboat-2024
  // Erasing calibration data stored in the IMU:
  Serial.println("Calibration started...");
  CMPS12_SERIAL.write(0xE0);
  while(CMPS12_SERIAL.available() < 1);
  CMPS12_SERIAL.read();
  CMPS12_SERIAL.write(0xE5);
  while(CMPS12_SERIAL.available() < 1);
  CMPS12_SERIAL.read();
  CMPS12_SERIAL.write(0xE2);
  while(CMPS12_SERIAL.available() < 1);
  CMPS12_SERIAL.read();
  Serial.println("Checking calibration status...");
  CMPS12_SERIAL.write(CMPS12_CALIBRATION_STATUS);
  while(CMPS12_SERIAL.available() < 1);
  unsigned char status = CMPS12_SERIAL.read();
  int count = 0;
  while(count < 50 or status < 243) {
    CMPS12_SERIAL.write(CMPS12_CALIBRATION_STATUS);
    while(CMPS12_SERIAL.available() < 1);
    status = CMPS12_SERIAL.read();
    Serial.println(int(status), BIN);
    if(status >= 243){
        count += 1;
    }
    delay(100);
  }
  Serial.println("Calibration done");
  return true;
}

