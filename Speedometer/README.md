# Speedometer GUI

## Compile Java Speedometer GUI

__/Speedometer__
```
make
```

## Run Java Speedometer GUI

__/Speedometer__
```
./run.sh
```
If permission is denied to run the command enter `chmod u+x run.sh` to allow
run.sh to have executable permission to the user. Then try again: `./run.sh`

*OR*

__/Speedometer__
```
cd build
java -cp ../src/utility/lib/jSerialComm-2.5.1.jar:. SerialRoute
```
