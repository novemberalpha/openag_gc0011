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

void setup()
{
 Serial.begin(9600); //Start Serial connection with host
 mySerial.begin(9600); //Start Serial connection with Sensor
 delay(1000);
 mySerial.println("K 2"); // Set to polling mode
 delay(1000);

}
void loop()
{
  delay(1000);
  mySerial.println("Z");
  String data_string = mySerial.readStringUntil(0x0A);
  float co2 = 0;

  // Check for failure
  if (data_string[1] != 'Z') {
    Serial.println("Error");
  }
  else { //  good reading
    co2 = (float)(data_string.substring(3,8).toInt());
    co2 = round(co2 / 10) * 10;
    Serial.println(co2);
  }
}
