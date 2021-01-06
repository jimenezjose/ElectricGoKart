/*******************************************************************************
                                                    Jose Jorge Jimenez-Olivas

File Name:       GoKart.ino
Description:     Main source file that manages the go-kart's electrical system. 
*******************************************************************************/
#include "Speedometer.h"

Speedometer speedometer;

void setup() {
    Serial.begin(9600);
    speedometer.init();
}

void loop() {
    speedometer.run();
}
