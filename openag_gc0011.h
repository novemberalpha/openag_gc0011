/**
 *  \file openag_gc0011.h
 *  \brief Sensor module for co2 count parts per million
 */

#ifndef OPENAG_GC0011_H
#define OPENAG_GC0011_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

// 8 MHz(ish) AVR ---------------------------------------------------------
#if (F_CPU >= 7400000UL) && (F_CPU <= 9500000UL)
#define COUNT 3
// 16 MHz(ish) AVR --------------------------------------------------------
#elif (F_CPU >= 15400000UL) && (F_CPU <= 19000000L)
#define COUNT 6
#else
#error "CPU SPEED NOT SUPPORTED"
#endif

// how many timing transitions we need to keep track of. 2 * number bits + extra
#define MAXTIMINGS 85

#include <openag_module.h>
#include <std_msgs/Float32.h>
#include <SoftwareSerial.h>

/**
 *  \brief Sensor module for measuring ambient co2
 */
class Gc0011 : public Module {
  public:
    // Public Functions
    Gc0011(int rx_pin, int tx_pin);
    void begin();
    void update();
    bool get_carbon_dioxide(std_msgs::Float32 &msg);


  private:
    // Private Functions
    void getData();
    bool readSensor();

    // Private Variables
    int _rx_pin;
    int _tx_pin;
    float _carbon_dioxide;
    bool _send_carbon_dioxide;
    uint32_t _time_of_last_reading;
    const uint32_t _min_update_interval = 2000;
    SoftwareSerial *_my_serial;

    String val;
    String message;
    double multiplier;
    uint8_t buffer[25];
    uint8_t _ind;
    uint32_t _last_read_time;
    bool _first_reading;

    // Status codes
    static const uint8_t CODE_FAILED_TO_READ = 1;
};

#endif
