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

#include <Adafruit_MCP4725.h> 
#include "utils/Filter.h"

#include <Wire.h>

/*
 * Interface to the motor controller using a DAC from 0-3.6V for 0-100% throttle
 */
class Motor {
private:
  const int MIN_THROTTLE;
  const int MAX_THROTTLE;
  Adafruit_MCP4725 dac;
  Filter filter;
  int throttle = 0;
  int vout = 0;

public:
  // 0..4095 dac SDA
  // 3200, 80% -> 3.66V, ~3.5V
  const int MIN_VOUT = 0;
  const int MAX_VOUT = ((int)(0.8 * 4095));
  
  /*
   * Motor Constructor
   * @note internal DAC uses I2C
   */
  Motor(int minThrottle, int maxThrottle) : filter(0.8), MIN_THROTTLE(minThrottle), MAX_THROTTLE(maxThrottle) {}
  void setThrottle(int);
  void setVout(int);
  int getThrottle();
  int getVout();

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
 */
void Motor::setThrottle(int throttleValue) {
  if(throttleValue < MIN_THROTTLE) { 
    throttleValue = MIN_THROTTLE;
  }
  else if(throttleValue > MAX_THROTTLE) { 
    throttleValue = MAX_THROTTLE;
  }
  throttle = filter.read(throttleValue);
  vout = map(throttleValue, MIN_THROTTLE, MAX_THROTTLE, MIN_VOUT, MAX_VOUT);
  dac.setVoltage(vout, false);
}

/**
 * Sets voltage for external DAC. For input range see MIN_VOUT, MAX_VOUT.
 * @param voltage Volatage input for external DAC module.
 */
void Motor::setVout(int voltage) {
  if(voltage < MIN_VOUT) {
    voltage = MIN_VOUT;
  }
  else if(voltage > MAX_VOUT) {
    voltage = MAX_VOUT;
  }
  vout = voltage;
  dac.setVoltage(vout, false);
}

/**
 * Getter for internal throttle value.
 */
int Motor::getThrottle() {
  return throttle;
}

/**
 * Getter for current voltage to external DAC.
 */
int Motor::getVout() {
  return vout;
}

#endif
