# Speedometer GUI

## How to Run Dashboard GUI: 
```
$ cd Go_Kart/Speedometer
$ ./runDashboardGUI.sh
```

## Dashboard Display

    The speed of the go kart is fed through a peripheral microcontroller via serial comminication. A Raspberry Pi 3 
    is hosting the Dashboard GUI and reading serial port messages from a microcontroller.

![Alt Text](https://github.com/jimenezjose/Go_Kart/blob/assets/images/SpeedometerGUI%20screenshot.png)

#### My Rasberry PI Configuration
    For completness of documentation, this is how I run the Raspberrt Pi dashboard GUI on startup.

    Source of [Help](https://learn.sparkfun.com/tutorials/how-to-run-a-raspberry-pi-program-on-startup/all)

File: /home/pi/.config/autostart/dashboard.desktop
```desktop
[Desktop Entry]
Type=Application
Name=Dashboard
Exec=/home/pi/dashboard.sh
```
Note: dashboard.sh must be executable to the user. "chmod u+x /home/pi/dashboard.sh"

File: /home/pi/dashboard.sh
```bash
#!/bin/bash
export DISPLAY=:0.0
# how to bind HC-05 to raspberry pi: https://dev.to/ivanmoreno/how-to-connect-raspberry-pi-with-hc-05-bluetooth-module-arduino-programm-3h7a
# manually bind to bluetooth module via mac address. use "hcitool scan" to find mac address  
sudo rfcomm bind /dev/rfcomm0 XX:XX:XX:XX:XX:XX
cd $HOME/Go_Kart/Dashboard
bash runDashboardGUI.sh
```

