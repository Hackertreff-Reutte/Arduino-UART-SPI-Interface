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


#ifndef UNITTEST_UART_INTERCEPTOR_H
#define UNITTEST_UART_INTERCEPTOR_H

//change this add some point (testing without Arduino.h dependency)
#include <Arduino.h>

void uinittest_logError(String category, uint8_t id);
String uinittest_getLastErrorCategory();
uint8_t uinittest_getLastErrorID();
void uinittest_resetErrorUpdatedStatus();
bool uinittest_wasErrorUpdated();
uint8_t uinittest_getErrorUpdatedCount();

#endif