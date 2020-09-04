#!/bin/bash

#This script will create a folder suited for the arudino IDE.
#Just run this script and then open the folder inside the Arduino_IDE folder called "uart-spi-converter" with the Arduino IDE

rm -rf ./Arduino_IDE/
mkdir Arduino_IDE
mkdir ./Arduino_IDE/uart-spi-converter
cp ./src/main.cpp ./Arduino_IDE/uart-spi-converter/uart-spi-converter.ino
