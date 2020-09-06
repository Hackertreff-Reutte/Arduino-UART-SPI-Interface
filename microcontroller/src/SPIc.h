#ifndef SPIc_H
#define SPIc_H

extern void setupSPI(uint8_t spi_id, uint8_t miso, uint8_t mosi, uint8_t clock,  uint32_t speed, uint8_t bitorder, uint8_t mode);
extern void setupSlave(uint8_t spi_id, uint8_t slave_id, uint8_t pin);
extern void updateSpiMode(uint8_t spi_id, uint8_t mode);
extern void updateSpiSpeed(uint8_t spi_id, uint32_t speed);
extern void stopSpi(uint8_t spi_id);
extern uint32_t transfer(uint8_t spi_id, uint8_t slave_id, uint8_t bitcount, uint32_t data);
extern void startSpiBulkTransfer(uint8_t spi_id, uint8_t slave_id);
extern void stopSpiBulkTransfer(uint8_t spi_id);
extern uint8_t bulkTransfer(uint8_t spi_id, uint8_t data);


#endif