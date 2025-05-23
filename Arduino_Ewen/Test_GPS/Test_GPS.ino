/*

Code from : https://www.redohm.fr/2015/03/utilisation-du-module-gps-grove-sen10752p/

*/
unsigned char buffer[64];
int count=0;   

void setup() {
  Serial2.begin(9600);
  Serial.begin(9600); 
}

void loop() {
  if (Serial2.available())                    
    {
        while(Serial2.available())            
        {
            buffer[count++]=Serial2.read();   
            if(count == 64)break;
        }
        Serial.write(buffer,count);              
        clearBufferArray();                      
        count = 0;                               
    }
}

void clearBufferArray()                     
{
    for (int i=0; i<count;i++)
    { buffer[i]=NULL;}                      
}