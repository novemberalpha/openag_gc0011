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
 
#include "openag_gc0011.h"

String message = "";
String parameters[] = {"1"};
Gc0011 gc0011_1("gc0011_1", parameters);

void setup() {
  Serial.begin(9600);
  gc0011_1.begin();
}

void loop() {
  delay(1000);
}

