/*
 Cozir Sample code
 Written by: Jason Berger ( for Co2meter.com)

 This sketch connects will connect to a Cozir gss or SprintIR sensor
 and report readings back to the host computer over usb. The value is
 stored in a global variable 'co2' and can be used for any number of applications.

 pin connections:

 Arduino________Cozir Sensor
 GND ------------------ 1 (gnd)
 3.3v------------------- 3 (Vcc)
 11 -------------------- 5 (Rx)
 12 -------------------- 7 (Tx)
 */
 
#include <SoftwareSerial.h>

SoftwareSerial mySerial(12, 11); // RX, TX
String val= ""; //holds the string of the value
double co2 =0; // holds the actual value
double multiplier = 1; //each range of sensor has a different value.
 // up to 2% =1
 // up to 65% = 10
 //up to 100% = 100;
uint8_t buffer[25];
uint8_t ind =0;

void setup()
{
 Serial.begin(9600); //Start Serial connection with host
 mySerial.begin(9600); //Start Serial connection with Sensor
 delay(1000);
 mySerial.println("K 1"); // Set to streaming mode
 delay(1000);

}
void loop()
{
 //We read incoming bytes into a buffer until we get '0x0A' which is the ASCII value for new-line
 while(buffer[ind-1] != 0x0A)
 {
   if(mySerial.available())
   {
    buffer[ind] = mySerial.read();
    ind++;
   }
 }

 report(); //Once we get the '0x0A' we will report what is in the buffer
}
void report()
{
 //Cycle through the buffer and send out each byte including the final linefeed

 /*
 each packet in the stream looks like "Z 00400 z 00360"
 'Z' lets us know its a co2 reading. the first number is the filtered value
 and the number after the 'z' is the raw value.
 We are really only interested in the filtered value

 */

 for(int i=0; i < ind+1; i++)
 {
 if(buffer[i] == 'z') //once we hit the 'z' we can stop
 break;

 if((buffer[i] != 0x5A)&&(buffer[i] != 0x20)) //ignore 'Z' and white space
 {
 val += buffer[i]-48; //because we break at 'z' the only bytes getting added are the numbers
 // we subtract 48 to get to the actual numerical value
 // example the character '9' has an ASCII value of 57. [57-48=9]
  }
 }

 co2 = (multiplier * val.toInt()); //now we multiply the value by a factor specific ot the sensor. see the Cozir software guide
 Serial.print( "Co2 = ");
 Serial.print(co2);
 Serial.println(" ppm");
 ind=0; //Reset the buffer index to overwrite the previous packet
 val=""; //Reset the value string
} 
