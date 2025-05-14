const byte pinAnemo = 2;       // Entrée impulsions anémomètre
volatile unsigned int impulsions = 0;

unsigned long dernierCalcul = 0;
const unsigned long intervalleMesure = 2000;  // 2 secondes

// Facteur de conversion (à ajuster selon ton capteur)
const float facteurVitesse = 2.4;  // km/h par impulsion/sec

void compteur() {
  impulsions++;
}

void setup() {
  pinMode(pinAnemo, INPUT_PULLUP);  // Utilise résistance interne
  attachInterrupt(digitalPinToInterrupt(pinAnemo), compteur, FALLING);
  Serial.begin(9600);
}

void loop() {
  unsigned long maintenant = millis();

  if (maintenant - dernierCalcul >= intervalleMesure) {
    noInterrupts();               // On bloque les interruptions pendant le calcul
    unsigned int nb = impulsions;
    impulsions = 0;
    interrupts();                 // On réactive les interruptions

    float nbSec = intervalleMesure / 1000.0;
    float impulsionsParSec = nb / nbSec;
    float vitesse_kmh = impulsionsParSec * facteurVitesse;
    float vitesse_ms = vitesse_kmh / 3.6;

    Serial.print("Impulsions/sec : ");
    Serial.print(impulsionsParSec);
    Serial.print(" | Vitesse : ");
    Serial.print(vitesse_kmh);
    Serial.print(" km/h (");
    Serial.print(vitesse_ms);
    Serial.println(" m/s)");

    dernierCalcul = maintenant;
  }
}
