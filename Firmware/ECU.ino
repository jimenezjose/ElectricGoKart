/*******************************************************************************
                                                    Jose Jorge Jimenez-Olivas

File Name:       ECU.ino
Description:     Main source file that manages the go-kart's electrical system. 
*******************************************************************************/

#include "Dashboard.h"

const int throttle = A0;
const int lockSwitch = 6;

Dashboard dashboard(throttle, lockSwitch);

void setup() {
    dashboard.init();
}

void loop() {
    dashboard.run();
}
