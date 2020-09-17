#ifndef UART_H
#define UART_H

#include <Arduino.h>
#include "settings.h"
#include "errorcodes.h"


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