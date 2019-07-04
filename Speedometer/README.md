# Speedometer GUI

Speedometer display interface used for the go-kart's dashboard. The speed of 
the go kart is fed through a peripheral microcontroller via serial
comminication.

## Prelimary Step to Compile or Run Speedometer
```
cd Speedometer
```

## Compile Java Speedometer GUI

_/Speedometer_
```
$ make
```

## Run Java Speedometer GUI

_/Speedometer_
```
$ ./run.sh
```
If permission is denied; enter `chmod u+x run.sh; ./run.sh`. This command allows
run.sh to have executable permission from the user.

**OR**

_/Speedometer_
```
$ cd build
$ java -cp ../src/utility/lib/jSerialComm-2.5.1.jar:. SerialRoute
```
