/*******************************************************************************
 * Jose Jimenez-Olivas 
 * Email: jjj023@ucsd.edu
 * 
 *                 University of California, San Diego
 *                              Go-Kart
 * 
 * File Name:       goKartECU.ino
 * Description:     Manages the go-kart's electrical system. 
*******************************************************************************/
#include "BaseStation.h"
#include "Dashboard.h"
#include "Motor.h"
#include "GPS.h"
#include "utils/Filter.h"

#define MIN_THROTTLE 160
#define MAX_THROTTLE 885

const int pedal = A0;
const int lockSwitch = 6;
int throttle = 0;
int speed_mph = 0;
int voltageToMotor = 0;

BaseStation baseStation(Serial1, throttle, speed_mph, voltageToMotor);
Dashboard dashboard(Serial1); 
Motor motor(MIN_THROTTLE, MAX_THROTTLE);
GPS gps(8, 7); /* RX, TX */

void setup() {
  while(!Serial1);
  Serial1.begin(9600);
  Serial1.println("Go-Kart Software");
  Serial1.println("By: Jose Jimenez-Olivas\n");

  motor.init();
  gps.init();
  dashboard.init();
  baseStation.init();
  
  Serial1.println("OK.");
  baseStation.displayUsage();
}

void loop() {
  /* inputs */
  baseStation.task();
  throttle = analogRead(pedal);
  speed_mph = gps.get_speed_mph();

  /* outputs */
  if(baseStation.getMode() == BaseStationMode::MANUAL) {
    motor.setThrottle(throttle);
    dashboard.displaySpeed(speed_mph);
  }
  else if(baseStation.getMode() == BaseStationMode::OVERRIDE) {
    motor.setThrottle(0);
    dashboard.displaySpeed(speed_mph);
  }

  /* sensor outputs */
  voltageToMotor = motor.getVout();
}

/**
 * Forcibly set actuators to their rest state.
 */
void emergencyBrake() {
  throttle = 0;
  speed_mph = 0;
  motor.setThrottle(throttle); 
  dashboard.displaySpeed(speed_mph); 
}
