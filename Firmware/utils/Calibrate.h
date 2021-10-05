#ifndef CALIBRATE_H
#define CALIBRATE_H

class Calibrate {
private:
public:
  Calibrate() {}

  /**
   * Calibrate analog pin with output analysis of pin.
   */
  void calibratePin(int pin) {
    Serial.println("Calibrating...");
    int minValue, maxValue;
    minValue = maxValue = analogRead(pin);
    unsigned long t0, t;
    t0 = t = millis();

    while(t < t0 + 5000) {
      // for 5 secs find min and max throttle value
      int value = analogRead(pin);
      if(value < minValue) minValue = value;
      if(value > maxValue) maxValue = value;
      t = millis();
    }

    Serial.print("Min: ");
    Serial.println(minValue);
    Serial.print("Max: ");
    Serial.println(maxValue);
  }
  
};

#endif