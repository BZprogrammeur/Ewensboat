#ifndef CONFIG_H
#define CONFIG_H

#define SERVOMIN_RUDDER  180 // Minimum pulse length
#define SERVOMAX_RUDDER  430 //somme des deux doit faire 580
#define SERVOMIN_SAIL  225 // Minimum pulse length
#define SERVOMAX_SAIL  350 // Maximum pulse length
#define R 12
#define GAMMA M_PI/4  ///< Incidence angle
#define ZETA M_PI/4
#define SERVO_SAIL 0
#define SERVO_RUDDER 1
#define R_EARTH 6371000.0

#define AILERON_MIN 1260
#define AILERON_MAX 1830
#define ELEVATION_MIN 1900
#define ELEVATION_MAX 910
#define CONTROL_THRESHOLD 1450

struct GPScoord{
  double lat;
  double lng;
};

struct Cartcoord{
  double x;
  double y;
};

const GPScoord M = {52.4844041, -1.8898449};

#define CMPS12_SERIAL Serial3
#define ANGLE_16 0x13
#define CMPS12_CALIBRATION_STATUS 0x24

#endif
