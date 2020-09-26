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


#ifndef SPIc_H
#define SPIc_H

#include <Arduino.h>
#include <SPI.h>
#include "settings.h"
#include "statuscodes.h"
#include "UART.h"

void setupSPI(uint8_t spi_id, uint8_t miso, uint8_t mosi, uint8_t clock,  uint32_t speed, uint8_t bitorder, uint8_t mode);
void setupSlave(uint8_t spi_id, uint8_t slave_id, uint8_t pin);
void updateSpiMode(uint8_t spi_id, uint8_t mode);
void updateSpiSpeed(uint8_t spi_id, uint32_t speed);
void stopSpi(uint8_t spi_id);
uint32_t transfer(uint8_t spi_id, uint8_t slave_id, uint8_t bitcount, uint32_t data);
void startSpiBulkTransfer(uint8_t spi_id, uint8_t slave_id);
void stopSpiBulkTransfer(uint8_t spi_id);
uint8_t bulkTransfer(uint8_t spi_id, uint8_t data);

//there are some private function for bound checks
//this is an importanted guard for the unit testing framework (otherwise multiple definitions)
#ifdef UNIT_TEST 
bool isSPI_ID_inBounds(uint8_t spi_id);
bool isSPI_notInitialized(uint8_t spi_id);
bool isSPI_Initialized(uint8_t spi_id);
bool isSPI_Slave_inBounds(uint8_t spi_id, uint8_t slave_id);
bool isSPI_Slave_notInitialized(uint8_t spi_id, uint8_t slave_id);
bool isSPI_Slave_Initialized(uint8_t spi_id, uint8_t slave_id);
bool isSPI_notTransmitting(uint8_t spi_id);
bool isSPI_Transmitting(uint8_t spi_id);
bool isValidBitCount(uint8_t bitcount);
bool isValidMode(uint8_t mode);
bool isValidBitorder(uint8_t bitorder);
#endif


#endif