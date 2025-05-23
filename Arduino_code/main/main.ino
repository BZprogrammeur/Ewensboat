#include "imu.h"

void setup() {
    Serial.begin(115200);
    imu_init();
}

void loop() {
    float roll = get_roll();
    Serial.print("Roll: ");
    Serial.println(roll);
    delay(100);
}
