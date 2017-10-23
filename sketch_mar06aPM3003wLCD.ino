//******************************
 //*Abstract: Read value of PM1,PM2.5 and PM10 of air quality
 //
 //*Version：V3.1
 //*Author：Zuyang @ HUST
 //*Modified by Cain for Arduino Hardware Serial port compatibility
 //*Date：March.25.2016
 //******************************
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#define LENG 23   //0x42 + 31 bytes equal to 32 bytes 24
unsigned char buf[LENG];

int PM01Value=0;          //define PM1.0 value of the air detector module
int PM2_5Value=0;         //define PM2.5 value of the air detector module
int PM10Value=0;         //define PM10 value of the air detector module
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
// you can change the overall brightness by range 0 -> 255
int brightness = 255;
 

void setup()
{
  Serial.begin(9600);   //use serial0
  Serial.setTimeout(1500);    //set the Timeout to 1500ms, longer than the data transmission periodic time of the sensor
  // set up the LCD's number of rows and columns: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Booting Arduino...");
}

void loop()
{
  if(Serial.find(0x42)){    //start to read when detect 0x42
    Serial.readBytes(buf,LENG);

    if(buf[0] == 0x4d){
      if(checkValue(buf,LENG)){
        PM01Value=transmitPM01(buf); //count PM1.0 value of the air detector module
        PM2_5Value=transmitPM2_5(buf);//count PM2.5 value of the air detector module
        PM10Value=transmitPM10(buf); //count PM10 value of the air detector module 
      }           
    } 
  }
  static unsigned long OledTimer=millis();  
    if (millis() - OledTimer >=1000) 
    {
      OledTimer=millis(); 
  // write to LCD
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("PM 2.5: ");
    lcd.setCursor(9,0);
    lcd.print(PM2_5Value);
    lcd.setCursor(0,1);
    lcd.print("PM 10 : ");
    lcd.setCursor(9,1);
    lcd.print(PM10Value);
     }
/*
 // replaced old code to serial out
  static unsigned long OledTimer=millis();  
    if (millis() - OledTimer >=1000) 
    {
      OledTimer=millis(); 
      
      Serial.print("PM1.0: ");  
      Serial.print(PM01Value);
      Serial.println("  ug/m3");            
    
      Serial.print("PM2.5: ");  
      Serial.print(PM2_5Value);
      Serial.println("  ug/m3");     
      
      Serial.print("PM1 0: ");  
      Serial.print(PM10Value);
      Serial.println("  ug/m3");   
      Serial.println();
    }
  */
}
char checkValue(unsigned char *thebuf, char leng)
{  
  char receiveflag=0;
  int receiveSum=0;

  for(int i=0; i<(leng-2); i++){
  receiveSum=receiveSum+thebuf[i];
  }
  receiveSum=receiveSum + 0x42;
 
  if(receiveSum == ((thebuf[leng-2]<<8)+thebuf[leng-1]))  //check the serial data 
  {
    receiveSum = 0;
    receiveflag = 1;
  }
  return receiveflag;
}

int transmitPM01(unsigned char *thebuf)
{
  int PM01Val;
  PM01Val=((thebuf[3]<<8) + thebuf[4]); //count PM1.0 value of the air detector module
  return PM01Val;
}

//transmit PM Value to PC
int transmitPM2_5(unsigned char *thebuf)
{
  int PM2_5Val;
  PM2_5Val=((thebuf[5]<<8) + thebuf[6]);//count PM2.5 value of the air detector module
  return PM2_5Val;
  }

//transmit PM Value to PC
int transmitPM10(unsigned char *thebuf)
{
  int PM10Val;
  PM10Val=((thebuf[7]<<8) + thebuf[8]); //count PM10 value of the air detector module  
  return PM10Val;
}
