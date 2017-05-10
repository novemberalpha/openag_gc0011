
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

Gc0011 gc0011_1(12, 11);
std_msgs::Float32 gc0011_1_air_carbon_dioxide_msg;

uint32_t last_status_read = 0;

bool should_read_statuses() {
  uint32_t curr_time = millis();
  bool res = (curr_time - last_status_read) > 1000;
  if (res) {
    last_status_read = curr_time;
  }
  return res;
}

void setup() {
  Serial.begin(9600);
  gc0011_1.begin();
}

void loop() {
  gc0011_1.update();
  if (should_read_statuses()) {
    Serial.println(gc0011_1.get_air_carbon_dioxide(gc0011_1_air_carbon_dioxide_msg));
    if (gc0011_1.get_air_carbon_dioxide(gc0011_1_air_carbon_dioxide_msg)) {
      Serial.println("Success");
    }
  }
}

