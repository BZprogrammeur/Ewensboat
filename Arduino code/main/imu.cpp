#include "imu.h"
#include <Wire.h>
#include <MPU6050.h>

MPU6050 imu;

// Complementary filter variables
float roll = 0.0; // Filtered roll angle
float alpha = 0.98; // Complementary filter coefficient
unsigned long last_time = 0;

void imu_init() {
    Wire.begin();
    imu.initialize();
    if (!imu.testConnection()) {
        Serial.println("Échec de connexion à l'IMU !");
        while (1);
    }
    Serial.println("IMU connectée !");
}

float get_roll() {
    int16_t ax, ay, az, gx, gy, gz;
    imu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    
    // Convert accelerometer data to g
    float ax_g = ax / 16384.0;
    float ay_g = ay / 16384.0;
    float az_g = az / 16384.0;

    // Calculate roll angle from accelerometer
    float accel_roll = atan2(ay_g, az_g) * 180.0 / PI;

    // Convert gyroscope data to degrees per second
    float gyro_roll_rate = gx / 131.0;

    // Calculate elapsed time in seconds
    unsigned long current_time = millis();
    float dt = (current_time - last_time) / 1000.0;
    last_time = current_time;

    // Apply complementary filter
    roll = alpha * (roll + gyro_roll_rate * dt) + (1 - alpha) * accel_roll;

    return roll;
}
