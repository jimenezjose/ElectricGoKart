/*******************************************************************************
                                                    Jose Jorge Jimenez-Olivas

File Name:       Dashboard.h
Description:     This class calculates the go-kart's speed and evaluates the 
                 driver's desired transmission state. It then sends this data 
                 to the go-kart's dashboard, i.e. Dashboard GUI.
*******************************************************************************/
#ifndef DASHBOARD_H
#define DASHBOARD_H

#define DASHBOARD_SERIAL Serial1

#include "Filter.h"

class Dashboard {
private:
    const int MAX_SPEED = 120;
    const int throttle;
    const int lockSwitch;

    Filter filter;

    void displaySpeed();
    void displayTranmssion();
    void displayLockState();

public:
    Dashboard(const int throttle, const int lockSwitch) : filter(0.8), 
        throttle(throttle), lockSwitch(lockSwitch) {}

    void init() {
        DASHBOARD_SERIAL.begin(9600);
        pinMode(throttle, INPUT);
        pinMode(lockSwitch, INPUT);
    }

    void run() {
        displaySpeed();
        displayTranmssion();
        displayLockState();
    }
};

void Dashboard::displaySpeed() {
    long knobValue = filter.read(analogRead(throttle));
    long speedValue = map(knobValue, 0, 990, 0, MAX_SPEED);
    DASHBOARD_SERIAL.print("Speed: ");
    DASHBOARD_SERIAL.println(speedValue);
    Serial.print("Speed: ");
    Serial.println(speedValue);
}

void Dashboard::displayTranmssion() {
    // park
    // Serial.println("P");
    // Drive
    // Serial.println("D"); 
    // Reverse 
    // Serial.println("R);
}

void Dashboard::displayLockState() {
    int lockState = digitalRead(lockSwitch);
    DASHBOARD_SERIAL.print("Locked: ");
    DASHBOARD_SERIAL.println(lockState);
    Serial.print("Locked: ");
    Serial.println(lockState);
}

#endif
