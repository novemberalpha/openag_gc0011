/**
 *  \file openag_gc0011.h
 *  \brief Carbon Dioxide Sensor for PFC1 BOM
 */
#include "openag_gc0011.h"

Gc0011::Gc0011(int rx_pin, int tx_pin) {
  status_level = OK;
  status_code = CODE_OK;
  status_msg = "";
  _send_carbon_dioxide = false;
  _time_of_last_reading = 0;
  SoftwareSerial _serial_port(rx_pin, tx_pin); //connect with sensor
}

void Gc0011::begin() {
  // Enable serial port
  _serial_port->begin(9600);

  // Set operation mode
  _serial_port->print("K 2\r\n"); // enable polling mode
}

bool Gc0011::get_air_carbon_dioxide(std_msgs::Float32 &msg) {
  msg.data = _carbon_dioxide;
  bool res = _send_carbon_dioxide;
  _send_carbon_dioxide = false;
  return res;
}

void Gc0011::update() {
  if (millis() - _time_of_last_reading > _min_update_interval){
    readData();
    _time_of_last_reading = millis();
  }
}

void Gc0011::readData() {
  // Read sensor
  _serial_port->print("Z\r\n");
  String data_string = _serial_port->readStringUntil(0x0A);

  // Check for failure
  if (data_string[1] != 'Z') {
    status_level = ERROR;
    status_code=  CODE_FAILED_TO_READ;
    status_msg = "Failed to read from sensor";
  }
  else { // good reading
    status_level = OK;
    status_code = CODE_OK;
    status_msg = "";
    _carbon_dioxide = (float)(data_string.substring(3,8).toInt());
    // _carbon_dioxide = round(_carbon_dioxide / 10) * 10;
    _send_carbon_dioxide = true;
  }
}
