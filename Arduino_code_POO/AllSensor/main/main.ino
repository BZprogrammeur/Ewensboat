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
  navigator->run_mission();
  // Dry test line 1
  // navigator->linefollowing(52.4844663, -1.8895039, 52.4843069, -1.8905943);
  
  // Dry test loop 
  // GPScoord Point1 = {52.4844663, -1.8895039};
  // GPScoord Point2 = {52.4843069, -1.8905943};
  // GPScoord Point3 = {52.4845141, -1.8905922};
  // GPScoord Point4 = {52.4847932, -1.8899488};
  // GPScoord Point5 = {52.4846881, -1.8896900};
  // GPScoord listpoints[] = {Point1, Point2, Point3, Point4, Point5, Point1};
  // navigator->path_following(listpoints, 6);

  // Dry test line 2
  // GPScoord Point1b = {52.485958, -1.889726};
  // GPScoord Point2b = {52.4860084, -1.8889055};
  // GPScoord listpointsb[] = {Point1b, Point2b, Point1b};
  // navigator -> path_following(listpointsb, 3);

  // Water test loop 1
  // Note : Point 5 is close to the edge and might have to be removed.
  // GPScoord Point1 = {52.42918, -1.94642};
  // GPScoord Point2 = {52.42924, -1.94659};
  // GPScoord Point3 = {52.42940, -1.94662};
  // GPScoord Point4 = {52.42958, -1.94590};
  // GPScoord Point5 = {52.429331, -1.945867};
  // GPScoord listpoints[] = {Point1, Point2, Point3, Point4, Point5, Point1};
  // navigator->path_following(listpoints, 6);

  // Water test line 1
  // GPSCoord Point1 = {52.42953, -1.94514};
  // GPSCoord Point2 = {52.42945, -1.94673};
  // GPScoord listpoints[] = {Point1, Point2, Point1};
  // navigator->path_following(listpoint, 3);

  // Water test line 2
  // Note : Very short line intended to test position holding.
  // GPSCoord Point1 = {52.42939, -1.94654};
  // GPSCoord Point2 = {52.42936, -1.94652};
  // GPScoord listpoints[] = {Point1, Point2, Point1};
  // navigator->non_blocking_path_following(listpoint, 3);

}
