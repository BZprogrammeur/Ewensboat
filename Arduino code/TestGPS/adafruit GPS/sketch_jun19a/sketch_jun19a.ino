#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include <SD.h>
#include <SPI.h>

#define GPSSerial Serial2
Adafruit_GPS GPS(&GPSSerial);

const int chipSelect = 53;  // Pin CS pour la carte SD (Mega 2560)
File logFile;

void setup() {
  Serial.begin(115200);
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  delay(1000);
  Serial.println("demarrage GPS");
  if (!SD.begin(chipSelect)) {
    Serial.println("Erreur : carte SD non détectée !");
    return;
  }
  Serial.println("Carte SD prête.");
  logFile = SD.open("gps.txt", FILE_WRITE);
  if (logFile) {
    logFile.println("=== Début du log ===");
    //logFile.close();
  } else {
    Serial.println("Erreur : impossible d'ouvrir log.txt");
  }
}


unsigned long lastLog = 0;
unsigned long interval = 5000; // 5 secondes

void loop() {
  GPS.read();
  if (GPS.newNMEAreceived()) {
    if (!GPS.parse(GPS.lastNMEA())) return;
    Serial.print("Fix: "); Serial.println(GPS.fix);
    Serial.print("Satellites: "); Serial.println(GPS.satellites);
    Serial.print("Latitude: "); Serial.println(GPS.latitudeDegrees, 6);
    Serial.print("Longitude: "); Serial.println(GPS.longitudeDegrees, 6);
    if (millis() - lastLog > interval) {
      //logFile = SD.open("gps.txt", FILE_WRITE);
      logFile.print(GPS.latitudeDegrees, 6);
      logFile.print(",");
      logFile.println(GPS.longitudeDegrees, 6);
      //logFile.close();
      logFile.flush();
      lastLog = millis();
    }
  }
}
