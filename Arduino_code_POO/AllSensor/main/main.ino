#include "nav.h"
#include "controler.h"

nav* navigator;
Controler* controler;

void setup() {
    Serial.begin(9600);
    Serial.println("Starting main...");
    delay(500);
    navigator = new nav();
}

void loop() {
  // navigator->update();
  // navigator->linefollowing(52.4844663, -1.8895039, 52.4843069, -1.8905943);
  // GPScoord Point1 = {52.4844663, -1.8895039};
  // GPScoord Point2 = {52.4843069, -1.8905943};
  // GPScoord Point3 = {52.4845141, -1.8905922};
  // GPScoord Point4 = {52.4847932, -1.8899488};
  // GPScoord Point5 = {52.4846881, -1.8896900};
  // GPScoord listpoints[] = {Point1, Point2, Point3, Point4, Point5, Point1};
  // navigator->path_following(listpoints, 6);
  GPScoord Point1b = {52.485958, -1.889726};
  GPScoord Point2b = {52.4860084, -1.8889055};
  GPScoord listpointsb[] = {Point1b, Point2b, Point1b};
  navigator -> path_following(listpointsb, 3);

}
