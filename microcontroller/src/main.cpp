#include <Arduino.h>

//this ifndef is just here for a script to create a single main file (used by the arduino ide) and to not include non-existend files 
#ifndef ARDUINO_IDE
#include "SPIc.h"
#include "UART.h"
#endif





//this is an importanted guard for the unit testing framework (otherwise multiple definitions)
#ifndef UNIT_TEST 

void setup() {


}

void loop() {
  
}

#endif