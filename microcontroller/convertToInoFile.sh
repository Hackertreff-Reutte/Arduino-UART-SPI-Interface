#!/bin/bash

#This script will create a folder suited for the arudino IDE.
#Just run this script and then open the folder inside the Arduino_IDE folder called "uart_spi_converter" with the Arduino IDE

rm -rf ./Arduino_IDE/
mkdir Arduino_IDE
mkdir ./Arduino_IDE/uart_spi_converter
touch ./Arduino_IDE/uart_spi_converter/uart_spi_converter.ino
echo "//This is a guard so that the code works with the Arduino IDE !!!Do not remove!!!" &>> ./Arduino_IDE/uart_spi_converter/uart_spi_converter.ino
echo "#define ARDUINO_IDE" &>> ./Arduino_IDE/uart_spi_converter/uart_spi_converter.ino
echo "" &>> ./Arduino_IDE/uart_spi_converter/uart_spi_converter.ino

#add the settings to the arduino file
echo "" &>> ./Arduino_IDE/uart_spi_converter/uart_spi_converter.ino
echo "" &>> ./Arduino_IDE/uart_spi_converter/uart_spi_converter.ino
echo "//---------Start of the contents from the file settings.h------------" &>> ./Arduino_IDE/uart_spi_converter/uart_spi_converter.ino
echo "" &>> ./Arduino_IDE/uart_spi_converter/uart_spi_converter.ino
echo "" &>> ./Arduino_IDE/uart_spi_converter/uart_spi_converter.ino
echo "//In this section can you tweak the settings a bit:" &>> ./Arduino_IDE/uart_spi_converter/uart_spi_converter.ino
echo "" &>> ./Arduino_IDE/uart_spi_converter/uart_spi_converter.ino

cat ./src/settings.h &>> ./Arduino_IDE/uart_spi_converter/uart_spi_converter.ino

echo "" &>> ./Arduino_IDE/uart_spi_converter/uart_spi_converter.ino
echo "//---------End of the contents from the file settings.h------------" &>> ./Arduino_IDE/uart_spi_converter/uart_spi_converter.ino
echo "" &>> ./Arduino_IDE/uart_spi_converter/uart_spi_converter.ino
echo "" &>> ./Arduino_IDE/uart_spi_converter/uart_spi_converter.ino
echo "" &>> ./Arduino_IDE/uart_spi_converter/uart_spi_converter.ino


#loop through all the files and add them to the newly created ino file 
FILES=./src/*
for file in $FILES
do
  if [ ${file: -4} == ".cpp" ]
  then
	if [ $file == "./src/main.cpp" ]
	then
		MAIN=$file;
	else
		echo "" &>> ./Arduino_IDE/uart_spi_converter/uart_spi_converter.ino
		echo "//---------Start of the contents from the file $file------------" &>> ./Arduino_IDE/uart_spi_converter/uart_spi_converter.ino
		echo "" &>> ./Arduino_IDE/uart_spi_converter/uart_spi_converter.ino
		cat $file &>> ./Arduino_IDE/uart_spi_converter/uart_spi_converter.ino
		echo "" &>> ./Arduino_IDE/uart_spi_converter/uart_spi_converter.ino
		echo "//---------End of the contents from the file $file------------" &>> ./Arduino_IDE/uart_spi_converter/uart_spi_converter.ino
		echo "" &>> ./Arduino_IDE/uart_spi_converter/uart_spi_converter.ino
		echo "" &>> ./Arduino_IDE/uart_spi_converter/uart_spi_converter.ino
		echo "" &>> ./Arduino_IDE/uart_spi_converter/uart_spi_converter.ino
	fi
  fi
  
done

echo "" &>> ./Arduino_IDE/uart_spi_converter/uart_spi_converter.ino
echo "//---------Start of the contents from the file $MAIN------------" &>> ./Arduino_IDE/uart_spi_converter/uart_spi_converter.ino
echo "" &>> ./Arduino_IDE/uart_spi_converter/uart_spi_converter.ino
cat $MAIN &>> ./Arduino_IDE/uart_spi_converter/uart_spi_converter.ino
echo "" &>> ./Arduino_IDE/uart_spi_converter/uart_spi_converter.ino
echo "//---------End of the contents from the file $MAIN------------" &>> ./Arduino_IDE/uart_spi_converter/uart_spi_converter.ino
echo "" &>> ./Arduino_IDE/uart_spi_converter/uart_spi_converter.ino
echo "" &>> ./Arduino_IDE/uart_spi_converter/uart_spi_converter.ino
echo "" &>> ./Arduino_IDE/uart_spi_converter/uart_spi_converter.ino
