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
const int tranmissionSwitch = 6;
const int keySwitch = A1;
int throttle = 0;
int speed_mph = 0;
int voltageToMotor = 0;
int isLocked = false;
Transmission transmission = Transmission::DRIVE;

BaseStation baseStation(Serial1, throttle, speed_mph, voltageToMotor, isLocked, transmission);
Dashboard dashboard(Serial1); 
Motor motor(MIN_THROTTLE, MAX_THROTTLE);
GPS gps(8, 7); /* RX, TX */

void setup() {
  while(!Serial1);
  Serial1.begin(9600);
  Serial1.println("Go-Kart Software");
  Serial1.println("By: Jose Jimenez-Olivas\n");

  pinMode(pedal, INPUT);
  pinMode(tranmissionSwitch, INPUT_PULLUP);
  pinMode(keySwitch, INPUT_PULLUP);
  motor.init();
  gps.init();
  dashboard.init();
  baseStation.init();
  
  Serial1.println("OK.");
  baseStation.displayUsage();
}

void loop() {
  baseStation.task();
  // TODO (github/jimenezjose): GPS task to collect entire suite of attributes from a single GNSS message.

  /* inputs */
  isLocked = !digitalRead(keySwitch) || baseStation.isLocked();
  throttle = analogRead(pedal);
  speed_mph = gps.get_speed_mph();
  transmission = digitalRead(tranmissionSwitch) ? Transmission::DRIVE : Transmission::REVERSE;

  /* outputs */
  if(isLocked) {
    emergencyBrake();
  }
  else if(baseStation.getMode() == BaseStationMode::MANUAL) {
    motor.setTransmission(transmission);
    motor.setThrottle(throttle);
  }
  else if(baseStation.getMode() == BaseStationMode::OVERRIDE) {
    motor.setThrottle(0);
  }
  
  voltageToMotor = motor.getVout();
  
  /* Display dashboard graphics. */
  dashboard.displaySpeed(speed_mph);
  dashboard.displayLockState(isLocked);
  dashboard.displayTransmission(transmission);
}

/**
 * Forcibly set actuators to their rest state.
 */
void emergencyBrake() {
  motor.setThrottle(0);
}
