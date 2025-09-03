#include "SDcard.h"

void SDcard::init() {
  if (!SD.begin(chipSelect)) {
    Serial.println("Erreur : carte SD non détectée !");
    return;
  }

  logNumber = getMaxLogIndex();

  sprintf(file, "log%d.txt", logNumber);

  Serial.println("Carte SD prête.");
  Serial.print("Ouverture : "); Serial.println(file);

  logFile = SD.open(file, FILE_WRITE);

  if (logFile) {
    logFile.println("=== Début du log ===");
    logFile.flush();
  } else {
    Serial.println("Erreur : impossible d'ouvrir les fichiers de log");
  }
}


void SDcard::saveDatas(float time, float latitude, float longitude,
                       float wind_direction, float wind_speed, float heading,
                       int angle_rud, int angle_sail, bool control) {
    //logFile = SD.open("log.txt", FILE_WRITE);
    if (logFile) {
        if (millis() - lastLog > interval) {
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

        //logFile.close();
          logFile.flush();
        }
    } else {
        Serial.println("Erreur d'ouverture du fichier log.txt");
    }
}

void SDcard::save_flag(int flag)
{
  if (logFile){
    logFile.print("flag: ");
    logFile.println(flag);
    logFile.flush();
    } 
  else {
        Serial.println("Erreur d'ouverture du fichier log.txt");
       }
}

void SDcard::save_capcible(float cap)
{
  if (logFile){
    logFile.print("cap cible: ");
    logFile.println(cap);
    logFile.flush();
    } 
    else {
        Serial.println("Erreur d'ouverture du fichier log.txt");
         }
}

int SDcard::getMaxLogIndex() {
  int maxIndex = -1;
  File root = SD.open("/");

  while (true) {
    File entry = root.openNextFile();
    if (!entry) {
      break;
    }

    String filename = entry.name();
    entry.close();

    if (filename.startsWith("log") && filename.endsWith(".TXT")) {
      String numberPart = filename.substring(6, filename.length() - 4);
      int index = numberPart.toInt();
      if (index > maxIndex) {
        maxIndex = index;
      }
    }
  }
  return maxIndex;
}
