#ifndef BLUETOOTH_TERMINAL_H
#define BLUETOOTH_TERMINAL_H

#include "motor.h"

#define BUFFER_SIZE 81
#define BT_TX_DELAY 2

#define BASE_STATION_USAGE "\n\
------------------------------ Base Station Commands ------------------------------\n\
\n\
monitor speed              - View GPS speed output.\n\
monitor throttle           - View throttle sensor output.\n\
monitor voltageToMotor     - View voltage output to motor.\n\
monitor lockState          - View go kart lock state.\n\
monitor transmission       - View motor transmission state.\n\
\n\
disable speed              - Silence speed output.\n\
disable throttle           - Silence throttle output.\n\
disable voltageToMotor     - Silence voltageToMotor output.\n\
disable lockState          - Silence lock state output.\n\
disable transmission       - Silence motor transmission state output.\n\
disable monitor            - Silence output.\n\
\n\
(WARNING: Use reponsibly.)\n\
sudo manual                - Default human drivable go kart.\n\
sudo autonomous            - Let the go kart drive itself.\n\
sudo override              - Grant yourself complete control of ECU.\n\
sudo set speed [num]       - Sets speed value forcefully. (override required)\n\
sudo set throttle [num]    - Sets throttle value forcefully. (override required)\n\
sudo kill                  - Drops all privileges of ECU back to manual mode.\n\
\n\
help                       - Usage on base station commands.\n\
\n\
-----------------------------------------------------------------------------------\n\
"

/** Base Station mode definitions. */
enum class BaseStationMode {
  MANUAL = 0,
  AUTONOMOUS = 1,
  OVERRIDE = 2,
};

/**
   Base station command line interface.

   HOW:
   To add a new field, say `x`, to monitor you must do the following:
     1. Add new input parameter `int & x` to constructor.
     2. Add new private field, `int & xSensor`.
     3. Initialize new sensor in initializer list: `BaseStation(..., int & x) : ..., xSensor(x)`.
     4. Add `monitor x` and `disable x` to `COMMANDS`.
     5. RESPECTIVELY, add `&BaseStation::monitorX` and `&BaseStation::disableX` to `COMMAND_FUNCS`.
     6. Declare `void monitorX()` and `void disableX()`.
     7. Add `bool x`, in `struct monitorSensors`.
     8. Define `monitorX()` and `disableX()` by setting `monitorSensors.x` to true/false.
     9. In function `serialOutputService` add correspoding serial output for sensor x readings!
     10. Done.
*/
class BaseStation {
  public:
    BaseStation(HardwareSerial & serial, int & throttle, int & speed, int & voltageToMotor, int & lockState, Transmission & trans) :
      BT(serial), throttleSensor(throttle), speedSensor(speed),
      voltageToMotorSensor(voltageToMotor), mode(BaseStationMode::MANUAL),
      lockState(lockState), transmission_(trans) {}

    void init();
    void task();
    void displayUsage();
    BaseStationMode getMode();
    int isLocked();

  private:
    String COMMANDS[16] = {
      "monitor speed",
      "monitor throttle",
      "monitor voltageToMotor",
      "monitor lockState",
      "monitor transmission",
      "disable speed",
      "disable throttle",
      "disable voltageToMotor",
      "disable lockState",
      "disable transmission",
      "disable monitor",
      "sudo manual",
      "sudo autonomous",
      "sudo override",
      "sudo kill",
      "help"
    };
    String PRIVELEGED_COMMANDS[2] = {
      "sudo set speed %d",
      "sudo set throttle %d"
    };
    void (BaseStation::*COMMAND_FUNCS[16])() = {
      &BaseStation::monitorSpeed,
      &BaseStation::monitorThrottle,
      &BaseStation::monitorVoltageToMotor,
      &BaseStation::monitorLockState,
      &BaseStation::monitorTransmission,
      &BaseStation::disableSpeed,
      &BaseStation::disableThrottle,
      &BaseStation::disableVoltageToMotor,
      &BaseStation::disableLockState,
      &BaseStation::disableTransmission,
      &BaseStation::disableMonitor,
      &BaseStation::setManualMode,
      &BaseStation::setAutonomousMode,
      &BaseStation::setOverrideMode,
      &BaseStation::kill,
      &BaseStation::help
    };

    HardwareSerial & BT;
    char buffer[ BUFFER_SIZE ] = {0}; /* input serial buffer */
    struct {
      bool throttle;
      bool speed;
      bool voltageToMotor;
      bool lockState;
      bool transmission;
    } monitorSensors = {0};
    int & throttleSensor;
    int & speedSensor;
    int & voltageToMotorSensor;
    int & lockState;
    Transmission & transmission_;
    BaseStationMode mode;

    /* base station commands */
    void monitorSpeed();
    void monitorThrottle();
    void monitorVoltageToMotor();
    void monitorLockState();
    void monitorTransmission();
    void disableSpeed();
    void disableThrottle();
    void disableVoltageToMotor();
    void disableLockState();
    void disableTransmission();
    void disableMonitor();
    void setManualMode();
    void setAutonomousMode();
    void setOverrideMode();
    void kill();
    void overrideHelp();
    void help();

    /* privileged base station commands (requires `sudo override`) */
    void setThrottle(int value);
    void setSpeed(int value);

    /* subroutines */
    void serialInputService();
    void serialOutputService();
    bool interpretCmd(String);
    void clearSerialBuffer();
};

/** Initializes base station configuration. */
void BaseStation::init() {
  Serial1.println("Initializing BaseStation...");
}

/** Run command line interface service. */
void BaseStation::task() {
  serialInputService();
  serialOutputService();
}

/** Displays Base Station Commands */
void BaseStation::displayUsage() {
  help();
}

/** Getter for base station mode. */
BaseStationMode BaseStation::getMode() {
  return mode;
}

/** Getter for lock state. */
int BaseStation::isLocked() {
  return getMode() == BaseStationMode::OVERRIDE;
}

/** Default human drivable mode. */
void BaseStation::setManualMode() {
  BT.println("BaseStation mode: MANUAL");
  mode = BaseStationMode::MANUAL;
}

/** Allows go kart to drive itself. */
void BaseStation::setAutonomousMode() {
  BT.println("BaseStation mode: AUTONOMOUS");
  mode = BaseStationMode::AUTONOMOUS;
}

/** Grant base station complete control of ECU. */
void BaseStation::setOverrideMode() {
  BT.println("BaseStation mode: OVERRIDE");
  mode = BaseStationMode::OVERRIDE;
}

/** Drop all ECU priveleges back to manual mode. */
void BaseStation::kill() {
  BT.println("BaseStation mode: MANUAL");
  mode = BaseStationMode::MANUAL;
}

/** Output speed sensor readings to console. */
void BaseStation::monitorSpeed() {
  monitorSensors.speed = true;
}

/** Output throttle sensor readings to console. */
void BaseStation::monitorThrottle() {
  monitorSensors.throttle = true;
}

/** Output voltageToMotor sensor readings to console. */
void BaseStation::monitorVoltageToMotor() {
  monitorSensors.voltageToMotor = true;
}

/** Output lockState sensor readings to console. */
void BaseStation::monitorLockState() {
  monitorSensors.lockState = true;
}

/** Output transmission state to console. */
void BaseStation::monitorTransmission() {
  monitorSensors.transmission = true;
}

/** Disable all sensor output to console. */
void BaseStation::disableMonitor() {
  memset(&monitorSensors, 0, sizeof(monitorSensors));
}

/** Disable speed output to console. */
void BaseStation::disableSpeed() {
  monitorSensors.speed = false;
}

/** Disable throttle sensor output to console. */
void BaseStation::disableThrottle() {
  monitorSensors.throttle = false;
}

/** Disable votageToMotor sensor output to console. */
void BaseStation::disableVoltageToMotor() {
  monitorSensors.voltageToMotor = false;
}

/** Disable lockState output to console. */
void BaseStation::disableLockState() {
  monitorSensors.lockState = false;
}

/** Disable transmission state output to console. */
void BaseStation::disableTransmission() {
  monitorSensors.transmission = false;
}

/** Prints priveleged command usage to user. */
void BaseStation::overrideHelp() {
  BT.println("not implemented yet...");
}

/** Prints BaseStation usage to console. */
void BaseStation::help() {
  BT.println(F(BASE_STATION_USAGE));
}

/** Live stream sensor monitor */
void BaseStation::serialOutputService() {
  if (monitorSensors.speed) {
    BT.print("Speed: ");
    BT.println(speedSensor);
  }
  if (monitorSensors.throttle) {
    BT.print("Throttle: ");
    BT.println(throttleSensor);
  }
  if (monitorSensors.voltageToMotor) {
    BT.print("VoltageToMotor: ");
    BT.println(voltageToMotorSensor);
  }
  if (monitorSensors.lockState) {
    BT.print("LockState: ");
    BT.println(lockState);
  }
  if (monitorSensors.transmission) {
    BT.print("Transmission: ");
    switch (transmission_) {
      case Transmission::PARK:
        BT.println("P");
        break;
      case Transmission::REVERSE:
        BT.println("R");
        break;
      case Transmission::DRIVE:
        BT.println("D");
        break;
    }
  }
}

/** Parse and interpret recieving serial bluetooth data. */
void BaseStation::serialInputService() {
  if (BT.available() == 0) return;

  /* incomming bluetooth data */
  int index = 0;
  while (BT.available() > 0) {
    /* read serial bluetooth bytes into buffer */
    if (index == BUFFER_SIZE - 1) {
      /* flush out the overflow of data */
      clearSerialBuffer();
      break;
    }
    buffer[index] = BT.read();
    if (buffer[index] == '\n') buffer[index] = '\0';
    index++;
    delay(BT_TX_DELAY);
  }
  /* null termination for string conversion */
  buffer[index] = '\0';
  String data = String(buffer);
  data.trim();
  BT.print("\ncommand entered: ");
  BT.println(data);
  /* interpret bluetooth data */
  if (interpretCmd(data) == false) {
    /* notify user of invalid command */
    BT.println("Invalid Command");
  }
}

/** Map data command to execute its corresponding function. */
bool BaseStation::interpretCmd(String data) {
  int index = 0;
  while (COMMANDS[index] != "") {
    /* validate commands string */
    if (COMMANDS[index].equals(data)) {
      /* executing command */
      (this->*COMMAND_FUNCS[index])();
      return true;
    }
    index++;
  }

  return false;
}

/** Clears serial buffer residue. */
void BaseStation::clearSerialBuffer() {
  while (BT.available() > 0) {
    BT.read();
    delay(BT_TX_DELAY);
  }
}

#endif
