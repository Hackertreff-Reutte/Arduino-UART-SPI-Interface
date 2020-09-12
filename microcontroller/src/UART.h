#ifndef UART_H
#define UART_H

#include <Arduino.h>
#include "settings.h"

void sendInformation(String info);
void sendWarning(String text);
void sendError(String category, uint8_t id);
void sendError(String category, uint8_t id, String text);
void sendRespone(uint8_t spi_id, uint8_t slave_id, uint8_t bitcount, String data);
void uartSetup2();

#endif