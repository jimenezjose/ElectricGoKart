#include "Speedometer.h"

Speedometer speedometer;

void setup() {
    Serial.begin(9600);
    speedometer.init();
}

void loop() {
    speedometer.run();
}
