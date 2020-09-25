#ifndef PARSER_H
#define PARSER_H

#include <Arduino.h>
#include <SPIc.h>
#include <UART.h>
#include "settings.h"
#include "errorcodes.h"

bool isSpace(char c);
String trimWhitespaces(String data);
String* getCommandInfo(String data, char separator);
void getCommand(String data);


#endif