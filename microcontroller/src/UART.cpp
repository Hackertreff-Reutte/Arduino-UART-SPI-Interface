/*
        UART-SPI Interface to easily debug or script SPI devices with a PC / UART-Interface. 
        Copyright (C) 2020  Alex Hold (UnHold)
        Copyright (C) 2020  Alina Schärmer (tabaluga34 / ina06)

        This program is free software: you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation, either version 3 of the License, or
        any later version.

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/



//this ifndef is just here for a script to create a single main file (used by the arduino ide) and to not include non-existend files 
#ifndef ARDUINO_IDE
#include "UART.h"
#else
#include <Arduino.h>
#endif





//----------------Serial Communication Functions-------------------

//this functions will send an information message
void sendInformation(String info) { 
  #if !defined(UNIT_TEST) || defined(UNIT_TEST_DEBUG)
  Serial.println("I:" + info);
  #endif
}


//only usable if debug is defined 
#ifdef DEBUG
//this functions will send a warning message
void sendWarning(String text) {
  #if !defined(UNIT_TEST) || defined(UNIT_TEST_DEBUG)
  Serial.println("W:" + text);
  #endif
}
#endif

//this function will send an error message with a given category and id
//for the categories and ids please look at the doc
void sendError(String category, uint8_t id) {

  #if !defined(UNIT_TEST) || defined(UNIT_TEST_DEBUG)
  Serial.println("E:" + category + ":" + String(id, DEC));
  #endif

  //this is for the unit test framework to check error messages
  #ifdef UNIT_TEST 
    uinittest_logError(category, id);
  #endif
}


//only usable if debug is defined 
#ifdef DEBUG
//this function will send an error message with a given category, id and text
//for the categories and ids please look at the doc
void sendError(String category, uint8_t id, String text) {

  #if !defined(UNIT_TEST) || defined(UNIT_TEST_DEBUG)
  Serial.println("E:" + category + ":" + String(id, DEC) + ":" + text);
  #endif

  //this is for the unit test framework to check error messages
  #ifdef UNIT_TEST 
    uinittest_logError(category, id);
  #endif
}
#endif

//this function is used to send back the response you get from the spi controller / slave
void sendRespone(uint8_t spi_id, uint8_t slave_id, uint8_t bitcount, uint32_t data, uint8_t number_base){

  #if !defined(UNIT_TEST) || defined(UNIT_TEST_DEBUG)
  Serial.println("R:" + String(spi_id, number_base) + ":" + String(slave_id, number_base) + ":" + String(bitcount, number_base) + ":" + String(data, number_base));
  #endif
}

void uartSetup(){
    //init the serial communication
  Serial.begin(UART_SPEED);

  //wait until the serial communication is ready (tty connected)
  while (!Serial)
  {
    delay(1000);
  }
}


String pullUART(){

  if(Serial.available() > 0){
    //new command is incomming
    String data = Serial.readStringUntil('\n');
    return data;
  }else{
    return "";
  }
}
