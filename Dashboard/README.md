# Speedometer GUI

## How to Run Dashboard GUI: 
(open your Terminal and run these following commands):
```
$ cd Go_Kart/Speedometer
$ ./runDashboardGUI.sh
```

Speedometer display interface used for the go-kart's dashboard. The speed of 
the go kart is fed through a peripheral microcontroller via serial
comminication.

![Alt Text](https://github.com/jimenezjose/Go_Kart/blob/assets/images/SpeedometerGUI%20screenshot.png)

#### My Rasberry pi configuration (for autostart)
For completness of documentation, this is how I run the dashboard GUI on startup.

Source of [Help](https://learn.sparkfun.com/tutorials/how-to-run-a-raspberry-pi-program-on-startup/all)

File: /home/pi/.config/autostart/speedometer.desktop
```
[Desktop Entry]
Type=Application
Name=Dashboard
Exec/home/pi/dashboard.sh
```

File: /home/pi/dashboard.sh
```
#!/bin/bash
export DISPLAY=:0.0
# manually bind to bluetooth module via mac address
# help: hcitool scan # to find mac address
sudo rfcomm bind /dev/rfcomm0 XX:XX:XX:XX:XX:XX
cd $HOME/Go_Kart/Speedometer
bash runDashboardGUI.sh
```

