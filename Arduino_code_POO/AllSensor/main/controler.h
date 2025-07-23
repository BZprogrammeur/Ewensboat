#ifndef CONTROLER_H
#define CONTROLER_H

#include <Arduino.h>
#include "config.h"

class Controler {
public:
    Controler();
    void update();
    int get_elevation();
    int get_aileron();
    void update_commands();
    bool unmanned_status();
    bool checkUnmanned();
    int get_control_value();
    int get_com_rudder();
    int get_com_sail();
private:
    const int controlPin = 23;
    const int elevationPin = 3;
    const int aileronPin = 2;
    int elevation, aileron, comSail, comRud, controlValue;
    bool unmanned = true;
};

#endif // CONTROLER_H
