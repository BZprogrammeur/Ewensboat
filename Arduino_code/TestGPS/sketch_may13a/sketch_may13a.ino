#include <TinyGPSPlus.h>
#include <HardwareSerial.h>

TinyGPSPlus gps;
HardwareSerial &gpsSerial = Serial2; // Utilise RX1/TX1 (pins 19/18)

void setup() {
  Serial.begin(9600);      // Pour voir les données sur le moniteur série
  gpsSerial.begin(9600);   // La plupart des GPS utilisent 9600 bps
  Serial.println("Initialisation GPS...");
}

void loop() {
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }

  if (gps.location.isUpdated()) {
    Serial.print("Latitude : ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitude : ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("Satellites : ");
    Serial.println(gps.satellites.value());
    Serial.print("Altitude : ");
    Serial.println(gps.altitude.meters());
    Serial.println("--------------------------");
  }
  else{Serial.println("pas de données reçues");}
}
