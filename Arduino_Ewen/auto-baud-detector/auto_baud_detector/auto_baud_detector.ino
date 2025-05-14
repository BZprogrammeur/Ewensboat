/*

https://www.electronicsforu.com/electronics-projects/hardware-diy/uart-automatic-baud-rate-detector

 Auto Baud Rate Detector - This arduino sketch predicts unknown uart baud rates.
 
 Note : I'm sure characters with multible zero bits in a row may fake out proper detection. If data is
 garbled in the serial monitor then the auto baud rate function failed.

Hardware connection details : 
* Connect  uart device's GND to GND of Arduino UNO
* Connect TX pin of uart device to RX(0th pin - D0) of Arduino UNO
 
*/
 
int recPin = 0;  //the pin receiving the serial input data
long baudRate;   // global in case useful elsewhere in a sketch
long rate=10000;

void setup(){
    pinMode(recPin, INPUT);      // make sure serial in is a input pin
    digitalWrite (recPin, HIGH); // pull up enabled just for noise protection
    Serial.begin(9600);
    delay(100);
}

void loop(){
    if (Serial.available() > 0) {     // wait for availbale stored bytes to read
  
        Serial.println("        Auto Baud Rate Detector           ");
        Serial.println("----------------------------------------- ");
        Serial.println("----------------------------------------- ");
    
        baudRate = detRate(recPin);  // Function finds a standard baudrate of either
                                     // 300,600,1200,2400,4800,9600,14400,19200,28800,38400,57600,115200 
                                     // Returns 0 if none or under 300 baud  
        Serial.flush();
        Serial.print("Detected  Standard baudrate at ");
        Serial.println(baudRate);
        Serial.print("rate in usec =  ");
        Serial.println(rate);
        float approx_baud_rate = 1 / (rate * 0.000001);
        Serial.print("Approximate Baudrate  =  ");
        Serial.print(approx_baud_rate);
        Serial.println();
        Serial.println("----------------------------------------- ");
        Serial.println();
        Serial.println();  
        delay(1000);
        Serial.end();                // Ends the serial communication 
        Serial.begin(9600);          // Re-establishes serial communication , this causes deletion of anything previously stored in the buffer //or cache
    }
}



/* function to return valid received baud rate */

long detRate(int recpin){
    long baud,  x;
   
    for (int i = 0; i < 5; i++){
        while(digitalRead(recpin) == 1){} // wait for low bit to start
        x = pulseIn(recpin,LOW);   // measure the next zero bit width
        rate = x < rate ? x : rate;
    }
     if (rate < 12)
        baud = 115200;
     else if (rate < 20)
        baud = 57600;
     else if (rate < 29)
        baud = 38400;
     else if (rate < 40)
        baud = 28800;
     else if (rate < 60)
        baud = 19200;
     else if (rate < 80)
        baud = 14400;
     else if (rate < 150)
        baud = 9600;
     else if (rate < 300)
        baud = 4800;
     else if (rate < 600)
        baud = 2400;
     else if (rate < 1200)
        baud = 1200;
     else if (rate < 2400)
        baud = 600;
     else if (rate < 4800)
        baud = 300;
     else 
        baud = 0;  
     return baud; 
}
