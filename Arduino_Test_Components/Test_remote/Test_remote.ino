// #include <Adafruit_PWMServoDriver.h>

#define SERVOMIN_SAIL  230 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX_SAIL  420 // this is the 'maximum' pulse length count (out of 4096)
#define SERVOMIN_RUDDER 150
#define SERVOMAX_RUDDER 600

// Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

uint8_t servosail = 0;
uint8_t servorud = 1;

int controlpin = 23;
int verticalpin = 25;
int elevationpin = 3;
int aileronpin = 2;
int elevation, aileron, com_sail, com_rud, control, vertical;

bool unmaned = false;
int compteur = 0;

void setup() {
    Serial.begin(9600);
    pinMode(elevationpin, INPUT);
    pinMode(aileronpin, INPUT);
    pinMode(controlpin, INPUT);
    pinMode(verticalpin, INPUT);
    // pwm.begin();
    // pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
}

void loop() {
  aileron = pulseIn(aileronpin, HIGH);
  Serial.print("Aileron : ");
  Serial.print(aileron);
  elevation = pulseIn(elevationpin, HIGH);
  Serial.print("; Elevation : ");
  Serial.print(elevation);
  control = pulseIn(controlpin, HIGH);
  Serial.print("; Control : ");
  Serial.println(control);
  // control = pulseIn(verticalpin, HIGH);
  // Serial.print("; Vertical : ");
  // Serial.println(vertical);
  delay(100);
}