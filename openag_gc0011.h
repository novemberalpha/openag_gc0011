/**
 *  \file openag_gc0011.h
 *  \brief Sensor module for co2 count parts per million or percentage.
 */

#ifndef OPENAG_GC0011_H
#define OPENAG_GC0011_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <SoftwareSerial.h>

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

/**
 *  \brief Sensor module for air temperature and humidity.
 */
class Gc0011 : public Module {
  public:
    // Public Functions
    Gc0011(int rx_pin, int tx_pin);
    void begin();
    void update();
    bool get_co2(stdmsgs::Float32 &msg);

  private:
    // Private Functions
    void getData();
    bool readSensor();

    // Private Variables
    String val;
    double multiplier;
    uint8_t buffer[25];
    uint8_t ind;
    
    int _rx_pin;
    int _tx_pin;
    float _co2;
    bool _send_co2;
    uint32_t _time_of_last_reading;
    const uint32_t _min_update_interval = 2000;

    uint8_t _data[6];
    uint8_t _count;
    uint32_t _last_read_time;
    bool _first_reading;

    // Status codes
    static const uint8_t CODE_FAILED_TO_READ = 1;
};

#endif
