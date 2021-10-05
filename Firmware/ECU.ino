/*******************************************************************************
 * Jose Jimenez-Olivas 
 * Email: jjj023@ucsd.edu
 * 
 *                 University of California, San Diego
 *                              Go-Kart
 * 
 * File Name:       ECU.ino
 * Description:     Manages the go-kart's electrical system. 
*******************************************************************************/
#include "BluetoothTerminal.h"
#include "Dashboard.h"
#include "Motor.h"
#include "GPS.h"
#include "utils/Filter.h"

const int pedal = A0;
const int lockSwitch = 6;

int throttle = 0;
int speed_mph = 0;

BluetoothTerminal bluetoothTerminal(Serial1, throttle, speed_mph);
Dashboard dashboard(Serial1); 
Motor motor;
GPS gps(8, 7); /* RX, TX */

void setup() {
    while(!Serial1);
    Serial1.begin(9600);
    Serial1.println("Go-Kart Software");
    Serial1.println("By: Jose Jimenez-Olivas\n");

    motor.init();
    gps.init();
    dashboard.init();
    bluetoothTerminal.init();
}

void loop() {
    /* inputs */
    bluetoothTerminal.task();
    throttle = analogRead(pedal);
    speed_mph = gps.get_speed_mph();

    /* outputs */
    if(bluetoothTerminal.override()) {
      ;
    }
    else {
      /* autonomous drive */
      motor.setThrottle(throttle);
      // display speed on dashboard - (10Hz) hardware interupt with timer
      dashboard.displaySpeed(speed_mph);
    }
}
