/*
 * Created by Ewen MELE based on a code from ArduinoGetStarted.com
 *
 * Original code page: https://arduinogetstarted.com/tutorials/arduino-micro-sd-card
 */

#include <SD.h>

#define PIN_SPI_CS 53

File myFile;

void setup() {
  Serial.begin(9600);

  if (!SD.begin(PIN_SPI_CS)) {
    Serial.println(F("SD CARD FAILED, OR NOT PRESENT!"));
    while (1); // don't do anything more:
  }

  Serial.println(F("SD CARD INITIALIZED."));

  // open file for reading
  myFile = SD.open("NAVLOG46.TXT", FILE_READ);
  if (myFile) {
    while (myFile.available()) {
      char ch = myFile.read(); // read characters one by one from Micro SD Card
      Serial.print(ch); // print the character to Serial Monitor
    }
    myFile.close();
  } else {
    Serial.print(F("SD Card: error on opening file NAVLOG46.TXT"));
  }
}

void loop() {
}

