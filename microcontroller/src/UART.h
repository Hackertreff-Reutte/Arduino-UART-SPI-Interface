#ifndef UART_H
#define UART_H

#include <Arduino.h>
#include "settings.h"
#include "errorcodes.h"

//header for the testing framework
#ifdef UNIT_TEST 
    //needed to get the error report of the spi functions
    #include "unittest_UART_interceptor.h"
#endif


//only usable if debug is defined 
#ifdef DEBUG
void sendError(String category, uint8_t id, String text);
void sendWarning(String text);
#endif

void sendInformation(String info);
void sendError(String category, uint8_t id);
void sendRespone(uint8_t spi_id, uint8_t slave_id, uint8_t bitcount, String data);
void uartSetup();

#endif