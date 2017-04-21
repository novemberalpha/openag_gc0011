/**
 *  \file openag_gc0011.cpp
 *  \brief Sensor module for co2 measurement.
 *  \details See openag_gc0011.h for details.
 */
#include "openag_gc0011.h"

Gc0011::Gc0011(int rx_pin, int tx_pin) {
  _rx_pin = rx_pin;
  _tx_pin = tx_pin;
  status_level = OK;
  status_code = CODE_OK;
  status_msg = "";
  val= ""; //holds the string of the value
  multiplier = 1; //each range of sensor has a different value.
      // up to 2% =1
      // up to 65% = 10
      //up to 100% = 100;
  _ind=0;
  SoftwareSerial _my_serial(_rx_pin, _tx_pin); //connect with sensor
}

void Gc0011::begin() {
  _my_serial->begin(9600);
  delay(500);
  _my_serial->println("K 2"); // Set to polling mode
  delay(500);
  _first_reading = true;
  _time_of_last_reading = 0;
}

void Gc0011::update() {
  if (millis() - _time_of_last_reading > _min_update_interval) {
    getData();
    _time_of_last_reading = millis();
  }
}

bool Gc0011::get_carbon_dioxide(std_msgs::Float32 &msg) {
  msg.data = _carbon_dioxide;
  bool res = _send_carbon_dioxide;
  _send_carbon_dioxide = false;
  return res;
}


void Gc0011::getData(void) {
  if (readSensor()) {
    if (status_level != OK) {
      status_level = OK;
      status_code = CODE_OK;
      status_msg = "";
    }

    _carbon_dioxide = (multiplier * val.toInt());
    _send_carbon_dioxide = true;
  }
  else {
    status_level = ERROR;
    status_code = CODE_FAILED_TO_READ;
    status_msg = "Failed to read from sensor";
  }
}

bool Gc0011::readSensor() {
  uint8_t last_state = HIGH;
  uint8_t counter = 0;
  uint8_t i;
  unsigned long current_time;
  _carbon_dioxide = 0;
  current_time = millis();
  if (current_time < _last_read_time) {
    // ie there was a rollover
    _last_read_time = 0;
  }
  if (!_first_reading && ((current_time - _last_read_time) < 2000)) {
    return true; // return last correct measurement
  }
  _first_reading = false;
  _last_read_time = millis();

  _my_serial->println("Z");
  //We read incoming bytes into a buffer until we get '0x0A' which is the ASCII value for new-line
  while(buffer[_ind-1] != 0x0A)
  {
    if(_my_serial->available())
    {
      buffer[_ind] = _my_serial->read();
      _ind++;
    }
  }

  /*
    Cycle through the buffer and send out each byte including the final linefeed
    each packet in the stream looks like "Z 00400 z 00360"
    'Z' lets us know its a co2 reading. the first number is the filtered value
    and the number after the 'z' is the raw value.
    We are really only interested in the filtered value
  */

   for(int i=0; i < _ind+1; i++) {
      if(buffer[i] == 'z') //once we hit the 'z' we can stop
      break;
      if((buffer[i] != 0x5A)&&(buffer[i] != 0x20)) //ignore 'Z' and white space
      {
        val += buffer[i]-48; //because we break at 'z' the only bytes getting added are the numbers
        // we subtract 48 to get to the actual numerical value
        // example the character '9' has an ASCII value of 57. [57-48=9]
      }
   }

   _carbon_dioxide = (multiplier * val.toInt()); //now we multiply the value by a factor specific ot the sensor. see the Cozir software guide
   _ind=0; //Reset the buffer index to overwrite the previous packet
   val=""; //Reset the value string

  // check we read a value
  if (_carbon_dioxide > 0) {
    _send_carbon_dioxide = true;
    return true;
  }
  return false;
}
