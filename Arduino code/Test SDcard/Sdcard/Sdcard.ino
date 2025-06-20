#include <SD.h>
#include <SPI.h>

const int chipSelect = 53;  // Pin CS (SS) de la carte SD

File logFile;

void setup() {
  Serial.begin(9600);

  // Initialisation de la carte SD
  if (!SD.begin(chipSelect)) {
    Serial.println("Erreur : carte SD non détectée !");
    return;
  }
  Serial.println("Carte SD prête.");

  // Ouvrir (ou créer) le fichier log.txt
  logFile = SD.open("log.txt", FILE_WRITE);
  if (logFile) {
    logFile.println("=== Début du log ===");
    logFile.close();  // Toujours fermer après écriture
  } else {
    Serial.println("Erreur : impossible d'ouvrir log.txt");
  }
}

void loop() {
  // Exemples de données à logguer
  float temperature = random(20, 30);  // Simule des données

  logFile = SD.open("log.txt", FILE_WRITE);
  if (logFile) {
    logFile.print("Temp: ");
    logFile.print(temperature, 2);
    logFile.println(" C");
    logFile.close();
  } else {
    Serial.println("Erreur d'ouverture du fichier log.txt");
  }

  delay(2000);  // Attend 2 secondes avant d'enregistrer à nouveau
}
