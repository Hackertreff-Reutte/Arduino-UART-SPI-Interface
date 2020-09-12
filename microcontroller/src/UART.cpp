
//this ifndef is just here for a script to create a single main file (used by the arduino ide) and to not include non-existend files 
#ifndef ARDUINO_IDE
#include "UART.h"
#else
#include <Arduino.h>
#endif





//----------------Serial Communication Functions-------------------

//this functions will send an information message
void sendInformation(String info) {
  Serial.println("I:" + info);
}


//this functions will send a warning message
void sendWarning(String text) {
  Serial.println("W:" + text);
}


//this function will send an error message with a given category and id
//for the categories and ids please look at the doc
void sendError(String category, uint8_t id) {
  Serial.println("E:" + category + ":" + String(id, DEC));
}

//this function will send an error message with a given category, id and text
//for the categories and ids please look at the doc
void sendError(String category, uint8_t id, String text) {
  Serial.println("E:" + category + ":" + String(id, DEC) + ":" + text);
}

//this function is used to send back the response you get from the spi controller / slave
void sendRespone(uint8_t spi_id, uint8_t slave_id, uint8_t bitcount, String data){
  Serial.println("R:" + String(spi_id, DEC) + ":" + String(slave_id, DEC) + ":" + String(bitcount, DEC) + ":" + data);
}

void uartSetup2(){
    //init the serial communication
  Serial.begin(UART_SPEED);

  //wait until the serial communication is ready (tty connected)
  while (!Serial)
  {
    delay(1000);
  }
}

