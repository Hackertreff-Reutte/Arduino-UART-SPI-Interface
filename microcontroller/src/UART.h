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