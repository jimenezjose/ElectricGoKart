/*******************************************************************************
 * Jose Jimenez-Olivas 
 * Email: jjj023@ucsd.edu
 * 
 *                 University of California, San Diego
 *                              Go-Karts
 * 
 * File Name:       Dashboard.h
 * Description:     This class calculates the go-kart's speed and evaluates the 
 *                  driver's desired transmission state. It then sends this data 
 *                  to the go-kart's dashboard, i.e. Dashboard GUI.
*******************************************************************************/
#ifndef DASHBOARD_H
#define DASHBOARD_H

/*
 * Dashboard proxy that sends data packet to Dashboard GUI. 
 */
class Dashboard {
private:
    HardwareSerial & DASHBOARD_SERIAL; 
    int prevSpeedValue = 0;

public:
    const int MAX_SPEED = 120;
    const int MIN_SPEED = 0;
    
    /**
     * Dashboard constructor.
     * @param serial serial line that connects to dashboard GUI app.
     */
    Dashboard(HardwareSerial & serial) : DASHBOARD_SERIAL(serial) {}
    void displaySpeed(int);
    void displayTranmssion(char);
    void displayLockState(int);

    /**
     * Initializes dashboard dependencies such as BT serial line.
     */
    void init() {
        Serial1.println("Initializing Dashboard...");
    }

};

/**
 * Sends speed value to be displayed on speedometer.
 * @param speeedValue expected value from [0, 120].
 */
void Dashboard::displaySpeed(int speedValue) {
    if(prevSpeedValue == speedValue) return;
    prevSpeedValue = speedValue;
    DASHBOARD_SERIAL.print("Speed: ");
    DASHBOARD_SERIAL.println(speedValue);
    
}

/**
 * Sends transmission state to be displayed on dashboard.
 * @param transmissionState state of go kart transmission.
 */
void Dashboard::displayTranmssion(char transmissionState) {
    DASHBOARD_SERIAL.println(transmissionState);
}

/**
 * Displays lock symbol on dashboard.
 * @param lockState boolean value to display lock symbol on dashboard.
 */
void Dashboard::displayLockState(int lockState) {
    DASHBOARD_SERIAL.print("Locked: ");
    DASHBOARD_SERIAL.println(lockState);
}

#endif
