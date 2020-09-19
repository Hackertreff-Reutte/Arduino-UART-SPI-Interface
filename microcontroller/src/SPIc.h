#ifndef SPIc_H
#define SPIc_H

#include <Arduino.h>
#include <SPI.h>
#include "settings.h"
#include "errorcodes.h"
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
#endif


#endif