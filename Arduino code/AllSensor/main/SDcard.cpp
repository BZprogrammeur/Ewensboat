#include "SDcard.h"

SDcard::SDcard() {
    if (!SD.begin(chipSelect)) {
        Serial.println("Erreur : carte SD non détectée !");
        return;
    }
    Serial.println("Carte SD prête.");

    logFile = SD.open("log.txt", FILE_WRITE);
    if (logFile) {
        logFile.println("=== Début du log ===");
        logFile.close();
    } else {
        Serial.println("Erreur : impossible d'ouvrir log.txt");
    }
}

void SDcard::saveDatas(float time, float latitude, float longitude,
                       float wind_direction, float wind_speed, float heading,
                       int angle_rud, int angle_sail, bool control) {
    logFile = SD.open("log.txt", FILE_WRITE);
    if (logFile) {
        logFile.print("Temps: ");
        logFile.print(time, 2);
        logFile.print(" s, ");

        logFile.print("Lat: ");
        logFile.print(latitude, 6);
        logFile.print(" °, ");

        logFile.print("Lon: ");
        logFile.print(longitude, 6);
        logFile.print(" °, ");

        logFile.print("Cap: ");
        logFile.print(heading);
        logFile.print(" °, ");

        logFile.print("Vent: ");
        logFile.print(wind_direction, 2);
        logFile.print(" °, ");
        logFile.print(wind_speed, 2);
        logFile.print(" m/s, ");

        logFile.print("Rudder: ");
        logFile.print(angle_rud);
        logFile.print(" °, ");

        logFile.print("Sail: ");
        logFile.print(angle_sail);
        logFile.print(" °, ");

        logFile.print("Control: ");
        logFile.println(control ? "AUTO" : "MANU");

        logFile.close();
    } else {
        Serial.println("Erreur d'ouverture du fichier log.txt");
    }
}
