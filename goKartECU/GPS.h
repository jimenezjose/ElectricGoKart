/*******************************************************************************
 * Jose Jimenez-Olivas 
 * Email: jjj023@ucsd.edu
 * 
 *                 University of California, San Diego
 *                              Go-Kart
 * 
 * File Name:       GPS.ino
 * Description:     Wrapper for gps driver to easily query speed.
*******************************************************************************/
#ifndef GPS_H
#define GPS_H

#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include "utils/Filter.h"

/* 
 * GPS wrapper that handles parsing gps data
 */
class GPS {

private:
  Filter speedFilter;
  int speed_mph = 0;

public:
  TinyGPS gps;
  SoftwareSerial gpsSerial;

  /**
   * GPS constructor
   * @param rx Serial RX
   * @param tx Serial TX
   */
  GPS(int rx, int tx) : gpsSerial(rx, tx), speedFilter(0) {}
  int get_speed_mph();


  /**
   * Sets up serial line between gps module and MCU.
   * @return Nothing.
   */
  void init() {
    Serial1.println("Initializing GPS...");
    while(!gpsSerial); /* wait for gps serial port to open */
    gpsSerial.begin(9600);
  }

};

/**
 * Accessor method to retrieve the current gps speed.
 * @return speed in mph.
 */
int GPS::get_speed_mph() {
    if(gpsSerial.available()) {
        char gps_data = gpsSerial.read();
        if(gps.encode(gps_data)) {
            /* parse gps speed data */
            speed_mph = speedFilter.read(gps.f_speed_mph());
        }
    }
    return speed_mph;
}

#endif
