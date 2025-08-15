/*
Documentation at https://cdn.shopify.com/s/files/1/0515/5992/3873/files/7911_SS.pdf
Code created by copilot and then edited with correct values
*/

int heading_bytes;
float heading;
volatile int pulseCount = 0;
unsigned long lastTime = 0;

void countPulse() {
    pulseCount++;  // Incrémente à chaque impulsion
    Serial.println("Detected change...");
}

void setup() {
    Serial.begin(9600);
    pinMode(19, INPUT_PULLUP);  // Utilisation de le pin 19 avec pull-up
    attachInterrupt(digitalPinToInterrupt(19), countPulse, RISING);  // Déclenche sur front montant
}

void loop() {
    unsigned long currentTime = millis();
    if (currentTime - lastTime >= 2250) {  // Calcul toutes les secondes
        float windSpeed = pulseCount * 0.44704;  // Facteur de conversion (à ajuster selon le capteur)
        Serial.print("Vitesse du vent: ");
        Serial.print(windSpeed);
        Serial.println(" m/s");
        heading_bytes = analogRead(A15);
        heading =  -2 * atan(tan(((float)heading_bytes / 1023 * 2 * M_PI + M_PI)/2)) * 180/M_PI;
        Serial.print("Direction du vent:");
        Serial.print(heading);
        Serial.println("°");
        pulseCount = 0;  // Réinitialisation du compteur
        lastTime = currentTime;
    }
}
