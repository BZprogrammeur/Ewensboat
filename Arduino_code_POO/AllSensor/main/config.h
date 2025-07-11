#ifndef CONFIG_H
#define CONFIG_H

#define SERVOMIN_RUDDER  200 // Minimum pulse length Rudder must be centered around XXX
#define SERVOMAX_RUDDER  410 // Maximum pulse length
#define SERVOMIN_SAIL  180 // Minimum pulse length
#define SERVOMAX_SAIL  280 // Maximum pulse length
#define SERVO_SAIL 0
#define SERVO_RUDDER 1

#define R 12
#define GAMMA M_PI/4  ///< Incidence angle
#define ZETA M_PI/4

#define R_EARTH 6371000.0

#define CMPS12_SERIAL Serial3
#define ANGLE_16 0x13
#define CMPS12_CALIBRATION_STATUS 0x24

#define AILERON_MIN 1260
#define AILERON_MAX 1830
#define ELEVATION_MIN 1900
#define ELEVATION_MAX 910
#define CONTROL_THRESHOLD 1450

// Coordinate for a line in front of the main entrance:
// Starting point : 52.4862971 -1.8895251
// Ending Point : 52.486735 -1.889113

#endif