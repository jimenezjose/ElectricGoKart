/*******************************************************************************
                                                    Jose Jorge Jimenez-Olivas

File Name:       Speedometer.h
Description:     This class calculates the go-kart's speed and evaluates the 
                 driver's desired transmission state. It then sends this data 
                 to the go-kart's dashboard, i.e. Speedometer GUI.
*******************************************************************************/
#ifndef SPEEDOMETER_H
#define SPEEDOMETER_H

#include "Filter.h"

class Speedometer {
private:
    const int MAX_SPEED = 120;
    const int knobPin = A0;

    Filter filter;

    void displaySpeed();
    void displayTranmssion();

public:
    Speedometer() : filter(0.8) {}

    void init() {
        pinMode(knobPin, INPUT);
    }

    void run() {
        displaySpeed();
        displayTransmission();
    }
};

void Speedometer::displaySpeed() {
    long knobValue = filter.read(analogRead(knobPin));
    long speed = map(knobValue, 20, 994, 0, MAX_SPEED);
    Serial.print("knobValue: ");
    Serial.print(knobValue);
    Serial.print(", Speed: ");
    Serial.println(speed);
}

void Speedometer::displayTransmission() {
    // park
    // Serial.println("P");
    // Drive
    // Serial.println("D"); 
    // Reverse 
    // Serial.println("R);
}

#endif