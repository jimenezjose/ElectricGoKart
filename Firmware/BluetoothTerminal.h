#ifndef BLUETOOTH_TERMINAL_H
#define BLUETOOTH_TERMINAL_H

#define BUFFER_SIZE 81
#define BT_TX_DELAY 2

#define COMMANDS { "enable override", "disable override", "monitor speed", "monitor throttle", "disable monitor", "help", "" } 
#define COMMAND_FUNCS { &enableOverride, &disableOverride, &monitorSpeed, &monitorThrottle, &disableMonitor, &help }

/*
 * Bluetooth Terminal command line interface.
 */
class BluetoothTerminal {
  private:
    HardwareSerial & BT;
    char buffer[ BUFFER_SIZE ] = {0}; /* BT terminal message */
    struct {
      bool enabled;
      bool throttle;
      bool speed;
    } monitorSensors = {0};
    int & throttleSensor;
    int & speedSensor;
    bool overrideEnabled = false;

    /* terminal commands */
    void help();
    void monitorThrottle();
    void monitorSpeed();
    void disableMonitor();
    void enableOverride();
    void disableOverride();
    /* subroutines */
    void serialMonitor();
    void serialInput();
    bool interpretCmd(String);
    void clearSerialBuffer();

  public:
    /**
     * Bluetooth Terminal Constructor.
     * @param serial Bluetooth serial line
     */
    BluetoothTerminal(HardwareSerial & serial, int & throttle, int & speed) : BT(serial), 
        throttleSensor(throttle), speedSensor(speed) {}
    void init();
    void task(); 
    bool override(); 
};

/**
 * Initializes bluetooth terminal configuration.
 * @return Nothing.
 */
void BluetoothTerminal::init() {
  Serial1.println("Initializing Bluetooth Terminal...");
}

/**
 * Run command line interface service.
 */
void BluetoothTerminal::task() {
  serialMonitor();
  serialInput();
}

bool BluetoothTerminal::override() {
  return overrideEnabled;
}

/**
 * Live stream sensor monitor
 */
void BluetoothTerminal::serialMonitor() {
  if(!monitorSensors.enabled) return;

  if(monitorSensors.speed) {
    BT.print("Speed: ");
    BT.println(speedSensor);
  }
  if(monitorSensors.throttle) {
    BT.print("Throttle: ");
    BT.println(throttleSensor);
  }
}

/**
 * Parse and interpret recieving serial bluetooth data.
 */
void BluetoothTerminal::serialInput() {
  if(BT.available() == 0) return;

  /* incomming bluetooth data */
  int index = 0;
  while(BT.available() > 0) {
    /* read serial bluetooth bytes into buffer */
    if(index == BUFFER_SIZE - 1) {
      /* flush out the overflow of data */
      clearSerialBuffer();
      break;
    }
    buffer[index] = BT.read();
    if(buffer[index] == '\n') buffer[index] = '\0';
    index++;
    delay(BT_TX_DELAY);
  }
  /* null termination for string conversion */ 
  buffer[index] = '\0'; 
  String data = String(buffer);
  BT.print("\ncommand entered: ");
  BT.println(data);
  /* interpret bluetooth data */
  if(interpretCmd(data) == false) {
    /* notify user of invalid command */
    BT.println("Invalid Command");
  }
}

/**
 * Map data command to execute its corresponding function
 */
bool BluetoothTerminal::interpretCmd(String data) {
  const String command[] = COMMANDS;
  void (BluetoothTerminal::*commandFunc[])() = COMMAND_FUNCS;
  int index = 0;
  
  while(command[index] != "") {
    /* validate commands string */
    if(command[index].equals(data)) {
      /* executing command */
      (this->*commandFunc[index])();
      return true;
    }
    index++;
  }
  return false;
}

/**
 * Clears serial buffer residue
 */
void BluetoothTerminal::clearSerialBuffer() {
  while(BT.available() > 0) {
    BT.read();
  }
}

void BluetoothTerminal::enableOverride() {
  overrideEnabled = true;
}

void BluetoothTerminal::disableOverride() {
  overrideEnabled = false;
}


/**
 * Enable sensor to output readings to serial monitor
 */
void BluetoothTerminal::monitorThrottle() {
  monitorSensors.throttle = true;
  monitorSensors.enabled = true;
}
void BluetoothTerminal::monitorSpeed() {
  monitorSensors.speed = true;
  monitorSensors.enabled = true;
}

/**
 * Disable sensor output to serial monitor
 */
void BluetoothTerminal::disableMonitor() {
  memset(&monitorSensors, 0, sizeof(monitorSensors));
}

/**
 * TODO
 */ 
void BluetoothTerminal::help() {
  BT.println("\nTerminal Commands: (in progress)");
}

#endif
