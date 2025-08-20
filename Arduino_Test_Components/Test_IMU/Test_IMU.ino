/*****************************************
*     CMPS12 I2C example for Arduino     *
*        By James Henderson, 2014        * 
*****************************************/
#define CMPS12_ADDRESS 0x60
#define CMPS12_Serial Serial3  // Address of CMPS12 shifted right one bit for arduino wire library
#define ANGLE_8  1           // Register to read 8bit angle from
#define CMPS_CALIBRATION_STATUS 0x24
#define CMPS_GET_ANGLE16 0x13

// unsigned char high_byte, low_byte, angle8;
// char pitch, roll;
// int16_t angle16;

float sawtooth(float x){
  return 2*atan(tan(x/2));
}

void setup()
{
  Serial.begin(9600);  // Start serial port
  Serial.println("Starting test...");
  CMPS12_Serial.begin(9600);
}

void loop(){
  //from Titouan Leost's code
  CMPS12_Serial.write(0xE0);
    while(CMPS12_Serial.available() < 1);
    Serial.println(CMPS12_Serial.read());
    CMPS12_Serial.write(0xE5);
    while(CMPS12_Serial.available() < 1);
    Serial.println(CMPS12_Serial.read());
    CMPS12_Serial.write(0xE2);
    while(CMPS12_Serial.available() < 1);
    Serial.println(CMPS12_Serial.read());

    Serial.println("   -> Checking calibration status...");

    CMPS12_Serial.write(CMPS_CALIBRATION_STATUS);
    while(CMPS12_Serial.available() < 1);
    unsigned char status = CMPS12_Serial.read();
    int m_cnt = 0;
    while(m_cnt < 50 or status != 255) {
        CMPS12_Serial.write(CMPS_CALIBRATION_STATUS);
        while(CMPS12_Serial.available() < 1);
        status = CMPS12_Serial.read();
        Serial.println(int(status), BIN);
        if(status == 255){
            m_cnt += 1;
        }
        delay(100);
    }

    Serial.println("   => Calibration done");
    delay(5);
    while(1){
      CMPS12_Serial.write(CMPS_GET_ANGLE16);  // Request and read 16 bit angle
      while(CMPS12_Serial.available() < 2);
        unsigned char high_byte = CMPS12_Serial.read();
        unsigned char low_byte = CMPS12_Serial.read();
        unsigned int angle16 = high_byte;           // Calculate 16 bit angle
        angle16 <<= 8;
        angle16 += low_byte;
        float m_raw_yaw = -sawtooth((angle16 / 10 + (float)(angle16%10)/10 - 180) * PI / 180) * 180 / PI; // heading = 0 if the boat is heading north, angle are given following the counterclockwise direction to facilitate computing.
        Serial.print("Angle:");
        Serial.println(m_raw_yaw);
    }
}

// void loop() {
//   if (Serial.available()) {
//     String input = Serial.readStringUntil('\n'); 
//     input.trim();

//     if (input.startsWith("0x") || input.startsWith("0X")) {
//       String hexPart = input.substring(2);
//       int value = (int) strtol(hexPart.c_str(), NULL, 16); 
//       byte command = (byte) value;

//       Serial.print("Received command: 0x");
//       Serial.println(command, HEX);

//       CMPS12_Serial.write(command);
//       Serial.println("Command sent to CMPS12");

//       delay(10); // Petit délai pour laisser le temps à la réponse

//       Serial.print("Received answer: ");
//       while (CMPS12_Serial.available() > 0) {
//         byte answer = CMPS12_Serial.read();
//         Serial.print("0x");
//         if (answer < 0x10) Serial.print("0"); // Pour affichage 0x0X
//         Serial.print(answer, HEX);
//         Serial.print(" ");
//       }
//       Serial.println();
//     } else {
//       Serial.println("Invalid format. Use 0x00 to 0xFF.");
//     }
//   }
// }

// void loop() {
//   Wire.beginTransmission(CMPS12_ADDRESS);  //starts communication with CMPS12
//   Wire.write(ANGLE_8);                     //Sends the register we wish to start reading from
//   Wire.endTransmission();
 
//   // Request 5 bytes from the CMPS12
//   // this will give us the 8 bit bearing, 
//   // both bytes of the 16 bit bearing, pitch and roll
//   Wire.requestFrom(CMPS12_ADDRESS, 5);       
  
//   while(Wire.available() < 5);        // Wait for all bytes to come back
  
//   angle8 = Wire.read();               // Read back the 5 bytes
//   high_byte = Wire.read();
//   low_byte = Wire.read();
//   pitch = Wire.read();
//   roll = Wire.read();
  
//   angle16 = high_byte;                 // Calculate 16 bit angle
//   angle16 <<= 8;
//   angle16 += low_byte;
    
//   Serial.print("roll: ");               // Display roll data
//   Serial.print(roll, DEC);
  
//   Serial.print("    pitch: ");          // Display pitch data
//   Serial.print(pitch, DEC);
  
//   Serial.print("    angle full: ");     // Display 16 bit angle with decimal place
//   Serial.print(angle16 / 10 - 180, DEC);
//   Serial.print(".");
//   Serial.print(angle16 % 10, DEC);
  
//   Serial.print("    angle 8: ");        // Display 8bit angle
//   Serial.println(angle8, DEC);
  
//   delay(100);                           // Short delay before next loop
// }