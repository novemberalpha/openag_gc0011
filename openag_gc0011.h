/**
 *  \file openag_gc0011.h
 *  \brief Carbon Dioxide Sensor for PFC1 BOM
 */

#ifndef OPENAG_GC0011_H
#define OPENAG_GC0011_H

#include "Arduino.h"
#include <openag_module.h>
#include <std_msgs/Float32.h>
#include <SoftwareSerial.h>

/**
 *  \brief Sensor module for measuring ambient co2
 */
class Gc0011 : public Module {
  public:
    Gc0011(int rx_pin, int tx_pin);
 
    // Public Functions
    void begin();
    void update();
    bool get_air_carbon_dioxide(std_msgs::Float32 &msg);


  private:
    // Private Variables 
    float _carbon_dioxide;
    bool _send_carbon_dioxide;
    uint32_t _time_of_last_reading;
    const uint32_t _min_update_interval = 2000;
    SoftwareSerial *_my_serial;
 
    // Private functions
    void readData();

    // Status codes
    static const uint8_t CODE_FAILED_TO_READ = 1;
};

#endif
