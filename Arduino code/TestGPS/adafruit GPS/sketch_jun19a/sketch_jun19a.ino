#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

#define GPSSerial Serial2
Adafruit_GPS GPS(&GPSSerial);

void setup() {
  Serial.begin(115200);
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  delay(1000);
  Serial.println("demarrage GPS");
}

void loop() {
  char c = GPS.read();
  if (GPS.newNMEAreceived()) {
    if (!GPS.parse(GPS.lastNMEA())) return;

    Serial.print("Fix: "); Serial.println(GPS.fix);
    Serial.print("Satellites: "); Serial.println(GPS.satellites);
    Serial.print("Latitude: "); Serial.println(GPS.latitudeDegrees, 6);
    Serial.print("Longitude: "); Serial.println(GPS.longitudeDegrees, 6);

  }
}
