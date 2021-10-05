/*******************************************************************************
 * Jose Jimenez-Olivas 
 * Email: jjj023@ucsd.edu
 * 
 *                 University of California, San Diego
 *                              Go-Kart
 * File Name:       Motor.ino
 * Description:     API for kart's throttle system. 
*******************************************************************************/
#ifndef MOTOR_H 
#define MOTOR_H

// 0..4095 dac SDA
// 3200, 80% -> 3.66V, ~3.5V
#define MIN_VOUT 0
#define MAX_VOUT ((int)(0.8 * 4095))

#include <Adafruit_MCP4725.h>
#include "utils/Filter.h"

/*
 * Interface to the motor controller using a DAC from 0-3.6V for 0-100% throttle
 */
class Motor {
private:
  int MIN_THROTTLE = 160;
  int MAX_THROTTLE = 885;

  Adafruit_MCP4725 dac;
  Filter filter;

public:
  
  /*
   * Motor Constructor
   * @note internal DAC uses I2C
   */
  Motor() : filter(0.8) {}
  void setThrottle(int);
  void setMinThrottle(int);
  void setMaxThrottle(int);

  /**
   * Sets up I2C prototocol from MCU to DAC with id 0x60.
   */
  void init() {
    // For Adafruit MCP4725A1 the address is 0x62 (default) or 0x63 (ADDR pin tied to VCC)
    // For MCP4725A0 the address is 0x60 or 0x61
    // For MCP4725A2 the address is 0x64 or 0x65
    Serial1.println("Initializing Motor...");
    dac.begin(0x60);
  }

};

/**
 * Reads throttle and excites the throttle motor accordingly.
 * @param throttleValue readings from a pedal potentiometer.
 * @return Nothing.
 */
void Motor::setThrottle(int throttleValue) {
    throttleValue = filter.read(throttleValue);
    int vout = map(throttleValue, MIN_THROTTLE, MAX_THROTTLE, MIN_VOUT, MAX_VOUT);
    dac.setVoltage(vout, false);
}

/**
 * Sets min potentiometer throttle readings.
 * @param value min throttle reading.
 * @return Nothing.
 */
void Motor::setMinThrottle(int value) {
    MIN_THROTTLE = value;
}

/**
 * Sets max potentiometer throttle readings.
 * @param value max throttle reading.
 * @return Nothing.
 */
void Motor::setMaxThrottle(int value) {
    MAX_THROTTLE = value;
}

#endif
