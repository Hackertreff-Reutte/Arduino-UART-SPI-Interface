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


#include "unittest_UART_interceptor.h"


String lastErrorCategory = "";
uint8_t lastErrorID = -1;
boolean errorUpdatedStatus = false;
uint8_t updatedCount = 0;

void uinittest_logError(String category, uint8_t id){
    lastErrorCategory = category;
    lastErrorID = id;
    errorUpdatedStatus = true;
    updatedCount++;
}

String uinittest_getLastErrorCategory(){
    return lastErrorCategory;
}


uint8_t uinittest_getLastErrorID(){
    return lastErrorID;
}

void uinittest_resetErrorUpdatedStatus(){
    errorUpdatedStatus = false;
    updatedCount = 0;
}

bool uinittest_wasErrorUpdated(){
    return errorUpdatedStatus;
}

uint8_t uinittest_getErrorUpdatedCount(){
    return updatedCount;
}