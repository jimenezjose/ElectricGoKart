#!/bin/bash
MAIN='SerialRoute'
BUILD_DIR='build'
JAR_SOURCES='src/utility/lib/jSerialComm-2.5.1.jar'

make
cd $BUILD_DIR
java -cp ../$JAR_SOURCES:. $MAIN
